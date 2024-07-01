#include <Arduino.h>

#ifndef snake_h
#define snake_h

class Snake{
  public:
    Snake();
    void begin();
    void loop();
    uint8_t menu();
    void draw_cadre(uint8_t mode);
    bool colision(uint8_t x_snake, uint8_t y_snake);
    void spawn_apple();

    uint8_t sizeXmap;
    uint8_t sizeYmap;
    uint8_t bloc_size;
    uint8_t x_apple;
    uint8_t y_apple;
    bool MAP [16][16]{
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    uint8_t direction;
    uint8_t x_snake;
    uint8_t y_snake;
    uint8_t last_x_snake;
    uint8_t last_y_snake;
    uint8_t snake [16*16*2];
    int size_snake;
    int last_update;
    bool playing;
    bool PAUSE;
    bool bool_BP_pause;
    uint8_t mode;
    int vitesse;

  private:
    
};

#endif
