#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "sdl.h"
#include "brick.h"

Sdl brick_sdl;

Brick::Brick(){
}

const double pi = 3.14;

float Brick::Convert(float degree){
    return (degree * (pi / 180));
}

#define NUM_PARTICLES_MAX 50

void Brick::drawMap(){
  for(int i = 0; i < 16; i++){
    if(MAP[i] != 0){
      for(int n = 0; n < 16; n++){
        if (bitRead(MAP[i], n)){
          brick_sdl.fillRect(n*8-1, i*4-1, 7, 3, SSD1306_WHITE);
        }
      }
    }
  }
}

bool Brick::colision_palette(){
  if ((y >= 56 and y <=59) and (x >= x_palette-3 and x<= x_palette+longueur_palette)){
    return 1;
  }
  else{
    return 0;
  }
}

bool Brick::colision_bloc(int x, int y){
  for(int i = y; i < y+2; i++){
    if(MAP[i] != 0){
      for(int n = 0; n < 2; n++){
        if (bitRead(MAP[i], n+x)){
          bitWrite(MAP[i], n+x, 0);
          return 1;
        }
      }
    }
  }
  return 0;
}

void Brick::begin(){
  droite = 0;
  gauche = 0;
  x_palette = 63 - longueur_palette/2;
  last_update_palette = millis();
  num_particules = 1;
  last_update = millis();
  colision_block_bool = 0;

  randomSeed(analogRead(26));
  angles[0] = Convert(-random(60,120));
  for(int i = 1; i < NUM_PARTICLES_MAX; i++){
    angles[i] = Convert(random(0,359));
  }

  brick_sdl.begin();
  brick_sdl.clearDisplay();

  drawMap();
  brick_sdl.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_WHITE);
}

void Brick::loop(){
  while(brick_sdl.buttonsState[EXIT] == 0){
    brick_sdl.checkButtons();
    
    if (brick_sdl.buttonsState[5] and x_palette+2 <= 127-longueur_palette){
      brick_sdl.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_BLACK);
      x_palette += 2;
      brick_sdl.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_WHITE);
    }
    else if (brick_sdl.buttonsState[3] and x_palette-2 >= 0){
      brick_sdl.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_BLACK);
      x_palette -= 2;
      brick_sdl.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_WHITE);
    }

    brick_sdl.clearDisplay();
    drawMap();
    brick_sdl.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_WHITE);

    if(millis() - last_update >= 10){
      last_update = millis();
      for(int i=0; i<num_particules*2; i+=2){
        x = coord[i];
        y = coord[i+1];
        angle = angles[i/2];
        brick_sdl.fillRect(int(x)-1, int(y)-1, 3, 3, SSD1306_BLACK);

        x += vitesse*cos(angle);
        y += vitesse*sin(angle);

        x_map = int(int(x)/8);
        y_map = int(int(y)/4);

        colision_block_bool = colision_bloc(x_map, y_map);
    
        if(y >= 62 or y <= 1 or colision_palette() or (colision_block_bool and angle >= Convert(180))){
          angle = -angle+Convert(random(-20,20));
          Serial.println(angle);
        }
        else if(x >= 126 or x <= 1 or (colision_block_bool and angle <= Convert(180))){
          angle = Convert(random(160,200))-angle;
        }

        angles[i/2] = angle;
        coord[i] = x;
        coord[i+1] = y;

        brick_sdl.fillRect(int(x)-1, int(y)-1, 3, 3, SSD1306_WHITE);
      }
      
      brick_sdl.display();
    }
    
    /*
    for(int i=0; i<num_particules*2 ; i+=2){
      x = coord[i];
      y = coord[i+1];
      brick_sdl.fillRect(int(x)-1, int(y)-1, 3, 3, SSD1306_WHITE);
    }
    brick_sdl.display();
    */
    
    delay(1);
  }
}
