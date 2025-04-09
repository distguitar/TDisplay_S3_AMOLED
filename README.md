# Desktop Telemetry Display
## Hardware Requirements
* LILYGO T-Display-S3 AMOLED ESP32-S3 with 1.91 Inch RM67162 Display TTGO Development Board Wireless Module (https://www.amazon.com/dp/B0C2T6T8YC)

## Software Requirements
* Arduino IDE
* Visual Studio 2022 Community Edition

## Arduino Libraries Required
* LilyGo-AMOLED-Series
* Arduino Core for ESP32 (https://github.com/espressif/arduino-esp32)
* Arduino_GFX_Library (https://github.com/moononournation/Arduino_GFX.git)

## Visual Studio 2022
* .NET 8

## Run Instructions
* Load Arduino project
* Set <SSID> and <PASS> on lines 13 and 14 in "T_Display_S3_AMOLED_8.ino"
* Compile and Deploy
* Connect to Serial Monitor to retrieve IP address
* Open DotNet project
* Change IP Address to Module's IP address in "UpdateTelemetry.cs" line 15
* Build and Run
