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
    ALLEGRO_COLOR backgroud_color;
    ALLEGRO_COLOR white;
    ALLEGRO_COLOR primary_menu_color, blocks_menu_color, functions_menu_color, sensors_menu_color;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_BITMAP *play_button, *play_button_selected, *pause_button, *pause_button_selected;
    ALLEGRO_BITMAP *stop_button, *stop_button_selected, *save_button, *save_button_selected;
    ALLEGRO_BITMAP *load_button, *load_button_selected, *save_as_button, *save_as_button_selected;
    ALLEGRO_BITMAP *vrep_button, *vrep_button_selected, *bluetooth_button, *bluetooth_button_selected, *mini_menu[6];
    ALLEGRO_BITMAP *action_walk, *action_turn;
    ALLEGRO_BITMAP *NUMBER[10], *logic_true, *logic_false;
    ALLEGRO_BITMAP *DECISION_BLOCK[4], *BLACK_SENSOR_FUNCTION, *BLACK_SENSOR_1_FUNCTION, *BLACK_SENSOR_2_FUNCTION, *BLACK_SENSOR_3_FUNCTION, *BLACK_SENSOR_4_FUNCTION;
    ALLEGRO_BITMAP *BLACK_SENSOR_5_FUNCTION, *COLOR_SENSOR_FUNCTION, *COLOR_SENSOR_1_FUNCTION, *COLOR_SENSOR_2_FUNCTION, *ULTRASONIC_SENSOR_FUNCTION;
    ALLEGRO_BITMAP *ULTRASONIC_SENSOR_1_FUNCTION, *ULTRASONIC_SENSOR_2_FUNCTION, *ULTRASONIC_SENSOR_3_FUNCTION;
    ALLEGRO_BITMAP *END_BLOCK[4], *FUNCTION_BLOCK[4], *LOOP_BLOCK[4], *MERGE_BLOCK[4], *START_BLOCK[4];
    ALLEGRO_BITMAP *POINT[2];
    ALLEGRO_BITMAP *trash;
    ALLEGRO_FONT *font;
    bool executing;
    int mouseX, mouseY;
    int menu_selected;
    Block* blocks_list_to_print[100];
    int mouse_aux_x, mouse_aux_y; // usado para arrastar as imagens;
    bool black_sensor_menu_selected, color_sensor_menu_selected, ultrasonic_sensor_menu_selected;
    bool dragging_black_sensor1, dragging_black_sensor2, dragging_black_sensor3, dragging_black_sensor4, dragging_black_sensor5;
    bool dragging_color_sensor1, dragging_color_sensor2;
    bool dragging_ultrasonic_sensor1, dragging_ultrasonic_sensor2, dragging_ultrasonic_sensor3;
    bool sub_menu;
    int blocks_menu_X, blocks_menu_Y, sensors_menu_X, sensors_menu_Y, actions_menu_X, actions_menu_Y, extra_menu_X, extra_menu_Y;

    void load_bitmap(ALLEGRO_BITMAP **bitmap, char *adress);
    void add_block(Block *b);
    void remove_block(Block *b);
    void print_primary_menu();
    void print_secondary_menu();
    void print_function_block(Block *b);
    void print_end_block(Block *b);
    void print_start_block(Block *b);
    void print_loop_block(Block *b);
    void print_decision_block(Block *b);
    void print_merge_block(Block *b);
    void load_program_images();
    void destroy_program_images();
    void reset_dragging_variables();

public:

    Interface();
    ~Interface();
    void start();
};
#endif
