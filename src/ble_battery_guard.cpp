#include "ble_battery_guard.h"
#include <NimBLEDevice.h>

BatteryData battery = {
    .valid = false,
    .voltage = 0.0,
    .temperature = 0.0,
    .status = 0,
    .lastUpdateMs = 0
};

static NimBLEScan* pBLEScan;

void bleBatteryInit() {
    Serial.println("[BLE] Initializing BLE stub (dummy)");

    /*NimBLEDevice::init("BatteryGuardNode");
    pBLEScan = NimBLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->start(0, true);*/
}

void bleBatteryLoop() {
    // Dummy: Setze Werte sofort
    battery.voltage = 12.34;
    battery.temperature = 21.5;
    battery.status = 1;
    battery.valid = true;
    battery.lastUpdateMs = millis();
}

bool bleBatteryHasData() {
    return battery.valid;
}