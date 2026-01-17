#pragma once
#include <Arduino.h>

/**
 * @brief Holds the latest battery values received from the BLE Battery Guard.
 */
struct BatteryData {
    bool valid;          // Whether the data is valid

    float voltage;       // Voltage in volts
    float temperature;   // Temperature in Celsius
    uint8_t status;      // Status flags (bitfield)

    uint32_t lastUpdateMs; // Timestamp of last update in milliseconds
};

// Global instance of the battery data (defined in ble_battery_guard.cpp)
extern BatteryData battery;

/**
 * @brief Initialize the BLE Battery Guard scanning and connection.
 */
void bleBatteryInit();

/**
 * @brief Periodically called in loop() to maintain BLE connection
 *        and process notifications.
 */
void bleBatteryLoop();

/**
 * @brief Checks if valid battery data has been received.
 * @return true if data is valid, false otherwise
 */
bool bleBatteryHasData();