#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <esp_sleep.h>

// ----------------------------
// TTN OTAA Keys
// ----------------------------
static const u1_t PROGMEM APPEUI[8] = { 0x70,0xB3,0xD5,0x7E,0xD0,0x00,0x00,0x01 }; // JoinEUI
static const u1_t PROGMEM DEVEUI[8] = { 0x70,0xB3,0xD5,0x7E,0xD0,0x07,0x40,0xEA }; // DevEUI
static const u1_t PROGMEM APPKEY[16] = { 0xB7,0x90,0x87,0x00,0xD0,0x06,0x55,0xA2,0xD3,0x6F,0x9A,0xD7,0x23,0x76,0x63,0x77 };

// ----------------------------
// LoRaWAN setup
// ----------------------------
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16);}

static osjob_t sendjob;
const unsigned TX_INTERVAL = 600; // 10 minutes (600 seconds)

// ----------------------------
// Pin mapping T-Beam SX1262
// ----------------------------
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 23,
    .dio = {26,33,32},
};

// ----------------------------
// BLE Battery Guard
// ----------------------------
#include <vector>
std::vector<int> batteryLevels;

void readBatteryGuards() {
    batteryLevels.clear();

    BLEScan* scan = BLEDevice::getScan();
    scan->setActiveScan(true);
    BLEScanResults results = scan->start(5); // 5 seconds scan

    for (int i = 0; i < results.getCount(); i++) {
        BLEAdvertisedDevice device = results.getDevice(i);

        if (device.getName().startsWith("BG-")) { // Intact Battery Guard
            std::string manData = device.getManufacturerData();
            const uint8_t* d = (const uint8_t*)manData.data();

            if (manData.length() >= 2) {
                int mv = (d[0] << 8) | d[1]; // Voltage in mV
                int voltage = mv / 1000;    // Volts
                batteryLevels.push_back(voltage);
            }
        }
    }

    if (batteryLevels.size() > 0) {
        Serial.print("Found Battery Guard voltages (V): ");
        for (size_t i = 0; i < batteryLevels.size(); i++) {
            Serial.printf("%d", batteryLevels[i]);
            if (i < batteryLevels.size() - 1) Serial.print(", ");
        }
        Serial.println();
    } else {
        Serial.println("No Battery Guard sensors found");
    }
}

// ----------------------------
// LoRa send
// ----------------------------
void do_send(osjob_t* j){
    readBatteryGuards();

    byte payload[4];
    size_t count = batteryLevels.size() < 4 ? batteryLevels.size() : 4;

    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            payload[i] = batteryLevels[i];
        }
        // Fill remaining slots with 0 if fewer than 4 readings
        for (size_t i = count; i < 4; i++) {
            payload[i] = 0;
        }
    } else {
        // No sensors found, fill payload with 0xFF to indicate no sensor
        for (size_t i = 0; i < 4; i++) {
            payload[i] = 0xFF;
        }
    }

    LMIC_setTxData2(1, payload, sizeof(payload), 0);

    Serial.print("LoRaWAN uplink sent: Battery voltages (V) ");
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            Serial.printf("%d", payload[i]);
            if (i < count - 1) Serial.print(", ");
        }
        if (count < 4) {
            Serial.print(", ");
            for (size_t i = count; i < 4; i++) {
                Serial.printf("0");
                if (i < 3) Serial.print(", ");
            }
        }
    } else {
        Serial.print("No sensors found, sent fallback payload: ");
        for (size_t i = 0; i < 4; i++) {
            Serial.printf("0xFF");
            if (i < 3) Serial.print(", ");
        }
    }
    Serial.println();

    // Start deep sleep
    Serial.println("Entering deep sleep for 10 minutes...");
    esp_sleep_enable_timer_wakeup(TX_INTERVAL * 1000000ULL);
    esp_deep_sleep_start();
}

// ----------------------------
// Setup / Loop
// ----------------------------
void setup() {
    Serial.begin(115200);
    delay(100);

    // Disable GPS (Pin EN to LOW)
    pinMode(12, OUTPUT); 
    digitalWrite(12, LOW);

    // BLE init
    BLEDevice::init("");

    // LoRa init
    os_init();
    LMIC_reset();

    delay(1000);

    // Start job
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}