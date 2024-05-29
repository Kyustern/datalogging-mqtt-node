#include <string.h>
#include <Arduino.h>
#include <U8g2lib.h>

#include <airmon.h>
#include <screen.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

const uint8_t timeFont16 = *u8g2_font_inb16_mr;
const uint8_t height = 32;
const uint8_t width = 128;

void drawHMS(uint8_t h, uint8_t m, uint8_t s)
{
  u8g2.setFont(u8g2_font_inb16_mr);
}

void scrollText(String input, uint8_t step)
{
  u8g2.setFont(u8g2_font_inb16_mr);

  uint16_t width = u8g2.getUTF8Width(input.c_str());
  Serial.print("width : ");
  Serial.println(width);

  for (int16_t i = 0; i < width; i = i + step)
  {
    u8g2.clearBuffer();
    u8g2.drawStr((4 - i), 30, input.c_str());
    u8g2.sendBuffer();
  }

  u8g2.clearBuffer();
}

void printInline(String input, u8g2_uint_t lineY)
{
  u8g2.setFont(u8g2_font_inb16_mr);
  // uint16_t width = u8g2.getUTF8Width(input.c_str());
  u8g2.drawStr(4, lineY, input.c_str());
}

void drawValues(ENS_Reading *readingPointer)
{
  u8g2.clearBuffer();
  String aqi = String(readingPointer->aqi);
  String tvoc = String(readingPointer->tvoc);
  String eco2 = String(readingPointer->eco2);
  printInline(aqi, 10);
  printInline(tvoc, 20);
  printInline(aqi, 30);
  u8g2.sendBuffer();
}

const uint8_t fontSize = 16;
const uint8_t steps = fontSize;

void initScreen()
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_inb16_mr);
}
