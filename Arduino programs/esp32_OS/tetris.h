#include <Arduino.h>

#ifndef tetris_h
#define tetris_h

class Tetris{
  public:
    Tetris();
    int last_update;
    bool bool_bouton2;
    bool bool_bouton4;
    int score;
    int pieces_posee;
    bool PAUSE;
    bool END;
    int delai;
    int x;
    int y;
    int rotation;
    int piece;
    int next_piece;
    int lines[5] = {0,0,0,0};
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
    
    bool check_collision_y();
    bool check_collision_x_moins();
    bool check_collision_x_plus();
    bool check_line();
    void delete_lines();
    void draw_next_piece();
    void cancel_rotation();
    void handle_rotation();
    void RESET_map();
    void begin();
    void loop();

  private:
    
};

#endif
