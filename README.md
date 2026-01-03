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