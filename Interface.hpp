#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include <allegro5/allegro.h> //biblioteca matriz do allegro
#include <allegro5/allegro_color.h> //biblioteca para rotinas de cor
#include <allegro5/allegro_primitives.h> //biblioteca para desenho de formas geométricas
#include <allegro5/allegro_image.h> // biblioteca para importação e desenho de imagens
#include <allegro5/allegro_native_dialog.h> //biblioteca para uso de diálogos nativos
#include <allegro5/allegro_font.h> // biblioteca para escrita/ uso de fontes
#include <allegro5/allegro_ttf.h> // biblioteca para uso de fontes TrueType
#include <iostream> // usada para debugging
#include <math.h>
#include <fstream>
#include <cstring>
#include <iostream>

using namespace std;

#define display_width 700
#define display_height 900
#define roll_bar_width 20

class Interface {
    ALLEGRO_COLOR black;
    ALLEGRO_COLOR grey;
    ALLEGRO_COLOR white;
    ALLEGRO_COLOR menu_color;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_BITMAP *play_button, *pause_button, *stop_button, *save_button, *load_button, *save_as_button, *vrep_button, *bluetooth_button, *mini_menu[6];
    ALLEGRO_BITMAP *action_walk, *action_turn, *sensor_black, *sensor_ultrasonic, *NUMBER[10], *logic_true, *logic_false; //bitmaps para os blocos de travamento (os que travam nos blocos maiores)
    ALLEGRO_BITMAP *DECISION_BLOCK[4], *END_BLOCK[4], *FUNCTION_BLOCK[4], *LOOP_BLOCK[4], *MERGE_BLOCK[4], *START_BLOCK[4], *POINT[2]; //bitmaps para os blocos e suas variações (inativo, destacado pelo mouse, sendo movido e sendo rodado) e para os pontos de junção dos blocos (inativo e destacado)
    ALLEGRO_FONT *font;
    bool executing;

    void load_bitmap(ALLEGRO_BITMAP **bitmap, char *adress);

public:

    Interface();
    ~Interface();
    void start();
};
#endif
