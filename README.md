# LoRaWAN Battery Node

## Project Overview

The LoRaWAN Battery Node project demonstrates how to build a low-power battery monitoring device using a T-Beam development board integrated with the Intact Battery Guard BLE sensor. This device collects battery status data and sends it over LoRaWAN to The Things Network (TTN) for remote monitoring. The project is ideal for IoT enthusiasts and developers interested in battery-powered sensor nodes with long-range wireless communication.

## Hardware Requirements

- TTGO T-Beam development board (compatible with LoRaWAN and GPS)
- Intact Battery Guard BLE sensor
- Battery or power source for the T-Beam
- USB cable for programming the T-Beam
- Optional: Antenna for LoRa communication

## Software Requirements

- Arduino IDE (version 1.8.13 or later recommended)
- LoRaWAN libraries compatible with the T-Beam board
- TTN account for device registration and network access
- BLE library for Arduino to interface with the Intact Battery Guard sensor

## Installation

1. **Set up Arduino IDE:**
   - Download and install the Arduino IDE from [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software).
   - Install the ESP32 board definitions via the Boards Manager (search for "ESP32" by Espressif Systems).
   - Install required libraries (LoRaWAN, BLE) via the Library Manager.

2. **Connect the T-Beam:**
   - Connect the TTGO T-Beam to your computer using a USB cable.
   - Select the correct board (TTGO T-Beam) and port in the Arduino IDE.

3. **Upload the Firmware:**
   - Open the project sketch.
   - Configure your LoRaWAN keys and settings (see Configuration below).
   - Upload the sketch to the T-Beam.

## PlatformIO Setup

You can also build and upload the firmware using PlatformIO, which offers a powerful and flexible development environment.

1. **Install PlatformIO:**
   - Install Visual Studio Code if you haven't already: [https://code.visualstudio.com/](https://code.visualstudio.com/)
   - Install the PlatformIO extension from the VSCode marketplace.

2. **Open the Project:**
   - Open the project folder containing the source code and `platformio.ini` file in VSCode.

3. **Configure `platformio.ini`:**
   - Create or edit the `platformio.ini` file with the following example configuration:

   ```ini
   [env:tbeam]
   platform = espressif32
   board = ttgo-lora32-v2
   framework = arduino
   monitor_speed = 115200
   build_flags =
       -DCORE_DEBUG_LEVEL=5
       -DDEEP_SLEEP_ENABLED
   ```

   - The `build_flags` allow you to enable debug output and optimize for deep sleep power saving.

4. **Build and Upload:**
   - Use the PlatformIO sidebar or command palette to build the project (`PlatformIO: Build`).
   - Connect your T-Beam via USB.
   - Upload the firmware using `PlatformIO: Upload`.

5. **Monitor Serial Output:**
   - Open the serial monitor with `PlatformIO: Monitor` to view debug messages and sensor data.
   - Ensure the baud rate matches `monitor_speed` (115200 in the example).

PlatformIO provides better dependency management and build customization compared to the Arduino IDE, making it ideal for advanced users and continuous integration setups.

## Configuration

Before uploading, configure the following parameters in the sketch or configuration file:

- **LoRaWAN Keys:**
  - `DEV_EUI` (Device EUI)
  - `APP_EUI` (Application EUI)
  - `APP_KEY` (Application Key)

- **TTN Region:**
  - Set the appropriate frequency plan for your region (e.g., EU868, US915).

- **BLE settings:**
  - Ensure the BLE address or name of the Intact Battery Guard sensor is correctly set.

Refer to TTN documentation for generating and managing your LoRaWAN keys: [https://www.thethingsnetwork.org/docs/](https://www.thethingsnetwork.org/docs/)

## Usage

1. Power on the T-Beam with the battery connected.
2. The device will scan for the Intact Battery Guard BLE sensor and retrieve battery data.
3. Data is encoded and transmitted via LoRaWAN to TTN.
4. Monitor your device data on the TTN Console or integrate it with your preferred backend.
5. Use the data for battery health monitoring, alerts, or analytics.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Home Assistant Integration

You can integrate the LoRaWAN Battery Node data from TTN into Home Assistant for real-time monitoring and automation.

1. Configure the TTN integration in Home Assistant by adding your application and device details.
2. Use the following YAML example to define sensors that represent battery status data received from TTN:

```yaml
sensor:
  - platform: mqtt
    name: "Battery Voltage"
    state_topic: "ttn/<application_id>/devices/<device_id>/up"
    value_template: "{{ value_json.uplink_message.decoded_payload.battery_voltage }}"
    unit_of_measurement: "V"

  - platform: mqtt
    name: "Battery Level"
    state_topic: "ttn/<application_id>/devices/<device_id>/up"
    value_template: "{{ value_json.uplink_message.decoded_payload.battery_level }}"
    unit_of_measurement: "%"
```

Replace `<application_id>` and `<device_id>` with your TTN application and device identifiers. Adjust the `value_template` paths according to your payload structure.

For more details on setting up MQTT and TTN integration with Home Assistant, refer to the official Home Assistant documentation: [https://www.home-assistant.io/integrations/mqtt/](https://www.home-assistant.io/integrations/mqtt/)
# LoRaWAN Battery Node

## Project Overview

The LoRaWAN Battery Node is a low-power IoT sensor built around the TTGO T-Beam (ESP32 + SX1262 LoRa + AXP2101 PMU) that collects battery status data from an Intact Battery Guard BLE sensor and transmits it over LoRaWAN to The Things Network (TTN). It demonstrates battery monitoring, LoRaWAN communication, and deep-sleep power optimization. 

This project is suitable for IoT developers and enthusiasts who want a fully integrated battery monitoring node with long-range wireless connectivity.

---

## Hardware Requirements

- **TTGO T-Beam SX1262** with AXP2101 PMU
- **Intact Battery Guard** BLE sensor
- Battery or other power source for the T-Beam
- USB cable for programming
- Optional: LoRa antenna (external)

---

## Software Requirements

- PlatformIO (recommended) or Arduino IDE
- RadioLib for LoRa/SX1262
- LilyGo-LoRa-Series library (includes U8g2 and other dependencies)
- BLE library compatible with ESP32
- TTN account for device registration and LoRaWAN network access

---

## PlatformIO Setup

PlatformIO is recommended for building, dependency management, and upload.

1. **Install PlatformIO:**
   - Install VSCode: [https://code.visualstudio.com/](https://code.visualstudio.com/)
   - Install the PlatformIO extension.

2. **Open the Project:**
   - Open the folder containing `platformio.ini` in VSCode.

3. **Configure `platformio.ini`:**
   Example configuration for TTGO T-Beam with SX1262:

   ```ini
   [env:tbeam_sx1262]
   platform = espressif32
   board = ttgo-lora32-v2
   framework = arduino
   monitor_speed = 115200

   lib_extra_dirs = lib/U8g2, lib/RadioLib, lib/XPowersLib

   build_flags =
       -DCORE_DEBUG_LEVEL=5
       -DDEEP_SLEEP_ENABLED
       -DT_BEAM_SX1262
   ```

   Notes:
   - `lib_extra_dirs` points to submodules or local libraries.
   - `build_flags` include debug output and deep sleep optimizations.

4. **Build & Upload:**
   - Connect the T-Beam via USB.
   - Build with `PlatformIO: Build`.
   - Upload using `PlatformIO: Upload`.

5. **Monitor Serial Output:**
   - Use `PlatformIO: Monitor` at 115200 baud.
   - Debug messages include RSSI, SNR, LoRaWAN join attempts, and sensor readings.

---

## Configuration

Before uploading, configure these parameters:

- **LoRaWAN Keys (OTAA):**
  - `DEV_EUI`
  - `APP_EUI`
  - `APP_KEY`
  
  Make sure you use the format required by RadioLib OTAA examples (no LSB swap).

- **TTN Frequency Plan:**
  - Example: EU868
  - Confirm that your region matches TTN settings.

- **BLE Settings:**
  - Set the correct BLE address or device name for the Intact Battery Guard.

- **Deep Sleep:**
  - Adjust `DEEP_SLEEP_ENABLED` and sleep duration in your sketch if using low-power mode.

---

## Usage

1. Power on the T-Beam.
2. The device scans for the Intact Battery Guard BLE sensor and retrieves battery data.
3. Data is encoded and transmitted via LoRaWAN to TTN.
4. Monitor uplinks in the TTN console or integrate via MQTT to your backend.
5. Optionally integrate with **Home Assistant** for real-time monitoring.

Example MQTT sensors for Home Assistant:

```yaml
sensor:
  - platform: mqtt
    name: "Battery Voltage"
    state_topic: "ttn/<application_id>/devices/<device_id>/up"
    value_template: "{{ value_json.uplink_message.decoded_payload.battery_voltage }}"
    unit_of_measurement: "V"

  - platform: mqtt
    name: "Battery Level"
    state_topic: "ttn/<application_id>/devices/<device_id>/up"
    value_template: "{{ value_json.uplink_message.decoded_payload.battery_level }}"
    unit_of_measurement: "%"
```

Replace `<application_id>` and `<device_id>` with your TTN identifiers. Adjust the paths based on your payload.

---

## Debugging Tips

- RSSI around `-112 dBm` can indicate weak or no LoRaWAN gateway coverage.
- Join failures (`-1116`) may result from incorrect keys, frequency plan, or lack of gateway.
- RX/TX loopback tests can be performed with RadioLib SX1262 minimal examples.
- Serial debug output helps identify whether the SX1262 module initializes correctly and receives downlinks.

---

## Dependencies & Submodules

Recommended approach: use submodules for consistent library versions:

- **LilyGo-LoRa-Series** → contains U8g2 and other LilyGo libraries.
- **RadioLib**
- **XPowersLib**

Submodules allow PlatformIO to build without manually copying libraries into `lib/`.

---

## License

MIT License. See LICENSE file.

---

This setup provides a ready-to-build LoRaWAN battery node for testing, debugging, and integration with TTN or Home Assistant.