# Looping video player on esp32

Plays a short video file (mjpeg) without stopping.

Need: esp32, display ili9341 (SPI) with SD card (or external sd card module).

Set ``MJPEG_FILENAME`` to the name of the video file on the SD card.

## Connecting the display to the esp32

| Display       | ESP32 |
|---------------|-------|
| VCC           | 3.3V  |
| GND           | GND   |
| LED           | 16    |
| TFT_CS        | 15    |
| TFT_SCK       | 18    |
| TFT_MISO      | 19    |
| TFT_MOSI      | 23    |
| TFT_DC        | 2     |
| TFT_RST       | 4     |
| SD_CS         | 5     |
| SD_MOSI       | 21    |
| SD_MISO       | 19    |
| SD_SCK        | 17    |