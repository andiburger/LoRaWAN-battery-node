# IntAct Battery Guard – BLE Reverse Engineering

## Overview

This document summarizes the reverse-engineering steps performed so far for the IntAct Battery Guard Bluetooth sensor. The objective is to reliably extract battery-related data (voltage, temperature, status) using an ESP32 and forward it via LoRaWAN.

⸻

### Hardware & Software Context
+ Device: IntAct Battery Guard (BLE)
+ Target platform: ESP32 (LilyGO T-Beam SX1262)
+ Tools:
	+ nRF Connect (Android/iOS)
	+ ESP32 Arduino BLE library
	+ PlatformIO

⸻

## Step 1 – BLE Discovery

### Observations
+ The device is consistently discoverable via BLE scan
+ Advertised primary service UUID:
+ 0xFFF0

### Conclusion
+ The Battery Guard is not a beacon, but a BLE GATT device
+ Advertising alone does not expose measurement data

⸻

## Step 2 – Connection & Service Discovery

### Observations (nRF Connect)

After connecting, the following services are exposed:

UUID	Description
180A	Device Information (standard)
FFF0	Primary Battery Guard service
FEE0	Vendor-specific (likely configuration/firmware)

Conclusion
+ Relevant measurement data is located under service FFF0
+ An explicit BLE connection is required

⸻

## Step 3 – ESP32 Scan Implementation (Diagnostic Mode)

### Design Decisions
+ Use of the classic ESP32 Arduino BLE library (not NimBLE)
+ Rationale:
+ Stable API
+ Callback-based scanning
+ Reliable diagnostics

### Current Capabilities
+ BLE scan with callbacks
+ Output of:
+ MAC address
+ Device name (if present)
+ RSSI
+ Identification of Battery Guard via service UUID 0xFFF0

⸻

## Step 4 – Manufacturer Data Inspection

### Purpose
+ Determine whether battery metrics are transmitted in advertising frames

### Status
+ Manufacturer data is logged in hexadecimal form
+ Further analysis required to correlate payload changes with:
+ Voltage variation
+ Load changes
+ Temperature changes

⸻

## Current Implementation Status

Component	Status
BLE scan	Stable
Battery Guard identification	Reliable
LoRaWAN integration	Decoupled
BLE data decoding	Pending
GATT connect & read	Not implemented

Dummy values are currently used to keep the LoRaWAN payload structure stable.

⸻

## Next Steps
1.	Using nRF Connect:
	+ Inspect service FFF0
	+ Document characteristic UUIDs and properties (READ / NOTIFY)
2.	Verify whether manufacturer data alone is sufficient
3.	Implement ESP32 logic:
	+ Stop scan
	+ Connect to device
	+ Read characteristics or subscribe to notifications

⸻

## Summary
+ The IntAct Battery Guard is a GATT-based BLE device with a custom primary service (FFF0)
+ The current reverse-engineering approach is correct and stable
+ All prerequisites for GATT-level data extraction are in place

Status: Ready for live characteristic analysis