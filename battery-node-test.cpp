#include <Arduino.h>
#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>

// ----------------------------
// TTN OTAA Keys
// ----------------------------
static const u1_t APPEUI[8]  = { 0x0 };
static const u1_t DEVEUI[8]  = { 0x0 };
static const u1_t APPKEY[16] = { 0x0 };

// ----------------------------
// LoRaWAN setup
// ----------------------------
void os_getArtEui (u1_t* buf) { memcpy(buf, APPEUI, 8); }
void os_getDevEui (u1_t* buf) { memcpy(buf, DEVEUI, 8); }
void os_getDevKey (u1_t* buf) { memcpy(buf, APPKEY, 16); }

static osjob_t sendjob;
const unsigned TX_INTERVAL = 30; // 30 seconds

// ----------------------------
// Pin mapping T-Beam SX1276
// ----------------------------
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 23,
    .dio = {26,33,32},
};

void onEvent(ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_TXCOMPLETE:
            Serial.println("EV_TXCOMPLETE (TX finished)");
            break;
        case EV_JOINED:
            Serial.println("EV_JOINED (OTAA join complete)");
            break;
        default:
            Serial.print("Event ");
            Serial.println(ev);
            break;
    }
}

// ----------------------------
// Send fixed test payload
// ----------------------------
void do_send(osjob_t* j){
    uint8_t payload[2] = {0xAB, 0xCD}; // fixed test payload

    LMIC_setTxData2(1, payload, sizeof(payload), 0);
    Serial.print("Sending fixed payload: 0x");
    Serial.print(payload[0], HEX);
    Serial.print(" 0x");
    Serial.println(payload[1], HEX);

    delay(1); // yield to Watchdog

    // Schedule next transmission
    os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
}

// ----------------------------
// LMIC Task
// ----------------------------
void lmicTask(void *pvParameters) {
    Serial.println("LMIC Init Step 1...");
    os_init();
    yield();
    delay(0);

    Serial.println("LMIC Init Step 2...");
    LMIC_reset();
    yield();
    delay(0);

    // Schedule first transmission after 1 second
    os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(1), do_send);

    for (;;) {
        os_runloop_once();
        yield();
        delay(1);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting LoRaWAN test...");

    xTaskCreatePinnedToCore(
        lmicTask,        // Task function
        "LMIC Task",     // Name
        4096,            // Stack size
        NULL,            // Parameters
        1,               // Priority
        NULL,            // Task handle
        0                // Core 0
    );
}

void loop() {
    delay(1000); // Main loop idle; LMIC runs in its own task
}