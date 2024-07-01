// https://www.geeksforgeeks.org/convert-floating-point-number-string/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define HAUT 1
#define BAS 7
#define DROITE 5
#define GAUCHE 3
#define A 4
#define EXIT 2
#define EGAL 0
#define C 6

int buttons[] = {5, 15, 4, 16, 18, 13, 17, 14, 12};
// int buttons[] = {17, 15, 5, 14, 18, 16, 12, 13, 4}; old_version
int buttonsState[] = {0,0,0,0,0,0,0,0,0};

void checkButtons(){
  for(int i = 0; i < 9; i++){
    buttonsState[i] = !digitalRead(buttons[i]);
  }
}

#define MAX 10
bool bool_bouton = 1;

int x = 1;
int y = 1;

float number1 = 0;
float number2 = 0;
float result;
int digit = 0;
bool virgule = 0;
int float_power;

unsigned long lastime;
int operand = 1;
bool black = 0;
bool displaying_result = 0;

const char* ui[3][6] = {
  {"V","7","8","9","/","*"},
  {"<","4","5","6","0","+"},
  {")","1","2","3",",","-"}
};

void erase(int x, int y){
  display.setTextSize(1);
  display.fillRect(16*x, 16*y+16, 16, 16, SSD1306_BLACK);
  display.setCursor(x*16+5,y*16+21);
  display.setTextColor(SSD1306_WHITE);
  display.print(ui[y][x]);
}

void draw(int x, int y){
  display.setTextSize(1);
  display.fillRect(16*x, 16*y+16, 16, 16, SSD1306_WHITE);
  display.setCursor(x*16+5,y*16+21);
  display.setTextColor(SSD1306_BLACK);
  display.print(ui[y][x]);
}

// Reverses a string 'str' of length 'len' 
void reverse(char* str, int len){ 
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
int intToStr(int x, char str[], int d){ 
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
int ftoa(float n, char* res){ 
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

void display_char(char c){
  display.drawLine(digit*12, 0, digit*12, 15, SSD1306_BLACK);
  display.setTextSize(2);
  display.setCursor(digit*12,0);
  display.setTextColor(SSD1306_WHITE);
  display.print(c);
  display.display();
}

void handle_keys(int x, int y){
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
      display.fillRect(0, 0, 128, 16, SSD1306_BLACK);
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
      display.fillRect(0, 0, 128, 16, SSD1306_BLACK);
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
    display.drawLine(digit*12, 0, digit*12, 15, SSD1306_WHITE);
  }

  if(operation){
    if(displaying_result == 1)
      number2 = result;
    else
      number2 = number1;
    number1 = 0;
    digit = 0;
    display.fillRect(0, 0, 128, 16, SSD1306_BLACK);
    displaying_result = 0;
    virgule = 0;
    display.drawLine(digit*12, 0, digit*12, 15, SSD1306_WHITE);
  }
}

void displ_nbr(float nombre){
  char buf[MAX+1];
  ftoa(nombre, buf);
  display.setTextSize(2);
  display.fillRect(0, 0, 128, 16, SSD1306_BLACK);
  display.setCursor(0,0);
  display.setTextColor(SSD1306_WHITE);
  display.print(buf);
  display.display();
}

void setup(){
  Serial.begin(115200);
  lastime = millis();

  for(int i = 0; i < 9; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  for(int x = 0; x <= 5; x++){
    for(int y = 0; y <= 2; y++){
    display.setCursor(x*16+5,y*16+21);
    display.print(ui[y][x]);
    }
  }

  display.fillRect(16*x, 16*y+16, 16, 16, SSD1306_WHITE);
  display.setCursor(x*16+5,y*16+21);
  display.setTextColor(SSD1306_BLACK);
  display.print(ui[y][x]);

  display.drawLine(digit*12, 0, digit*12, 15, SSD1306_WHITE);
}

void loop(){
  if(millis()-lastime >= 400 and !displaying_result){
    lastime = millis();
    if(black){
      black = 0;
      display.drawLine(digit*12, 0, digit*12, 15, SSD1306_BLACK);
    }
    else{
      black = 1;
      display.drawLine(digit*12, 0, digit*12, 15, SSD1306_WHITE);
    }
    
  }

  checkButtons();
  if(bool_bouton){
    bool_bouton = 0;

    if(buttonsState[HAUT]){
      erase(x, y);
      if(y >= 1){
        y -= 1;
      }
      else{
        y = 2; 
      }
      draw(x, y);
    }

    if(buttonsState[BAS]){
      erase(x, y);
      if(y <= 1){
        y += 1;
      }
      else{
        y = 0; 
      }
      draw(x, y);
    }

    if(buttonsState[GAUCHE]){
      erase(x, y);
      if(x >= 1){
        x -= 1;
      }
      else{
        x = 5; 
      }
      draw(x, y);
    }

    if(buttonsState[DROITE]){
      erase(x, y);
      if(x <= 4){
        x += 1;
      }
      else{
        x = 0; 
      }
      draw(x, y);
    }

    if(buttonsState[A]){
      handle_keys(x,y);
    }

    if(buttonsState[EGAL]){
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

    if(buttonsState[C]){
      displaying_result = 0;
      number1 = 0;
      number2 = 0;
      digit = 0;
      virgule = 0;
      display.fillRect(0, 0, 128, 16, SSD1306_BLACK);
    }

    display.display();
  }

  if(!buttonsState[HAUT] and !buttonsState[BAS] and !buttonsState[GAUCHE] and !buttonsState[DROITE] and !buttonsState[A] and !buttonsState[EGAL]){
    bool_bouton = 1;
  }
  
  delay(20);
}
