#include "sdl.h"
#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Sdl::Sdl(){
  for(int i; i<=7; i++){
    buttonsState[i] = 0;
  }
}

void Sdl::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color){
  OLED.drawBitmap(x, y, bitmap, w, h, color);
}

void Sdl::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  OLED.fillRect(x, y, w, h, color);
}

void Sdl::fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color){
  OLED.fillRoundRect(x0, y0, w, h, radius, color);
}

void Sdl::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color){
  OLED.drawLine(x0, y0, x1, y1, color);
}

void Sdl::drawRect(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t color){
  OLED.drawRect(x0, y0, w, h, color);
}

void Sdl::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
  OLED.fillCircle(x0, y0, r, color);
}

void Sdl::clearDisplay(){
  OLED.clearDisplay();
}

void Sdl::display(){
  OLED.display();
}

void Sdl::begin(){
  OLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void Sdl::setRotation(uint8_t x){
  OLED.setRotation(x);
}

void Sdl::setTextSize(uint8_t s){
  OLED.setTextSize(s);
}

void Sdl::setTextColor(uint16_t c){
  OLED.setTextColor(c);
}

void Sdl::setCursor(int16_t x, int16_t y){
  OLED.setCursor(x, y);
}

void Sdl::print(String t){
  OLED.print(t);
}

void Sdl::print(char c){
  OLED.print(c);
}



bool Sdl::checkButtons(){
  bool input = 0;
  for(int i = 0; i < 9; i++){
    buttonsState[i] = !digitalRead(buttons[i]);
    if(!digitalRead(buttons[i])){
      input = 1;
    }
  }
  return (input);
}
