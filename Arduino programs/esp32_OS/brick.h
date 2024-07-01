#include <Arduino.h>

#ifndef brick_h
#define brick_h

#define NUM_PARTICLES_MAX 50

class Brick{
  public:
    Brick();
    void begin();
    void loop();
    float Convert(float degree);
    void drawMap();
    bool colision_palette();
    bool colision_bloc(int x, int y);
    
    const double pi = 3.14;
    int MAP[16] = 
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
    bool droite;
    bool gauche;
    const int longueur_palette = 20;
    int x_palette;
    int last_update_palette;
    int num_particules;
    int last_update;
    const int vitesse = 2;
    float angle;
    float angle_temp;
    float x;
    float y;
    float x_temp;
    float y_temp;
    int x_map;
    int y_map;
    bool colision_block_bool;
    float coord[NUM_PARTICLES_MAX*2] = {63,56, 5,5};
    float angles[NUM_PARTICLES_MAX];

  private:
    
};

#endif
