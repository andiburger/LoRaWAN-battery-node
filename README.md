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

### LoRaWAN Keys Setup (`keys.h`)

Before uploading the firmware, you must provide your LoRaWAN OTAA keys in a file named `keys.h`. This file is not included in the repository for security reasons.

#### 1. Create `keys.h`

In the root of the project (or as specified in the code), create a new file called `keys.h` with the following contents:

```cpp
#pragma once

// LoRaWAN OTAA keys (replace with your actual values)
static const char *dev_eui = "YOUR_DEVICE_EUI";
static const char *app_eui = "YOUR_APPLICATION_EUI";
static const char *app_key = "YOUR_APPLICATION_KEY";
```

Replace `YOUR_DEVICE_EUI`, `YOUR_APPLICATION_EUI`, and `YOUR_APPLICATION_KEY` with the values from The Things Network (TTN) console. Use the exact format provided by TTN (typically 16-character hexadecimal strings, e.g., `"70B3D57ED004B5C1"`). **Do not swap byte order**—the code expects the standard TTN format.

#### 2. Add `keys.h` to `.gitignore`

To avoid accidentally committing your keys, ensure your `.gitignore` file contains the line:

```
keys.h
```

This will prevent `keys.h` from being tracked by git.

#### 3. Additional Configuration

- **TTN Frequency Plan:** Set your region (e.g., EU868) in the code and TTN console to match.
- **BLE Settings:** Update the BLE address or device name in the code to match your Intact Battery Guard sensor.
- **Deep Sleep:** Adjust `DEEP_SLEEP_ENABLED` and the sleep duration in the code as needed for your use case.

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