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