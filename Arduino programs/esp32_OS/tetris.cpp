#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "sdl.h"
#include "tetris.h"

Sdl tetris_sdl;

const bool pieces[4][7][4][4] = {{
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

Tetris::Tetris(){
  
}

bool Tetris::check_collision_y(){
  for(int i = 0; i<= 3; i++){
    for(int j = 0; j<= 3; j++){
      if (MAP[i+y+1][j+x+1] == 1 and pieces[rotation][piece][i][j] == 1){
        return 1;
      }
    }
  }
  return 0;
}

bool Tetris::check_collision_x_moins(){
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

bool Tetris::check_collision_x_plus(){
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

bool Tetris::check_line(){
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

void Tetris::delete_lines(){
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

void Tetris::draw_next_piece(){
  tetris_sdl.fillRect(0, 53, 20, 10, SSD1306_BLACK);
  for(int i = 0; i<= 3; i++){
    for(int j = 0; j<= 3; j++){
      if(pieces[0][next_piece][i][j] == 1){
         tetris_sdl.fillRect(15-i*5,53+j*5, 5, 5, SSD1306_WHITE);
      }
    }
  }
}

void Tetris::cancel_rotation(){
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

void Tetris::handle_rotation(){
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

void Tetris::RESET_map(){
  for(int i = 0; i<= 19; i++){
    for(int j = 1; j<= 10; j++){
      MAP[i][j] = 0;
    }
  }
}

void Tetris::begin(){
  last_update = millis();

  bool_bouton2 = 0;
  bool_bouton4 = 0;
  
  score = 0;
  pieces_posee = 0;
  PAUSE = 0;
  END = 0;
  delai = 500;
  
  x = 4;
  y = 0;
  
  rotation = 0;
  
  tetris_sdl.begin();
  tetris_sdl.clearDisplay();
  randomSeed(analogRead(25));
  piece = random(0,7);
  next_piece = random(0,7);
  tetris_sdl.drawLine(28, 0, 28, 63, SSD1306_WHITE);
  tetris_sdl.drawLine(28, 50, 127, 50, SSD1306_WHITE);
  draw_next_piece();
}

void Tetris::loop(){
  
  tetris_sdl.checkButtons();
  if (tetris_sdl.buttonsState[2] == 1 and bool_bouton2 == 0){
    bool_bouton2 == 1;
    PAUSE = 0;
  }
  else if (tetris_sdl.buttonsState[2] == 0){
      bool_bouton2 = 0;
    }

  if (tetris_sdl.buttonsState[4] == 1){
    tetris_sdl.clearDisplay();
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
    tetris_sdl.drawLine(28, 0, 28, 63, SSD1306_WHITE);
    tetris_sdl.drawLine(28, 50, 127, 50, SSD1306_WHITE);
    draw_next_piece();
    END = 0;
  }
  delay(200);

  while (PAUSE == 0 and END == 0){
    tetris_sdl.setRotation(1);
    tetris_sdl.setTextSize(2);
    tetris_sdl.setTextColor(SSD1306_BLACK);
    tetris_sdl.setCursor(0,110);
    tetris_sdl.print(String(score));
    tetris_sdl.setRotation(0);
  
    for(int i = 0; i<= 19; i++){
      for(int j = 1; j<= 10; j++){
        if (MAP[i][j] == 1){
          tetris_sdl.fillRect(124-i*5, (j-1)*5, 5, 5, SSD1306_WHITE);
        }
        else{
          tetris_sdl.fillRect(124-i*5, (j-1)*5, 5, 5, SSD1306_BLACK);
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
          tetris_sdl.clearDisplay();
          tetris_sdl.setRotation(1);
          tetris_sdl.setTextSize(2);
          tetris_sdl.setTextColor(SSD1306_WHITE);
          tetris_sdl.setCursor(0,50);
          tetris_sdl.print("end");
          tetris_sdl.setRotation(0);
          tetris_sdl.display();
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
    
    tetris_sdl.checkButtons();
    if (tetris_sdl.buttonsState[7] == 1 and check_collision_x_plus() == 0){
      x += 1;
    }
    else if (tetris_sdl.buttonsState[1] == 1 and check_collision_x_moins() == 0){
      x -= 1;
    }
    else if (tetris_sdl.buttonsState[3] == 1 and check_collision_y() == 0){
      y += 1;
    }
    else if (tetris_sdl.buttonsState[4] == 1 and bool_bouton4 == 0){
      bool_bouton4 = 1;
      handle_rotation();
    }
    if (tetris_sdl.buttonsState[4] == 0){
      bool_bouton4 = 0;
    }
    if (tetris_sdl.buttonsState[2] == 1 and bool_bouton2 == 0){
      bool_bouton2 = 1;
      PAUSE = 1;
    }
    else if (tetris_sdl.buttonsState[2] == 0){
      bool_bouton2 = 0;
    }
  
    tetris_sdl.setRotation(1);
    tetris_sdl.setTextSize(2);
    tetris_sdl.setTextColor(SSD1306_WHITE);
    tetris_sdl.setCursor(0,110);
    tetris_sdl.print(String(score));
    tetris_sdl.setRotation(0);

    if (END == 0){
      for(int i = 0; i<= 3; i++){
        for(int j = 0; j<= 3; j++){
          if(pieces[rotation][piece][i][j] == 1){
            tetris_sdl.fillRect(124-(i+y)*5, (j+x)*5, 5, 5, SSD1306_WHITE);
          }
        }
      } 
    }
    tetris_sdl.display();
    delay(100);
  }
}
