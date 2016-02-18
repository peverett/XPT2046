#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX_AS.h>    // Core graphics library, with extra fonts.
#include <Adafruit_ILI9341_STM.h> // STM32 DMA Hardware-specific library
#include <XPT2046.h>

// Modify the following two lines to match your hardware
// Also, update calibration parameters below, as necessary
Adafruit_ILI9341_STM screen = Adafruit_ILI9341_STM(
                                            /*cs=*/ 7, 
                                            /*dc=*/ 9, 
                                            /*rst=*/ 8
                                            ); 
XPT2046 touch(/*cs=*/ 31, /*irq=*/ 27, /*spiPortNumber=*/ 2);

void setup() {
  delay(1000);
  screen.begin();
  //ucg.begin(UCG_FONT_MODE_SOLID);
  touch.begin(240, 320);  // Must be done before setting rotation
  screen.fillScreen(ILI9341_BLACK);

  // Replace these for your screen module
  touch.setCalibration(248, 1700, 1743, 309);
}

static uint16_t prev_x = 0xffff, prev_y = 0xffff;

void loop() {
  if (touch.isTouching()) {
    uint16_t x, y;
    touch.getPosition(x, y);
    if (prev_x == 0xffff) {
      screen.drawPixel(x, y, ILI9341_CYAN);
    } else {
      screen.drawLine(prev_x, prev_y, x, y, ILI9341_CYAN);
    }
    prev_x = x;
    prev_y = y;
  } else {
    prev_x = prev_y = 0xffff;
  }
  delay(20);
}
