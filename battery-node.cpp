#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

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
BLEAddress batteryAddr("xx:xx:xx:xx:xx:xx"); // MAC deines Intact Battery Guard
int batteryLevel = 0;

int readBattery() {
    // TODO: Hier BLE-Scan und GATT-Lese-Logik einfügen
    // Beispiel: batteryLevel = 80; // Testwert
    return batteryLevel;
}

// ----------------------------
// LoRa send
// ----------------------------
void do_send(osjob_t* j){
    int batt = readBattery();

    byte payload[2];
    payload[0] = batt; // % Batterie
    payload[1] = 0;    // optional Temperatur o.ä.

    LMIC_setTxData2(1, payload, sizeof(payload), 0);
    Serial.println("LoRaWAN uplink gesendet!");
}

// ----------------------------
// Setup / Loop
// ----------------------------
void setup() {
    Serial.begin(115200);

    // GPS deaktivieren (Pin EN kann auf LOW gesetzt werden, falls vorhanden)
    pinMode(12, OUTPUT); // EN Pin GPS
    digitalWrite(12, LOW);

    // LoRa Init
    os_init();
    LMIC_reset();

    // Deep Sleep vorbereiten
    os_setCallback(&sendjob, do_send);
}

void loop() {
    os_runloop_once();
}