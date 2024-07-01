#include <Adafruit_SSD1306.h>
 
#include "sdl.h"
#include "icons.h"
#include "tetris.h"
#include "snake.h"
#include "brick.h"
#include "calculator.h"

Tetris tetris;
Snake snake;
Brick brick;
Calculator calculator;

Sdl sdl;

int x_rect = 3;
int y_rect = 13;
int size_triangle = 8;
int size_rect = 21;
int x_rect2 = 100;
bool bool_bouton = 1;
int choix = 0;

void petitRect(int x, int y, int size_rect){
  sdl.fillRoundRect(x, y, size_rect, size_rect, 4, SSD1306_WHITE);
  sdl.fillRoundRect(x+2, y+2, size_rect-4, size_rect-4, 4, SSD1306_BLACK);
  sdl.fillCircle(int((size_rect)/2+x), int((size_rect)/2+y), int(size_rect/4), SSD1306_WHITE);
}

void loadChoix(bool gauche){
  int x_rect = 3;
  int y_rect = 12;
  int size_rect = 21;
  int x_rect2 = 100;
  int x_rect_milieu = 41;
  int y_rect_milieu = 1;
  int size_rect_milieu = 42;

  sdl.clearDisplay();
  petitRect(x_rect_milieu, y_rect_milieu, size_rect_milieu);
  sdl.display();

  if (gauche){
    for(int i=1; i<=19; i+=2){
      sdl.fillRect(x_rect+2*i-5,y_rect-(i-3)/2,size_rect+i-2,size_rect+i-1, SSD1306_BLACK); // efface rectangle gauche
      sdl.fillRect(x_rect_milieu+3*i-6,y_rect_milieu+(i-2)/2,size_rect_milieu-i+4,size_rect_milieu-i+3, SSD1306_BLACK); // efface rectangle milieu
      sdl.fillRect(x_rect2+1,y_rect,size_rect-i+1,size_rect-i/2+2, SSD1306_BLACK); // efface rectangle droite
      
      petitRect(x_rect, y_rect+(size_rect-i)/2-2, i+3); // nouveau rectangle à gauche
      petitRect(x_rect+2*i, y_rect-i/2, size_rect+i); // rectangle à gauche
      petitRect(x_rect_milieu+3*i, y_rect_milieu+i/2+3, size_rect_milieu-i-3); // rectangle du milieu
      if(i != 19){
          petitRect(x_rect2, y_rect+i/2, size_rect-i); // rectangle à droite
      }
      sdl.display();
    }
  }
  else{
    for(int i=1; i<=19; i+=2){
      sdl.fillRect(x_rect+i-2,y_rect+(i-2)/2,size_rect-i+2,size_rect-i+2, SSD1306_BLACK); // efface rectangle gauche
      sdl.fillRect(x_rect_milieu-2*i+4,y_rect_milieu+(i-1)/2,size_rect_milieu-i+4,size_rect_milieu-i+2, SSD1306_BLACK); // efface rectangle milieu
      sdl.fillRect(x_rect2-3*i+4,y_rect-(i+2)/2,size_rect+i+2,size_rect+i+2, SSD1306_BLACK); // efface rectangle droite
      
      petitRect(x_rect2, y_rect+(size_rect-i)/2-2, i+3); // nouveau rectangle à droite
      petitRect(x_rect2-3*i, y_rect-i/2, size_rect+i); // rectangle à droite
      if (i != 19){
          petitRect(x_rect+i, y_rect+i/2, size_rect-i); // rectangle à gauche
      }
      petitRect(x_rect_milieu-2*i, y_rect_milieu+i/2+3, size_rect_milieu-i-3); // rectangle du milieu
      sdl.display();
    }
  }
  sdl.fillRoundRect(43, 3, 38, 39, 4,BLACK);
  sdl.drawBitmap(41, 2, icons[choix], 42, 42, WHITE);
  sdl.display();
}

void initialisation(){
  for(int i = 0; i < 9; i++){
      pinMode(sdl.buttons[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
  sdl.begin();
  sdl.clearDisplay();

  petitRect(x_rect, y_rect, size_rect);
  petitRect(x_rect2, y_rect, size_rect);
  sdl.drawBitmap(41, 2, icons[choix], 42, 42, WHITE);
  sdl.display();
}

void setup() {
  initialisation();
}

void loop() {
  bool input;
  input = sdl.checkButtons();

  if(sdl.buttonsState[GAUCHE] and bool_bouton == 0){
    bool_bouton = 1;
    if (choix <= 2){
        choix += 1;
    }
    else{
        choix = 0;
    }
    loadChoix(1);
  }

  else if(sdl.buttonsState[DROITE] and bool_bouton == 0){
    bool_bouton = 1;
    if (choix >= 1){
        choix -= 1;
    }
    else{
        choix = 3;
    }
    loadChoix(0);
    
  }

  else if(sdl.buttonsState[A] and bool_bouton == 0){
    bool_bouton = 1;
    sdl.clearDisplay();
    sdl.display();

    switch(choix){
      case 0:
        tetris.begin();
        tetris.loop();
        break;

      case 1:
        snake.begin();
        snake.loop();
        break;

      case 2:
        brick.begin();
        brick.loop();
        break;

      case 3:
        calculator.begin();
        calculator.loop();
        break;
    }
    sdl.clearDisplay();
    petitRect(x_rect, y_rect, size_rect);
    petitRect(x_rect2, y_rect, size_rect);
    sdl.drawBitmap(41, 2, icons[choix], 42, 42, WHITE);
    sdl.display();
  }

  else if(!input){
    bool_bouton = 0;
  }
  
  
}
