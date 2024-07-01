#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "sdl.h"
#include "snake.h"

Sdl snake_sdl;

Snake::Snake(){
}

uint8_t Snake::menu(){
  bool choosen_taille = 0;
  bool choosen_speed = 0;
  bool update = 1;
  /* 6 tailles :    /  3 vitesses :
  * 8x8             /  - 1
  * 10x10           /  - 2
  * 12x12           /  - 3
  * 14x14           /
  * 16x16           /
  * 8x16            /
  */
  char *tailles[] = {"8x8", "8x16", "10x10", "12x12", "14x14", "16x16"};
  uint8_t choice = 0;
  bool bool_button = 0;

  while (!choosen_taille){
    snake_sdl.checkButtons();

    if (snake_sdl.buttonsState[DROITE] and bool_button){
      if (choice == 5){
        choice = 0;
      }
      else{
        choice++;
      }
      update = 1;
      bool_button = 0;
    }
    else if (snake_sdl.buttonsState[GAUCHE] and bool_button){
      if (choice == 0){
        choice = 5;
      }
      else{
        choice--;
      }
      update = 1;
      bool_button = 0;
    }
    else if (snake_sdl.buttonsState[A] and bool_button){
      choosen_taille = 1;
    }
    else if (!snake_sdl.buttonsState[GAUCHE] and !snake_sdl.buttonsState[DROITE] and !snake_sdl.buttonsState[A]){
      bool_button = 1;
    }

    if (update){
      snake_sdl.clearDisplay();
      snake_sdl.setCursor(46,45);

      if (choice == 1){
        snake_sdl.drawRect(28, 2, 70, 35, SSD1306_WHITE);
      }
      else{
        if (choice != 0){
          snake_sdl.setCursor(36,45);
        }
        snake_sdl.drawRect(int(64-(choice+5)*3/2), 16-choice*2, (choice+5)*3, (choice+5)*3, SSD1306_WHITE);
      }
      
      snake_sdl.print(tailles[choice]);

      snake_sdl.display();
      delay(200);

      update = 0;
    }
  }

  switch(choice){
    case 0:
      sizeYmap = 8;
      sizeXmap = 8;
      break;
    case 1:
      sizeYmap = 8;
      sizeXmap = 16;
      break;
    case 2:
      sizeYmap = 10;
      sizeXmap = 10;
      break;
    case 3:
      sizeYmap = 12;
      sizeXmap = 12;
      break;
    case 4:
      sizeYmap = 14;
      sizeXmap = 14;
      break;
    case 5:
      sizeYmap = 16;
      sizeXmap = 16;
      break;
  }
  bloc_size = int(64/sizeYmap);

  uint8_t choice_vitesse = 0;
  snake_sdl.setTextSize(2);
  update = 1;
  bool bool_A = 0;
  delay(100);
  
  while (!choosen_speed){
    snake_sdl.checkButtons();

    if (snake_sdl.buttonsState[DROITE] and bool_button){
      if (choice_vitesse == 3){
        choice_vitesse = 0;
      }
      else{
        choice_vitesse++;
      }
      update = 1;
      bool_button = 0;
    }
    else if (snake_sdl.buttonsState[GAUCHE] and bool_button){
      if (choice_vitesse == 0){
        choice_vitesse = 3;
      }
      else{
        choice_vitesse--;
      }
      update = 1;
      bool_button = 0;
    }
    else if (!snake_sdl.buttonsState[GAUCHE] and !snake_sdl.buttonsState[DROITE]){
      bool_button = 1;
    }
    if(snake_sdl.buttonsState[A] and bool_A){
      choosen_speed = 1;
    }
    else if (!snake_sdl.buttonsState[A]){
      bool_A = 1;
    }

    if (update){
      snake_sdl.clearDisplay();
      
      snake_sdl.setCursor(15,26);
      snake_sdl.print("speed :");

      snake_sdl.setCursor(105,26);
      snake_sdl.print(String(choice_vitesse+1));

      snake_sdl.display();
      delay(200);

      update = 0;
    }
  }
  
  vitesse = (choice_vitesse+1)*50;
  return choice;
}

void Snake::draw_cadre(uint8_t mode){
  if (mode == 1){
    snake_sdl.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  }
  else{
    snake_sdl.drawRect(0, 0, 64, 64, SSD1306_WHITE);
  }
}

bool Snake::colision(uint8_t x_snake, uint8_t y_snake){
  if (x_snake > sizeXmap or y_snake > sizeYmap){
    // on check pas si x<0 car x est en unsigned <=> x<0 -> x = 255 + x réel
    return 1;
  }
  if (MAP[x_snake][y_snake] == 1){
    // on check une collision avec lui-même
    return 1;
  }
  return 0;
}

void Snake::spawn_apple(){
  do{
    x_apple = random(0, sizeXmap);
    y_apple = random(0, sizeYmap);
  }
  while (MAP[x_apple][y_apple] == 1);
  snake_sdl.fillRect(x_apple*bloc_size, y_apple*bloc_size, bloc_size, bloc_size, SSD1306_WHITE);
}

void Snake::begin(){
  for (int x = 0; x<= 15; x++){
    for (int y = 0; y<= 15; y++){
      MAP[x][y] = 0;
    }
  }

  direction = 0;
  size_snake = 2;
  playing = 1;
  PAUSE = 0;

  Serial.begin(115200);
  randomSeed(analogRead(26));

  snake_sdl.begin();
  snake_sdl.setTextColor(SSD1306_WHITE);
  snake_sdl.setTextSize(2);

  last_update = millis();

  mode = menu();
  snake [0] = {2};
  snake [1] = {sizeYmap/2};
  snake [2] = {1};
  snake [3] = {sizeYmap/2};
  snake [4] = {0};
  snake [5] = {sizeYmap/2};

  snake_sdl.clearDisplay();
  MAP[0][sizeYmap/2] = 1;
  MAP[1][sizeYmap/2] = 1;
  MAP[2][sizeYmap/2] = 1;

  spawn_apple();

  snake_sdl.fillRect(0, (sizeYmap/2)*bloc_size, 3*bloc_size, bloc_size, SSD1306_WHITE);
  if (mode != 1){
    snake_sdl.setCursor(68,10);
    snake_sdl.print("score");
    snake_sdl.setCursor(80,40);
    snake_sdl.print(String(size_snake-1));
  }
  draw_cadre(mode);

  snake_sdl.display();
  
  delay(1000);
}

void Snake::loop(){
  while(playing){
    snake_sdl.checkButtons();
    
    if (snake_sdl.buttonsState[DROITE]){
      direction = 0;
    }
    else if (snake_sdl.buttonsState[HAUT]){
      direction = 1;
    }
    else if (snake_sdl.buttonsState[GAUCHE]){
      direction = 2;
    }
    else if (snake_sdl.buttonsState[BAS]){
      direction = 3;
    }
    else if (snake_sdl.buttonsState[A] and bool_BP_pause){
      bool_BP_pause = 0;
      PAUSE = !PAUSE;
      delay(50);
    }
    else if (!snake_sdl.buttonsState[A]){
      bool_BP_pause = 1;
    }
    if (snake_sdl.buttonsState[EXIT]){
      playing = 0;
    }
  
    if(millis()-last_update >= 400-vitesse and PAUSE == 0){
      last_update = millis();
      x_snake = snake[0];
      y_snake = snake[1];
  
      last_x_snake = snake[size_snake*2];
      last_y_snake = snake[size_snake*2+1];
      snake_sdl.fillRect(last_x_snake*bloc_size, last_y_snake*bloc_size, bloc_size, bloc_size, SSD1306_BLACK);
      // on efface le dernier morceau du serpent
      MAP[last_x_snake][last_y_snake] = 0;
      draw_cadre(mode);
      
      switch(direction){
        case 0:
          x_snake += 1;
          break;
        case 1:
          y_snake -= 1;
          break;
        case 2:
          x_snake -= 1;
          break;
        case 3:
          y_snake += 1;
          break;
      }
  
      if(colision(x_snake, y_snake)){
        // perdu
        playing = 0;
        snake_sdl.clearDisplay();
        snake_sdl.setTextColor(SSD1306_WHITE);
        snake_sdl.setCursor(22,26);
        snake_sdl.print("Perdu !");
        snake_sdl.display();
        delay(1000);
      }

      else{
        // le joueur se balade
        snake_sdl.fillRect(x_snake*bloc_size, y_snake*bloc_size, bloc_size, bloc_size, SSD1306_WHITE);
        MAP[x_snake][y_snake] = 1;
        // on affiche le nouveau pixel de la "tête"

        for (int i = size_snake*2; i>= 2; i -= 2){
          snake[i+1] = snake[i-1];
          snake[i] = snake[i-2];
        }
        // on décale toute la liste de ses morceaux
        
        snake[0] = x_snake;
        snake[1] = y_snake;

        if (x_snake == x_apple and y_snake == y_apple){
          // on mange une pomme
          if (mode != 1){
            snake_sdl.setTextColor(SSD1306_BLACK);
            snake_sdl.setCursor(80,40);
            snake_sdl.print(String(size_snake-1));
          }
          
          size_snake++;

          if (mode != 1){
            snake_sdl.setTextColor(SSD1306_WHITE);
            snake_sdl.setCursor(80,40);
            snake_sdl.print(String(size_snake-1));
          }
          //snake_sdl.fillRect(snake[size_snake*2]*bloc_size, snake[size_snake*2+1]*bloc_size, bloc_size, bloc_size, SSD1306_WHITE);
          // on rallonge la queue
          MAP[snake[size_snake*2]][snake[size_snake*2+1]] = 1;

          spawn_apple();
        }
      }
      snake_sdl.display();
    }
    delay(1);
  }
}
