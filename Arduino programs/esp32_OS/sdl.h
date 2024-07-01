#include <Arduino.h>
#ifndef sdl_h
#define sdl_h

#define HAUT 1
#define BAS 7
#define DROITE 5
#define GAUCHE 3
#define A 4
#define EXIT 2
#define EGAL 0
#define C 6

class Sdl{
  public:
    Sdl();
    const uint8_t buttons[9] = {5, 15, 4, 16, 18, 13, 17, 14, 12};
    int buttonsState[9];
    bool checkButtons();
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawRect(int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void clearDisplay();
    void display();
    void begin();
    void setRotation(uint8_t x);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c);
    void setCursor(int16_t x, int16_t y);
    void print(String t);
    void print(char c);
};

#endif
