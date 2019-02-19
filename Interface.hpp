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
#include "Block.hpp"
#include "StartBlock.hpp"
#include "ActionBlock.hpp"
#include "ConditionalBlock.hpp"
#include "EndBlock.hpp"
#include "LoopBlock.hpp"
#include "MergeBlock.hpp"
#include "BlackSensorBlock.hpp"
#include "ColorSensorBlock.hpp"
#include "UltrasonicSensorBlock.hpp"

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
    ALLEGRO_BITMAP *play_button, *play_button_selected, *pause_button, *pause_button_selected;
    ALLEGRO_BITMAP *stop_button, *stop_button_selected, *save_button, *save_button_selected;
    ALLEGRO_BITMAP *load_button, *load_button_selected, *save_as_button, *save_as_button_selected;
    ALLEGRO_BITMAP *vrep_button, *vrep_button_selected, *bluetooth_button, *bluetooth_button_selected, *mini_menu[6];
    ALLEGRO_BITMAP *action_walk, *action_turn, *sensor_black, *sensor_color, *sensor_ultrasonic;
    ALLEGRO_BITMAP *NUMBER[10], *logic_true, *logic_false;
    ALLEGRO_BITMAP *DECISION_BLOCK[3], *DECISION_BLACK_SENSOR_BLOCK[3], *DECISION_COLOR_SENSOR_BLOCK[3], *DECISION_ULTRASONIC_SENSOR_BLOCK[3];
    ALLEGRO_BITMAP *END_BLOCK[3], *FUNCTION_BLOCK[3], *LOOP_BLOCK[3], *MERGE_BLOCK[3], *START_BLOCK[3], *BLACK_SENSOR_BLOCK[3];
    ALLEGRO_BITMAP *COLOR_SENSOR_BLOCK[3], *ULTRASONIC_SENSOR_BLOCK[3], *POINT[2];
    ALLEGRO_BITMAP *trash;
    ALLEGRO_FONT *font;
    bool executing;
    int mouseX, mouseY;
    int menu_selected;
    Block* blocks_list_to_print[100];
    int mouse_aux_x, mouse_aux_y; // usado para arrastar as imagens;

    void load_bitmap(ALLEGRO_BITMAP **bitmap, char *adress);
    void add_block(Block *b);
    void remove_block(Block *b);

public:

    Interface();
    ~Interface();
    void start();
};
#endif
