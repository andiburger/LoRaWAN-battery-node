#include "ble_battery_guard.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// This callback class is called every time a BLE device is found.
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    // The onResult method is executed for each device found.
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        Serial.print("Found BLE Device: ");
        Serial.print(advertisedDevice.getAddress().toString().c_str());
        
        // Only display the name if it is present
        if (advertisedDevice.haveName()) {
            Serial.print(" | Name: ");
            Serial.print(advertisedDevice.getName().c_str());
        }
        
        // RSSI is the signal strength
        Serial.print(" | RSSI: ");
        Serial.println(advertisedDevice.getRSSI());
    }
};


BatteryData battery = {
    .valid = false,
    .voltage = 0.0,
    .temperature = 0.0,
    .status = 0,
    .lastUpdateMs = 0
};


void bleBatteryInit() {
    Serial.println("[BLE] Initializing BLE scan with callback...");
    BLEDevice::init("LilyGo-BLE");
    BLEScan* pBLEScan = BLEDevice::getScan();

    // Create an instance of our callback class and register it with the scanner.
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    
    // Start a 5-second blocking scan. This is for diagnosis to prevent
    // resource conflicts with LoRaWAN initialization.
    // The 'false' parameter makes it a blocking call.
    Serial.println("[BLE] Starting a 10-second scan...");
    pBLEScan->start(10, false);
    Serial.println("[BLE] Scan finished.");
}

void bleBatteryLoop() {
    // The scan is now running in the background. The loop function is free for
    // other tasks. Found devices are automatically output by the callback.
    
    // Dummy values for the battery (can be removed later)
    battery.voltage = 12.34;
    battery.temperature = 21.5;
    battery.status = 1;
    battery.valid = true;
    battery.lastUpdateMs = millis();
}

bool bleBatteryHasData() {
    return battery.valid;
}