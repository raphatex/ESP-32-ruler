#include <Arduino.h>

#ifndef calculator_h
#define calculator_h

#define MAX 10

class Calculator{
  public:
    Calculator();
    void erase(int x, int y);
    void draw(int x, int y);
    void reverse(char* str, int len);
    int intToStr(int x, char str[], int d);
    int ftoa(float n, char* res);
    void display_char(char c);
    void handle_keys(int x, int y);
    void displ_nbr(float nombre);
    void begin();
    void loop();

    bool bool_bouton;
    int x;
    int y;
    float number1;
    float number2;
    float result;
    int digit;
    bool virgule;
    int float_power;
    unsigned long lastime;
    int operand;
    bool black;
    bool displaying_result;
    const char* ui[3][6] = {
      {"V","7","8","9","/","*"},
      {"<","4","5","6","0","+"},
      {")","1","2","3",",","-"}
    };
  private:
    
};

#endif
