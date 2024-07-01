#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "sdl.h"
#include "calculator.h"

Sdl calculator_sdl;

Calculator::Calculator(){
}

void Calculator::erase(int x, int y){
  calculator_sdl.setTextSize(1);
  calculator_sdl.fillRect(16*x, 16*y+16, 16, 16, SSD1306_BLACK);
  calculator_sdl.setCursor(x*16+5,y*16+21);
  calculator_sdl.setTextColor(SSD1306_WHITE);
  calculator_sdl.print(ui[y][x]);
}

void Calculator::draw(int x, int y){
  calculator_sdl.setTextSize(1);
  calculator_sdl.fillRect(16*x, 16*y+16, 16, 16, SSD1306_WHITE);
  calculator_sdl.setCursor(x*16+5,y*16+21);
  calculator_sdl.setTextColor(SSD1306_BLACK);
  calculator_sdl.print(ui[y][x]);
}

// Reverses a string 'str' of length 'len' 
void Calculator::reverse(char* str, int len){ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
 
// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning. 
int Calculator::intToStr(int x, char str[], int d){ 
    int i = 0; 

    if (x == 0){
      str[0] = '0';
      str[1] = '\0';
    }

    else{
      while (x) { 
          str[i++] = (x % 10) + '0'; 
          x = x / 10; 
      }
  
      // If number of digits required is more, then 
      // add 0s at the beginning 
      while (i < d) 
          str[i++] = '0'; 

      reverse(str, i); 
      str[i] = '\0'; 
    }

    return i; 
} 
 
// Converts a floating-point/double number to a string. 
int Calculator::ftoa(float n, char* res){ 
    // Extract integer part 
    int ipart = (int)n; 
 
    // Extract floating part 
    float fpart = n - (float)ipart; 
 
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 

    int nb_int = i;

    // check if float part is needed
    i = 0; 
    int float_part = 0;
    int x = int(fpart * pow(10, MAX-nb_int-1));

    while (x) { 
        float_part += x % 10;
        x = x / 10; 
    }

    // check for display option after point 
    if (float_part) { 
      res[nb_int] = '.'; // add dot 

      // Get the value of fraction part upto given no. 
      // of points after dot. The third parameter 
      // is needed to handle cases like 233.007 
      fpart = fpart * pow(10, MAX-nb_int-1); 

      intToStr((int)fpart, res + nb_int + 1, MAX-nb_int-1);
    }

    nb_int += i;
    return nb_int;
} 

void Calculator::display_char(char c){
  calculator_sdl.drawLine(digit*12, 0, digit*12, 15, SSD1306_BLACK);
  calculator_sdl.setTextSize(2);
  calculator_sdl.setCursor(digit*12,0);
  calculator_sdl.setTextColor(SSD1306_WHITE);
  calculator_sdl.print(c);
  calculator_sdl.display();
}

void Calculator::handle_keys(int x, int y){
  int pos = x + y*6;
  int chiffre = -1;
  bool operation = 0;

  switch(pos){
    case 0:
      // v
      if (displaying_result)
        result = sqrt(result);
      else
        result = sqrt(number1);
      calculator_sdl.fillRect(0, 0, 128, 16, SSD1306_BLACK);
      displaying_result = 1;
      virgule = 0;
      digit = 0;
      
      displ_nbr(result);
      break;

    case 1:
      // 7
      chiffre = 7;
      break;

    case 2:
      // 8
      chiffre = 8;
      break;

    case 3:
      // 9
      chiffre = 9;
      break;

    case 4:
      // /
      operand = 0;
      operation = 1;
      break;

    case 5:
      // *
      operand = 1;
      operation = 1;
      break;

    case 6:
      // <-

      break;

    case 7:
      // 4
      chiffre = 4;
      break;

    case 8:
      // 5
      chiffre = 5;
      break;

    case 9:
      // 6
      chiffre = 6;
      break;

    case 10:
      // 0
      chiffre = 0;
      break;

    case 11:
      // +
      operand = 2;
      operation = 1;
      break;

    case 12:
      // )

      break;

    case 13:
      // 1
      chiffre = 1;
      break;

    case 14:
      // 2
      chiffre = 2;
      break;

    case 15:
      // 3
      chiffre = 3;
      break;

    case 16:
      // ,
      if(!virgule){
        virgule = 1;
        float_power = 1;
        display_char(',');
        digit++;
      }
      break;
    
    case 17:
      // -
      operand = 3;
      operation = 1;
      break;
  }

  if (chiffre != -1 and digit <= 9){
    if (displaying_result){
      calculator_sdl.fillRect(0, 0, 128, 16, SSD1306_BLACK);
      displaying_result = 0;
    }
    
    if (!virgule){
      number1 = number1*10;
      number1 += chiffre;
    }

    else{
      number1 += chiffre*pow(0.1, float_power);
      float_power++;
    }

    display_char(chiffre+'0');
    digit++;
    calculator_sdl.drawLine(digit*12, 0, digit*12, 15, SSD1306_WHITE);
  }

  if(operation){
    if(displaying_result == 1)
      number2 = result;
    else
      number2 = number1;
    number1 = 0;
    digit = 0;
    calculator_sdl.fillRect(0, 0, 128, 16, SSD1306_BLACK);
    displaying_result = 0;
    virgule = 0;
    calculator_sdl.drawLine(digit*12, 0, digit*12, 15, SSD1306_WHITE);
  }
}

void Calculator::displ_nbr(float nombre){
  char buf[MAX+1];
  ftoa(nombre, buf);
  calculator_sdl.setTextSize(2);
  calculator_sdl.fillRect(0, 0, 128, 16, SSD1306_BLACK);
  calculator_sdl.setCursor(0,0);
  calculator_sdl.setTextColor(SSD1306_WHITE);
  calculator_sdl.print(buf);
  calculator_sdl.display();
}

void Calculator::begin(){
  calculator_sdl.checkButtons();
  bool_bouton = 0;
  x = 1;
  y = 1;
  number1 = 0;
  number2 = 0;
  digit = 0;
  virgule = 0;
  operand = 1;
  black = 0;
  displaying_result = 0;

  Serial.begin(115200);
  lastime = millis();
  calculator_sdl.begin();

  calculator_sdl.clearDisplay();
  calculator_sdl.setTextColor(SSD1306_WHITE);
  calculator_sdl.setTextSize(1);

  for(int x = 0; x <= 5; x++){
    for(int y = 0; y <= 2; y++){
    calculator_sdl.setCursor(x*16+5,y*16+21);
    calculator_sdl.print(ui[y][x]);
    }
  }

  calculator_sdl.fillRect(16*x, 16*y+16, 16, 16, SSD1306_WHITE);
  calculator_sdl.setCursor(x*16+5,y*16+21);
  calculator_sdl.setTextColor(SSD1306_BLACK);
  calculator_sdl.print(ui[y][x]);

  calculator_sdl.drawLine(digit*12, 0, digit*12, 15, SSD1306_WHITE);
}

void Calculator::loop(){
  while(calculator_sdl.buttonsState[EXIT] == 0){
    if(millis()-lastime >= 400 and !displaying_result){
      lastime = millis();
      if(black){
        black = 0;
        calculator_sdl.drawLine(digit*12, 0, digit*12, 15, SSD1306_BLACK);
      }
      else{
        black = 1;
        calculator_sdl.drawLine(digit*12, 0, digit*12, 15, SSD1306_WHITE);
      }
      
    }

    calculator_sdl.checkButtons();
    if(bool_bouton){
      bool_bouton = 0;

      if(calculator_sdl.buttonsState[HAUT]){
        erase(x, y);
        if(y >= 1){
          y -= 1;
        }
        else{
          y = 2; 
        }
        draw(x, y);
      }

      if(calculator_sdl.buttonsState[BAS]){
        erase(x, y);
        if(y <= 1){
          y += 1;
        }
        else{
          y = 0; 
        }
        draw(x, y);
      }

      if(calculator_sdl.buttonsState[GAUCHE]){
        erase(x, y);
        if(x >= 1){
          x -= 1;
        }
        else{
          x = 5; 
        }
        draw(x, y);
      }

      if(calculator_sdl.buttonsState[DROITE]){
        erase(x, y);
        if(x <= 4){
          x += 1;
        }
        else{
          x = 0; 
        }
        draw(x, y);
      }

      if(calculator_sdl.buttonsState[A]){
        handle_keys(x,y);
      }

      if(calculator_sdl.buttonsState[EGAL]){
        switch(operand){
          case 0:
            // /
            if (number1 == 0){
              result = 0;
            }
            else{
              result = number2/number1;
            }
            break;

          case 1:
            // *
            result = number1*number2;
            break;

          case 2:
            // +
            result = number1+number2;
            break;

          case 3:
            // -
            result = number2-number1;
            break;
        }
        displ_nbr(result);
        number1 = 0;
        number2 = 0;
        digit = 0;
        virgule = 0;
        displaying_result = 1;
      }

      if(calculator_sdl.buttonsState[C]){
        displaying_result = 0;
        number1 = 0;
        number2 = 0;
        digit = 0;
        virgule = 0;
        calculator_sdl.fillRect(0, 0, 128, 16, SSD1306_BLACK);
      }

      calculator_sdl.display();
    }

    if(!calculator_sdl.buttonsState[HAUT] and !calculator_sdl.buttonsState[BAS] and !calculator_sdl.buttonsState[GAUCHE] and !calculator_sdl.buttonsState[DROITE] and !calculator_sdl.buttonsState[A] and !calculator_sdl.buttonsState[EGAL]){
      bool_bouton = 1;
    }
    
    delay(20);
  }
}