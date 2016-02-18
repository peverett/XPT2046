#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX_AS.h>    // Core graphics library, with extra fonts.
#include <Adafruit_ILI9341_STM.h> // STM32 DMA Hardware-specific library
#include <XPT2046.h>

// Modify the following two lines to match your hardware
Adafruit_ILI9341_STM screen = Adafruit_ILI9341_STM(
                                            /*cs=*/ 7, 
                                            /*dc=*/ 9, 
                                            /*rst=*/ 8
                                            ); 
XPT2046 touch(/*cs=*/ 31, /*irq=*/ 27, /*spiPortNumber=*/ 2);

static void calibratePoint(uint16_t x, uint16_t y, uint16_t &vi, uint16_t &vj) {
  // Draw cross
  screen.drawFastHLine(x - 8, y, 16, ILI9341_WHITE);
  screen.drawFastVLine(x, y - 8, 16, ILI9341_WHITE);
  while (!touch.isTouching()) {
    delay(10);
  }
  touch.getRaw(vi, vj);
  // Erase by overwriting with black
  screen.drawFastHLine(x - 8, y, 16, ILI9341_BLACK);
  screen.drawFastVLine(x, y - 8, 16, ILI9341_BLACK);
}

void calibrate() {
  uint16_t x1, y1, x2, y2;
  uint16_t vi1, vj1, vi2, vj2;
  touch.getCalibrationPoints(x1, y1, x2, y2);
  calibratePoint(x1, y1, vi1, vj1);
  delay(1000);
  calibratePoint(x2, y2, vi2, vj2);
  touch.setCalibration(vi1, vj1, vi2, vj2);

  char buf[80];
  snprintf(buf, sizeof(buf), "%d,%d,%d,%d", (int)vi1, (int)vj1, (int)vi2, (int)vj2);
  screen.pushColor(ILI9341_WHITE);
  screen.drawCentreString("setCalibrationParams:", 120, 20, 2);
  screen.drawCentreString(buf, 120, 50, 2);
}

void setup() {
  delay(1000);
  screen.begin();
  touch.begin(240, 320);
  screen.fillScreen(ILI9341_BLACK);

  calibrate();  // No rotation!!
}

void loop() {
  // Do nothing
  delay(1000);
}
