#ifndef _FLUXPROG_HPP_
#define _FLUXPROG_HPP_

#include <allegro5/allegro.h> //biblioteca matriz do allegro
#include <allegro5/allegro_color.h> //biblioteca para rotinas de cor
#include <allegro5/allegro_primitives.h> //biblioteca para desenho de formas geométricas
#include <allegro5/allegro_image.h> // biblioteca para importação e desenho de imagens
#include <allegro5/allegro_native_dialog.h> //biblioteca para uso de diálogos nativos
#include <allegro5/allegro_font.h> // biblioteca para escrita/ uso de fontes
#include <allegro5/allegro_ttf.h> // biblioteca para uso de fontes TrueType
#include <iostream> // usada para debugging
#include <cmath>
#include <fstream>
#include <cstring>
#include <iostream>
#include <signal.h>
#include "Block.hpp"
#include "StartBlock.hpp"
#include "ActionBlock.hpp"
#include "ConditionalBlock.hpp"
#include "EndBlock.hpp"
#include "LoopBlock.hpp"
#include "MergeBlock.hpp"
#include "Communication.hpp"
#include "fluxProgCte.h"

using namespace std;

#define display_width 700
#define display_height 900
#define roll_bar_width 20
#define valor_maximo_blocos 100

class FluxProg {
    ALLEGRO_COLOR black;
    ALLEGRO_COLOR backgroud_color;
    ALLEGRO_COLOR white;
    ALLEGRO_COLOR primary_menu_color, blocks_menu_color, functions_menu_color, sensors_menu_color;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *play_button, *play_button_selected, *pause_button, *pause_button_selected;
    ALLEGRO_BITMAP *stop_button, *stop_button_selected, *save_button, *save_button_selected;
    ALLEGRO_BITMAP *load_button, *load_button_selected, *save_as_button, *save_as_button_selected;
    ALLEGRO_BITMAP *vrep_button, *vrep_button_selected, *bluetooth_button, *bluetooth_button_selected, *mini_menu[6];
    ALLEGRO_BITMAP *WALK_FOWARD_ACTION, *TURN_LEFT_ACTION, *TURN_RIGHT_ACTION;
    ALLEGRO_BITMAP *NUMBER[10], *logic_true, *logic_false;
    ALLEGRO_BITMAP *DECISION_BLOCK[4], *BLACK_SENSOR_FUNCTION, *BLACK_SENSOR_1_FUNCTION, *BLACK_SENSOR_2_FUNCTION, *BLACK_SENSOR_3_FUNCTION, *BLACK_SENSOR_4_FUNCTION;
    ALLEGRO_BITMAP *BLACK_SENSOR_5_FUNCTION, *COLOR_SENSOR_FUNCTION, *COLOR_SENSOR_1_FUNCTION, *COLOR_SENSOR_2_FUNCTION, *ULTRASONIC_SENSOR_FUNCTION;
    ALLEGRO_BITMAP *ULTRASONIC_SENSOR_1_FUNCTION, *ULTRASONIC_SENSOR_2_FUNCTION, *ULTRASONIC_SENSOR_3_FUNCTION;
    ALLEGRO_BITMAP *MICRO_BLACK_SENSOR1, *MICRO_BLACK_SENSOR2, *MICRO_BLACK_SENSOR3, *MICRO_BLACK_SENSOR4, *MICRO_BLACK_SENSOR5;
    ALLEGRO_BITMAP *MICRO_COLOR_SENSOR1, *MICRO_COLOR_SENSOR2, *MICRO_ULTRASONIC_SENSOR1, *MICRO_ULTRASONIC_SENSOR2, *MICRO_ULTRASONIC_SENSOR3;
    ALLEGRO_BITMAP *MICRO_WALK_FOWARD, *MICRO_TURN_LEFT, *MICRO_TURN_RIGHT;
    ALLEGRO_BITMAP *END_BLOCK[4], *FUNCTION_BLOCK[4], *LOOP_BLOCK[4], *MERGE_BLOCK[4], *START_BLOCK[4];
    ALLEGRO_BITMAP *POINT[2];
    ALLEGRO_BITMAP *trash;
    ALLEGRO_BITMAP *arrow;
    ALLEGRO_FONT *font;
    bool executing;
    int mouseX, mouseY;
    int menu_selected;
    Block* blocks_list_to_print[valor_maximo_blocos];
    int mouse_aux_x, mouse_aux_y; // usado para arrastar as imagens;
    bool black_sensor_menu_selected, color_sensor_menu_selected, ultrasonic_sensor_menu_selected;
    bool dragging_black_sensor1, dragging_black_sensor2, dragging_black_sensor3, dragging_black_sensor4, dragging_black_sensor5;
    bool dragging_color_sensor1, dragging_color_sensor2;
    bool dragging_ultrasonic_sensor1, dragging_ultrasonic_sensor2, dragging_ultrasonic_sensor3;
    bool dragging_walk_foward, dragging_turn_left, dragging_turn_right;
    bool sub_menu;
    int menu1_X_limit;
    int menu1_Y_limit;
    int menu_connect_X_begin;
    int menu_connect_X_limit;
    int menu2_X_limit;
    int menu_blocks_Y_begin;
    int menu_blocks_Y_limit;
    int menu_sensors_Y_begin;
    int menu_sensors_Y_limit;
    int menu_actions_Y_begin;
    int menu_actions_Y_limit;
    int menu_extra_Y_begin;
    int menu_extra_Y_limit;
    int temporary_line_X, temporary_line_Y;
    bool drawing_line;
    int block_selected;
    int number_of_selected_out;
    bool executing_fluxogram;
    Block* current_executing_block;
    bool simulator_connected, robot_connected, program_connected;
    Communication* communication;

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
    void check_dragging();
    void draw_dragging();
    void refresh_executing_block();
    void check_mouse_on_menus();
    void print_list_of_blocks();
    void check_mouse_on_points(Block *b);
    void draw_temporary_line();
    bool check_colisions();
    void draw_lines();
    void delete_connections();
    void draw_everything();

    void execute();
    bool check_if_only_one_startblock_exists();
    bool check_if_at_least_one_endblock_exist();
    bool check_if_all_the_blocks_have_connections();
    bool check_if_all_blocks_have_functions_or_sensors();
    void reset_fluxogram_execution();

    void connect_simulator();
    void connect_robot();
    void connect();

public:

    FluxProg();
    ~FluxProg();
    void start();
};
#endif
