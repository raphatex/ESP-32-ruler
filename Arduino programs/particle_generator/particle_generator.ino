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

double Convert(double degree){
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}

#define NUM_PARTICLES 50

int last_update = millis();
int vitesse = 2;
float x = 2;
float y = 2;
float angle = Convert(80);

float coord[NUM_PARTICLES*2] = {2,2, 5,5};
float angles[NUM_PARTICLES] = {Convert(80), Convert(12)};

void setup(){
  randomSeed(analogRead(26));
  for(int i = 0; i < NUM_PARTICLES*2; i+=2){
    coord[i] = random(2,126);
    coord[i+1] = random(2,62);
    angles[i/2] = Convert(random(0,359));
  }

  for(int i = 0; i < 9; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
}

void loop(){

  if(millis() - last_update > 10){
    for(int i=0; i<NUM_PARTICLES*2; i+=2){
      x = coord[i];
      y = coord[i+1];
      angle = angles[i/2];
      display.fillRect(int(x)-1, int(y)-1, 3, 3, SSD1306_BLACK);
      last_update = millis();
  
      if(y >= 62 or y <= 1){
        angle = -angle;
       }
       else if(x >= 126 or x <= 1){
        angle = Convert(180)-angle;
       }
      angles[i/2] = angle;
  
      x += vitesse*cos(angle);
      y += vitesse*sin(angle);
      
      coord[i] = x;
      coord[i+1] = y;
    }
  }
  for(int i=0; i<NUM_PARTICLES*2 ; i+=2){
    x = coord[i];
    y = coord[i+1];
    display.fillRect(int(x)-1, int(y)-1, 3, 3, SSD1306_WHITE);
  }
  display.display();
  delay(10);
}
