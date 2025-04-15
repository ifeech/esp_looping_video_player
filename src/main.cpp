// https://github.com/moononournation/RGB565_video/blob/master/SD_MJPEG_video/SD_MJPEG_video.ino

#define MJPEG_FILENAME "/t1_240_30fps.mjpeg"
#define MJPEG_BUFFER_SIZE (300 * 220 * 2 / 4)

#include <WiFi.h>
#include <SD.h>
#include <SD_MMC.h>

#define SD_CS 5
#define SD_MOSI 21
#define SD_MISO 19
#define SD_SCK 17

SPIClass spiSD(HSPI);

#include <Arduino_GFX_Library.h>
#define TFT_BRIGHTNESS 128
#define TFT_BL 16

Arduino_HWSPI *bus = new Arduino_HWSPI(2 /* DC */, 15 /* CS */, SCK, MOSI, MISO);
Arduino_GFX *gfx = new Arduino_ST7789(bus, 4 /* RST */, 2 /* rotation */);

// Arduino_HWSPI *bus = new Arduino_HWSPI(2 /* DC */, 15 /* CS */, SCK, MOSI, MISO);
// Arduino_GFX *gfx = new Arduino_ILI9341(bus, 4 /* RST */, 0 /* rotation */);

#include "MjpegClass.h"
static MjpegClass mjpeg;

File vFile;
uint8_t *mjpeg_buf = nullptr;

void setup()
{
  WiFi.mode(WIFI_OFF);
  Serial.begin(115200);

  // Init Video
  gfx->begin();
  gfx->fillScreen(BLACK);

#ifdef TFT_BL
  ledcSetup(1, 12000, 8);       // 12 kHz PWM, 8-bit resolution
  ledcAttachPin(TFT_BL, 1);     // assign TFT_BL pin to channel 1
  ledcWrite(1, TFT_BRIGHTNESS); // brightness 0 - 255
#endif
  
  spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  // Init SD card
  if (!SD.begin(SS, spiSD, 80000000)) /* SPI bus mode */
  {
    Serial.println(F("ERROR: SD card mount failed!"));
    gfx->println(F("ERROR: SD card mount failed!"));

    while (true) ; // Stop execution
  }

  vFile = SD.open(MJPEG_FILENAME);
  if (!vFile || vFile.isDirectory())
  {
    Serial.println(F("ERROR: Failed to open " MJPEG_FILENAME " file for reading"));
    gfx->println(F("ERROR: Failed to open " MJPEG_FILENAME " file for reading"));

    while (true) ; // Stop execution
  }

  mjpeg_buf = (uint8_t *)heap_caps_malloc(MJPEG_BUFFER_SIZE, MALLOC_CAP_DMA);
  if (!mjpeg_buf)
  {
    Serial.println(F("mjpeg_buf malloc failed!"));
    delay(1000); // Wait and try again

    while (true) ; // Stop execution
  }
  
  mjpeg.setup(vFile, mjpeg_buf, (Arduino_TFT *)gfx, true);
}

void loop()
{
  // Read and play video in a loop
  while (mjpeg.readMjpegBuf())
  {
    mjpeg.drawJpg();
  }

  vFile.seek(0); // return the pointer to the beginning of the file
  mjpeg.clearIndex();

  delay(100);
}