#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int buttons[] = {5, 15, 4, 16, 18, 13, 17, 14, 12};
// int buttons[] = {17, 15, 5, 14, 18, 16, 12, 13, 4}; old_version
int buttonsState[] = {0,0,0,0,0,0,0,0,0};

void checkButtons(){
  for(int i = 0; i < 9; i++){
    buttonsState[i] = !digitalRead(buttons[i]);
  }
}

const double pi = 3.14;

float Convert(float degree){
    return (degree * (pi / 180));
}

#define NUM_PARTICLES_MAX 50

int MAP[] = 
{0b0000000000000000,
 0b0111111111111110,
 0b0111111111111110,
 0b0111111111111110,
 0b0111111111111110,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000,
 0b0000000000000000
 };

bool droite = 0;
bool gauche = 0;
const int longueur_palette = 20;
int x_palette = 63 - longueur_palette/2;
int last_update_palette = millis();

int num_particules = 1;
int last_update = millis();
const int vitesse = 2;
float angle;
float angle_temp;
float x;
float y;
float x_temp;
float y_temp;
int x_map;
int y_map;

bool colision_block_bool = 0;

float coord[NUM_PARTICLES_MAX*2] = {63,56, 5,5};
float angles[NUM_PARTICLES_MAX] = {Convert(80), Convert(12)};

void drawMap(){
  for(int i = 0; i < 16; i++){
    if(MAP[i] != 0){
      for(int n = 0; n < 16; n++){
        if (bitRead(MAP[i], n)){
          display.fillRect(n*8-1, i*4-1, 7, 3, SSD1306_WHITE);
        }
      }
    }
  }
}

bool colision_palette(){
  if ((y >= 56 and y <=59) and (x >= x_palette-3 and x<= x_palette+longueur_palette)){
    return 1;
  }
  else{
    return 0;
  }
}

bool colision_bloc(int x, int y){
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

void setup(){
  Serial.begin(115200);
  randomSeed(analogRead(26));
  angles[0] = Convert(-random(60,120));
  Serial.print("angle 0 :");
  Serial.println(angles[0]);
  for(int i = 1; i < NUM_PARTICLES_MAX; i++){
    angles[i] = Convert(random(0,359));
  }

  for(int i = 0; i < 9; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  display.clearDisplay();

  drawMap();
  display.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_WHITE);
}

void loop(){
  checkButtons();
  
  if (buttonsState[5] and x_palette+2 <= 127-longueur_palette){
    display.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_BLACK);
    x_palette += 2;
    display.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_WHITE);
  }
  else if (buttonsState[3] and x_palette-2 >= 0){
    display.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_BLACK);
    x_palette -= 2;
    display.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_WHITE);
  }

  display.clearDisplay();
  drawMap();
  display.fillRect(x_palette, 58, longueur_palette, 4, SSD1306_WHITE);

  if(millis() - last_update >= 10){
    last_update = millis();
    for(int i=0; i<num_particules*2; i+=2){
      x = coord[i];
      y = coord[i+1];
      angle = angles[i/2];
      display.fillRect(int(x)-1, int(y)-1, 3, 3, SSD1306_BLACK);

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

      display.fillRect(int(x)-1, int(y)-1, 3, 3, SSD1306_WHITE);
    }
    
    display.display();
  }
  
  /*
  for(int i=0; i<num_particules*2 ; i+=2){
    x = coord[i];
    y = coord[i+1];
    display.fillRect(int(x)-1, int(y)-1, 3, 3, SSD1306_WHITE);
  }
  display.display();
  */
  
  delay(1);
}
