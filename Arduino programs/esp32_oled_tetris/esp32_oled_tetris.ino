#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool bool_bouton2 = 0;
bool bool_bouton4 = 0;
int buttons[] = {5, 15, 4, 16, 18, 13, 17, 14, 12};
int buttonsState[] = {0,0,0,0,0,0,0,0,0};

void checkButtons(){
  for(int i = 0; i < 9; i++){
    buttonsState[i] = !digitalRead(buttons[i]);
  }
}


int lines[] = {0,0,0,0};
int score = 0;
int pieces_posee = 0;
bool PAUSE = 0;
bool END = 0;
int delai = 500;

int x = 4;
int y = 0;

int rotation = 0;
int piece;
int next_piece;

int last_update = millis();

bool pieces[4][7][4][4] = {{
// 0°
  {{1,0,0,0},
   {1,1,0,0},
   {1,0,0,0},
   {0,0,0,0}},

  {{0,1,0,0},
   {1,1,0,0},
   {1,0,0,0},
   {0,0,0,0}},
   
  {{1,0,0,0},
   {1,1,0,0},
   {0,1,0,0},
   {0,0,0,0}},
   
  {{1,0,0,0},
   {1,0,0,0},
   {1,0,0,0},
   {1,0,0,0}},

  {{1,0,0,0},
   {1,0,0,0},
   {1,1,0,0},
   {0,0,0,0}},

  {{0,1,0,0},
   {0,1,0,0},
   {1,1,0,0},
   {0,0,0,0}},

  {{1,1,0,0},
   {1,1,0,0},
   {0,0,0,0},
   {0,0,0,0}}
},
// 90°
{
  {{1,1,1,0},
   {0,1,0,0},
   {0,0,0,0},
   {0,0,0,0}},

  {{1,1,0,0},
   {0,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
   
  {{0,1,1,0},
   {1,1,0,0},
   {0,0,0,0},
   {0,0,0,0}},
   
  {{1,1,1,1},
   {0,0,0,0},
   {0,0,0,0},
   {0,0,0,0}},

  {{1,1,1,0},
   {1,0,0,0},
   {0,0,0,0},
   {0,0,0,0}},

  {{1,0,0,0},
   {1,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},

  {{1,1,0,0},
   {1,1,0,0},
   {0,0,0,0},
   {0,0,0,0}}
},
// 180°
{
  {{0,1,0,0},
   {1,1,0,0},
   {0,1,0,0},
   {0,0,0,0}},

  {{0,1,0,0},
   {1,1,0,0},
   {1,0,0,0},
   {0,0,0,0}},
   
  {{1,0,0,0},
   {1,1,0,0},
   {0,1,0,0},
   {0,0,0,0}},
   
  {{1,0,0,0},
   {1,0,0,0},
   {1,0,0,0},
   {1,0,0,0}},

  {{1,1,0,0},
   {0,1,0,0},
   {0,1,0,0},
   {0,0,0,0}},

  {{1,1,0,0},
   {1,0,0,0},
   {1,0,0,0},
   {0,0,0,0}},

  {{1,1,0,0},
   {1,1,0,0},
   {0,0,0,0},
   {0,0,0,0}}
},
// 270°
{
  {{0,1,0,0},
   {1,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},

  {{1,1,0,0},
   {0,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
   
  {{0,1,1,0},
   {1,1,0,0},
   {0,0,0,0},
   {0,0,0,0}},
   
  {{1,1,1,1},
   {0,0,0,0},
   {0,0,0,0},
   {0,0,0,0}},

  {{0,0,1,0},
   {1,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},

  {{1,1,1,0},
   {0,0,1,0},
   {0,0,0,0},
   {0,0,0,0}},

  {{1,1,0,0},
   {1,1,0,0},
   {0,0,0,0},
   {0,0,0,0}}
}};

bool MAP[21][12] = {
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1},
};

void initialisation(){
  for(int i = 0; i < 9; i++){
      pinMode(buttons[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();

  randomSeed(analogRead(25));
  piece = random(0,7);
  next_piece = random(0,7);
  display.drawLine(28, 0, 28, 63, SSD1306_WHITE);
  display.drawLine(28, 50, 127, 50, SSD1306_WHITE);
  display.display();
  draw_next_piece();
}

bool check_collision_y(){
  for(int i = 0; i<= 3; i++){
    for(int j = 0; j<= 3; j++){
      if (MAP[i+y+1][j+x+1] == 1 and pieces[rotation][piece][i][j] == 1){
        return 1;
      }
    }
  }
  return 0;
}

bool check_collision_x_moins(){
  if (x <= 0){
    return 1;
  }
  else{
    for(int i = 0; i<= 3; i++){
      for(int j = 0; j<= 3; j++){
        if (i+y <= 20 and j+x <= 11){
          if (MAP[i+y][j+x] == 1 and pieces[rotation][piece][i][j] == 1){
          return 1;
          }
        }
      }
    }
  }
  return 0;
}

bool check_collision_x_plus(){
  for(int i = 0; i<= 3; i++){
    for(int j = 0; j<= 3; j++){
      if (i+y <= 20 and j+x <= 11){
        if (MAP[i+y][j+x+2] == 1 and pieces[rotation][piece][i][j] == 1){
          return 1;
        }
      }
    }
  }
  return 0;
}

bool check_line(){
  bool response = 0;
  for(int i = 0; i<= 3; i++){
    int total = 0;
    for(int j = 1; j<= 10; j++){
      if (i+y <= 19){
        if(MAP[i+y][j] == 1){
          total += 1;
        }
      }
    }
    if (total == 10){
      lines[i] = 1;
      response = 1;
    }
    else{
      lines[i] = 0;
    }
  }
  for(int i = 0; i<=3; i++){
  }
  return response;
}

void delete_lines(){
  for(int i = 0; i<= 3; i++){
    if (lines[i] == 1){
      for(int z = y; z>=1; z--){
        for(int j = 1; j<= 10; j++){
          MAP[i+z][j] = MAP[i+z-1][j];
        }
      }
    }
  }
}

void draw_next_piece(){
  display.fillRect(0, 53, 20, 10, SSD1306_BLACK);
  for(int i = 0; i<= 3; i++){
    for(int j = 0; j<= 3; j++){
      if(pieces[0][next_piece][i][j] == 1){
         display.fillRect(15-i*5,53+j*5, 5, 5, SSD1306_WHITE);
      }
    }
  }
}

void handle_rotation(){
  if(rotation == 3){
      rotation = 0;
    }
    else{
      rotation +=1;
    }
    y -=1;
    if(check_collision_y()){
      if(rotation == 0){
        rotation = 3;
      }
      else{
        rotation -= 1;
      }
      y +=1;
      return;
    }
    y +=1;
    if (piece <=3){
      switch (rotation){
        case 1:
          x -= 1;
          break;
        case 3:
          x -= 1;
          break;
        case 0:
          x += 1;
          break;
        case 2:
          x += 1;
          break;
      }
    }
    x-=1;
    if(check_collision_x_plus()){
      cancel_rotation();
      x+=1;
      return;
    }
    x+=2;
    if(check_collision_x_moins()){
      cancel_rotation();
    }
    x-=1;
}

void cancel_rotation(){
  if (piece <=3){
    switch (rotation){
      case 1:
        x += 1;
        break;
      case 3:
        x += 1;
        break;
      case 0:
        x -= 1;
        break;
      case 2:
        x -= 1;
        break;
    }
  }
  if(rotation == 0){
    rotation = 3;
  }
  else{
    rotation -= 1;
  }
}

void RESET_map(){
  for(int i = 0; i<= 19; i++){
    for(int j = 1; j<= 10; j++){
      MAP[i][j] = 0;
    }
  }
}

void setup() {
  initialisation();
}

void loop() {
  
  checkButtons();
  if (buttonsState[2] == 1 and bool_bouton2 == 0){
    bool_bouton2 == 1;
    PAUSE = 0;
  }
  else if (buttonsState[2] == 0){
      bool_bouton2 = 0;
    }

  if (buttonsState[4] == 1){
    display.clearDisplay();
    pieces_posee = 0;
    delai = 500;
    score = 0;
    x = 4;
    y = 0;
    rotation = 0;
    last_update = millis();
    randomSeed(analogRead(25));
    piece = random(0,7);
    next_piece = random(0,7);
    RESET_map();
    display.drawLine(28, 0, 28, 63, SSD1306_WHITE);
    display.drawLine(28, 50, 127, 50, SSD1306_WHITE);
    draw_next_piece();
    END = 0;
  }
  delay(200);

  while (PAUSE == 0 and END == 0){
  
    display.setRotation(1);
    display.setTextSize(2);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(0,110);
    display.print(String(score));
    display.setRotation(0);
  
    for(int i = 0; i<= 19; i++){
      for(int j = 1; j<= 10; j++){
        if (MAP[i][j] == 1){
          display.fillRect(124-i*5, (j-1)*5, 5, 5, SSD1306_WHITE);
        }
        else{
          display.fillRect(124-i*5, (j-1)*5, 5, 5, SSD1306_BLACK);
        }
      }
    }
  
    if (millis() -  last_update >= delai){
      if (check_collision_y() == 0){
        y += 1;
        last_update = millis();
      }
      else{
        for(int i = 0; i<= 3; i++){
          for(int j = 0; j<= 3; j++){
            if (i+y <= 20 and j+x <= 9){
              if (pieces[rotation][piece][i][j] == 1){
                MAP[i+y][j+x+1] = pieces[rotation][piece][i][j];
              }
            }
          }
        }
        if(y <= 1){
          display.clearDisplay();
          display.setRotation(1);
          display.setTextSize(2);
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,50);
          display.print("end");
          display.setRotation(0);
          display.display();
          END = 1;
        }
        if (END == 0){
          if (check_line() == 1){
            delete_lines();
          }
          pieces_posee += 1;
          if (pieces_posee == 20 and delai > 50){
            pieces_posee = 0;
            delai -= 50;
          }
          score += 1;
          x = 4;
          y = 0;
          rotation = 0;
          piece = next_piece;
          next_piece = random(0,7);
          draw_next_piece();
        }
      }
    }
    
    checkButtons();
    if (buttonsState[7] == 1 and check_collision_x_plus() == 0){
      x += 1;
    }
    else if (buttonsState[1] == 1 and check_collision_x_moins() == 0){
      x -= 1;
    }
    else if (buttonsState[3] == 1 and check_collision_y() == 0){
      y += 1;
    }
    else if (buttonsState[4] == 1 and bool_bouton4 == 0){
      bool_bouton4 = 1;
      handle_rotation();
    }
    if (buttonsState[4] == 0){
      bool_bouton4 = 0;
    }
    if (buttonsState[2] == 1 and bool_bouton2 == 0){
      bool_bouton2 = 1;
      PAUSE = 1;
    }
    else if (buttonsState[2] == 0){
      bool_bouton2 = 0;
    }
  
    display.setRotation(1);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,110);
    display.print(String(score));
    display.setRotation(0);

    if (END == 0){
      for(int i = 0; i<= 3; i++){
        for(int j = 0; j<= 3; j++){
          if(pieces[rotation][piece][i][j] == 1){
            display.fillRect(124-(i+y)*5, (j+x)*5, 5, 5, SSD1306_WHITE);
          }
        }
      } 
    }
    display.display();
    delay(100);
  }
}
