#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

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
const unsigned TX_INTERVAL = 30; // 30 seconds

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
// Send fixed test payload
// ----------------------------
void do_send(osjob_t* j){
    byte payload[2] = {0xAB, 0xCD}; // fixed test payload

    LMIC_setTxData2(1, payload, sizeof(payload), 0);
    Serial.print("Sending fixed payload: 0x");
    Serial.print(payload[0], HEX);
    Serial.print(" 0x");
    Serial.println(payload[1], HEX);

    // Schedule next transmission
    os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
}

// ----------------------------
// Setup / Loop
// ----------------------------
void setup() {
    Serial.begin(115200);
    delay(100); // allow time for serial monitor to start

    Serial.println("Starting LoRaWAN test...");

    // LoRa Init
    os_init();
    LMIC_reset();

    // Start job
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}