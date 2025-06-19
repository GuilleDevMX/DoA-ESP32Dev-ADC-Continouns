# DoA-ESP32Dev-ADC-Continouns

This repository implements data acquisition from an array of Electret microphones using the ESP32 with the [IDF Version 5.4.1 framework](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html), 
using the [I2S sound communication protocol](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/i2s.html), and the framework's native library [```esp_adc/adc_continuous.h```](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s2/api-reference/peripherals/adc_continuous.html).

# Requirements

For this project, we worked with the [ESP32-devKitC]((https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/index.html)) board. In the Visual Studio Code text editor, and with the [PlatformIO](https://platformio.org/) extension.

Within the project folder, the platformio.ini file specifies the card and settings for the PlatformIO project.

The latest version of the IDF Tool Chain is required.

- [IDF Tool Chain 5.4.1](https://docs.platformio.org/en/latest/platforms/espressif32.html)

# Connection of the HW-484 microphone array with the esp32


| ESP32 PIN | SIGNAL   | HW-484 PIN | MICROPHONE # | SGINAL        |
|-----------|----------|------------|--------------|---------------|
| GPIO32    | ADC CH 4 | A0         | 1            | ANALOG OUTPUT |
| GPIO33    | ADC CH 5 | A0         | 2            | ANALOG OUTPUT |
| GPIO34    | ADC CH 6 | A0         | 3            | ANALOG OUTPUT |
| GPIO35    | ADC CH 7 | A0         | 4            | ANALOG OUTPUT |
| GPIO36    | ADC CH 8 | A0         | 5            | ANALOG OUTPUT |
| GND       | GROUND   | G          | 1            | GROUND        |
| 3.3       | 3.3 V    | +          | 1            | POWER SUPPLY  |

# Flash tool for ESP32

You can use a tool to directly load the compiled file and upload it to your ESP32 module. I recommend use [Flash Tool](https://www.espressif.com/en/support/download/other-tools) from the espressif page.

In the project repository, inside the ```.pio\build\esp32dev\``` folder, you will find the binaries and firmware to upload.


