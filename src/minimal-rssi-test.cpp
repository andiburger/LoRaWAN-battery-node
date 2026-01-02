#include <Arduino.h>
#include <RadioLib.h>

// T-Beam SX1262 Pins
#define RADIO_CS_PIN    18
#define RADIO_DIO1_PIN   26
#define RADIO_RST_PIN    23
#define RADIO_BUSY_PIN   19
#define RADIO_TCXO_ENABLE 4  // falls TCXO Board (T-Beam V1.1+)

// SX1262 Radio Objekt
SX1262 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("T-Beam SX1262 RX Test");

  // TCXO einschalten
  pinMode(RADIO_TCXO_ENABLE, OUTPUT);
  digitalWrite(RADIO_TCXO_ENABLE, HIGH);
  delay(5);

  // Radio initialisieren (868 MHz, 125 kHz, SF7, CR 4/5, 14 dBm, TCXO aktiviert)
  int state = radio.begin(868.1, 125.0, 7, 5, 0x34, 14, 0, 0, true);
  if(state != RADIOLIB_ERR_NONE){
    Serial.print("Radio init failed, code: ");
    Serial.println(state);
    while(1);
  }

  // Optional: DIO2 als RF-Switch konfigurieren, falls Board benötigt
  // radio.setDio2AsRfSwitch();

  Serial.println("Radio initialized successfully.");
}

void loop() {
  uint8_t buf[256];
  int state = radio.readData(buf, sizeof(buf));

  if(state == RADIOLIB_ERR_NONE) {
    Serial.print("RX OK | RSSI: ");
    Serial.print(radio.getRSSI());
    Serial.print(" dBm | SNR: ");
    Serial.println(radio.getSNR());

    Serial.print("Payload: ");
    for(int i = 0; i < sizeof(buf); i++) {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  else if(state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.print("RX timeout | RSSI: ");
    Serial.println(radio.getRSSI());
  }
  else {
    Serial.print("RX error: ");
    Serial.println(state);
  }

  // wieder in RX gehen!
  radio.startReceive();
  delay(1000);
}