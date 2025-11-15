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
const unsigned TX_INTERVAL = 600; // 10 Minuten (600 Sekunden)

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
int batteryLevel = 0;

int readBattery() {
    BLEScan* scan = BLEDevice::getScan();
    scan->setActiveScan(true);
    BLEScanResults results = scan->start(5); // 5 Sekunden Scan

    for (int i = 0; i < results.getCount(); i++) {
        BLEAdvertisedDevice device = results.getDevice(i);

        if (device.getName().startsWith("BG-")) { // Intact Battery Guard
            std::string manData = device.getManufacturerData();
            const uint8_t* d = (const uint8_t*)manData.data();

            if (manData.length() >= 2) {
                int mv = (d[0] << 8) | d[1]; // Spannung in mV
                batteryLevel = mv / 1000;    // Volt
                Serial.printf("Battery Guard gefunden: %d V\n", batteryLevel);
                return batteryLevel;
            }
        }
    }

    Serial.println("Battery Guard nicht gefunden, vorheriger Wert verwendet");
    return batteryLevel;
}

// ----------------------------
// LoRa send
// ----------------------------
void do_send(osjob_t* j){
    int batt = readBattery();

    byte payload[2];
    payload[0] = batt; // Batterie %
    payload[1] = 0;    // optional Temperatur o.ä.

    LMIC_setTxData2(1, payload, sizeof(payload), 0);
    Serial.printf("LoRaWAN uplink gesendet: Batterie %d%%\n", batt);

    // Deep Sleep starten
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

    // GPS deaktivieren (Pin EN auf LOW)
    pinMode(12, OUTPUT); 
    digitalWrite(12, LOW);

    // BLE init
    BLEDevice::init("");

    // LoRa init
    os_init();
    LMIC_reset();

    delay(1000);

    // Job starten
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}