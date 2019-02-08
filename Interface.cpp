#include "Interface.hpp"

Interface :: Interface()
{

    //variavel que diz se o programa ainda está executando ou não
    executing = true;

    //inicializa o allegro, dá msg de erro caso falhe alguma inicializacao
    if(!al_init()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro", NULL, NULL);
        //executing = false;
    }
    if(!al_init_image_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_image", NULL, NULL);
        //executing = false;
    }
    if(!al_init_primitives_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_primitives", NULL, NULL);
        //executing = false;
    }
    if(!al_init_native_dialog_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_native_dialog", NULL, NULL);
        //executing = false;
    }
    if(!al_install_keyboard()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_keyboard", NULL, NULL);
        //executing = false;
    }
    if(!al_install_mouse()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_mouse", NULL, NULL);
        //executing = false;
    }
    if(!al_init_font_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_font", NULL, NULL);
        //executing = false;
    }
    if(!al_init_ttf_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_ttf", NULL, NULL);
        //executing = false;
    }

    //CRIA A JANELA DO PROGRAMA E SETA AS VARIÁVEIS DA JANELA
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    display = al_create_display(display_width, display_height);
    al_set_window_position(display, 40, 40);
    al_set_window_title(display, "Fluxprog_v2");
    al_clear_to_color(al_map_rgb(0, 0, 0));


    //inicialização das cores
    black = al_map_rgb(0, 0, 0);
    grey = al_map_rgb(128, 128, 128);
    white = al_map_rgb(255, 255, 255);
    menu_color = al_map_rgb(176, 175, 175);


    if(!display) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do display", NULL, NULL);
        //executing = false;
    }

    //load_bitmap(&menu, "images/menu.png");
    load_bitmap(&play_button, "images/menu_buttons/play_button.png");
    load_bitmap(&pause_button, "images/menu_buttons/pause_button.png");
    load_bitmap(&stop_button, "images/menu_buttons/stop_button.png");
    load_bitmap(&save_button, "images/menu_buttons/save_button.png");
    load_bitmap(&load_button, "images/menu_buttons/load_button.png");
    load_bitmap(&save_as_button, "images/menu_buttons/save_as_button.png");
    load_bitmap(&vrep_button, "images/menu_buttons/vrep_button.png");
    load_bitmap(&bluetooth_button, "images/menu_buttons/bluetooth_button.png");
    load_bitmap(&mini_menu[0], "images/blocks/decision_block/mini_decision_block.png");
    load_bitmap(&mini_menu[1], "images/blocks/function_block/mini_function_block.png");
    load_bitmap(&mini_menu[2], "images/blocks/start_block/mini_start_block.png");
    load_bitmap(&mini_menu[3], "images/blocks/end_block/mini_end_block.png");
    load_bitmap(&mini_menu[4], "images/blocks/merge_block/mini_merge_block.png");
    load_bitmap(&mini_menu[5], "images/blocks/loop_block/mini_loop_block.png");

    load_bitmap(&action_walk, "images/functions/mini_action_walk.png");
    load_bitmap(&action_turn, "images/functions/mini_action_turn.png");
    load_bitmap(&sensor_ultrasonic, "images/functions/mini_ultrasonic_sensor.png");
    load_bitmap(&sensor_black, "images/functions/mini_black_sensor.png");
    load_bitmap(&logic_true, "images/functions/mini_logic_true.png");
    load_bitmap(&logic_false, "images/functions/mini_logic_false.png");
    load_bitmap(&POINT[0], "images/point.png");
    load_bitmap(&POINT[1], "images/hpoint.png");

    load_bitmap(&DECISION_BLOCK[0], "images/blocks/decision_block/decision_block.png");
    load_bitmap(&DECISION_BLOCK[1], "images/blocks/decision_block/decision_block_dragging.png");
    load_bitmap(&DECISION_BLOCK[2], "images/blocks/decision_block/decision_block_executing.png");
    load_bitmap(&DECISION_BLOCK[3], "images/blocks/decision_block/decision_block_selected.png");
    load_bitmap(&END_BLOCK[0], "images/blocks/end_block/end_block.png");
    load_bitmap(&END_BLOCK[1], "images/blocks/end_block/end_block_dragging.png");
    load_bitmap(&END_BLOCK[2], "images/blocks/end_block/end_block_executing.png");
    load_bitmap(&END_BLOCK[3], "images/blocks/end_block/end_block_selected.png");
    load_bitmap(&FUNCTION_BLOCK[0], "images/blocks/function_block/function_block.png");
    load_bitmap(&FUNCTION_BLOCK[1], "images/blocks/function_block/function_block_dragging.png");
    load_bitmap(&FUNCTION_BLOCK[2], "images/blocks/function_block/function_block_executing.png");
    load_bitmap(&FUNCTION_BLOCK[3], "images/blocks/function_block/function_block_selected.png");
    load_bitmap(&LOOP_BLOCK[0], "images/blocks/loop_block/loop_block.png");
    load_bitmap(&LOOP_BLOCK[1], "images/blocks/loop_block/loop_block_dragging.png");
    load_bitmap(&LOOP_BLOCK[2], "images/blocks/loop_block/loop_block_executing.png");
    load_bitmap(&LOOP_BLOCK[3], "images/blocks/loop_block/loop_block_selected.png");
    load_bitmap(&MERGE_BLOCK[0], "images/blocks/merge_block/merge_block.png");
    load_bitmap(&MERGE_BLOCK[1], "images/blocks/merge_block/merge_block_dragging.png");
    load_bitmap(&MERGE_BLOCK[2], "images/blocks/merge_block/merge_block_executing.png");
    load_bitmap(&MERGE_BLOCK[3], "images/blocks/merge_block/merge_block_selected.png");
    load_bitmap(&START_BLOCK[0], "images/blocks/start_block/start_block.png");
    load_bitmap(&START_BLOCK[1], "images/blocks/start_block/start_block_dragging.png");
    load_bitmap(&START_BLOCK[2], "images/blocks/start_block/start_block_executing.png");
    load_bitmap(&START_BLOCK[3], "images/blocks/start_block/start_block_selected.png");

    load_bitmap(&NUMBER[0], "images/functions/mini_number_0.png");
    load_bitmap(&NUMBER[1], "images/functions/mini_number_1.png");
    load_bitmap(&NUMBER[2], "images/functions/mini_number_2.png");
    load_bitmap(&NUMBER[3], "images/functions/mini_number_3.png");
    load_bitmap(&NUMBER[4], "images/functions/mini_number_4.png");
    load_bitmap(&NUMBER[5], "images/functions/mini_number_5.png");
    load_bitmap(&NUMBER[6], "images/functions/mini_number_6.png");
    load_bitmap(&NUMBER[7], "images/functions/mini_number_7.png");
    load_bitmap(&NUMBER[8], "images/functions/mini_number_8.png");
    load_bitmap(&NUMBER[9], "images/functions/mini_number_9.png");

    font = al_load_font("OpenSans-Regular.ttf", 10, 0);
    if(!font) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao da fonte", NULL, NULL);
        executing = false;
    }


    //al_show_native_message_box(display, "Message Box Title", "Error", "Display windoiw teste", NULL, NULL);
}

Interface :: ~Interface() {
    al_destroy_display(display);

    //destroi os objetos usados pelo allegro
	al_destroy_bitmap(play_button);
	al_destroy_bitmap(pause_button);
	al_destroy_bitmap(stop_button);
	al_destroy_bitmap(save_button);
    al_destroy_bitmap(save_as_button);
    al_destroy_bitmap(load_button);
    al_destroy_bitmap(vrep_button);
	al_destroy_bitmap(bluetooth_button);
    for(int i=0; i<6; i++) {
        al_destroy_bitmap(mini_menu[i]);
    }
    al_destroy_bitmap(action_walk);
    al_destroy_bitmap(action_turn);
    al_destroy_bitmap(sensor_ultrasonic);
    al_destroy_bitmap(sensor_black);
    al_destroy_bitmap(logic_true);
    al_destroy_bitmap(logic_false);
    al_destroy_bitmap(POINT[0]);
	al_destroy_bitmap(POINT[1]);
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(DECISION_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(END_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(FUNCTION_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(LOOP_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(MERGE_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(START_BLOCK[i]);
    }
    for(int i=0; i<10; i++) {
        al_destroy_bitmap(NUMBER[i]);
    }

	al_destroy_font(font);

	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
	al_shutdown_native_dialog_addon();
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
}

void Interface :: start() {

    while(executing) {
        //avisa o allegro sobre a mudança de tamanho do display para que as variáveis sejam atualizadas
        al_acknowledge_resize(display);
        //seta a cor de fundo da tela
        al_clear_to_color(al_map_rgb(0, 0, 0));

        //desenha o retangulo no qual ficam os botões de opções
        al_draw_filled_rectangle(0, 0, al_get_display_width(display), (4 + al_get_bitmap_height(play_button)), menu_color);

        //desenha a moldura do botão e dai o botão em si
        al_draw_bitmap(play_button, 14 + 0*al_get_bitmap_width(play_button), 2, 0);

        al_draw_bitmap(pause_button, 14 + 1*al_get_bitmap_width(pause_button), 2, 0);

        al_draw_bitmap(stop_button, 14 + 2*al_get_bitmap_width(stop_button), 2, 0);

        al_draw_bitmap(save_button, 14 + 3*al_get_bitmap_width(save_button), 2, 0);

        al_draw_bitmap(load_button, 14 + 4*al_get_bitmap_width(load_button), 2, 0);

        al_draw_bitmap(save_as_button, 14 + 5*al_get_bitmap_width(save_as_button), 2, 0);

        //desenha o botão de conexão ao v-rep e bluetooth acompanhando o lado direito da tela
        al_draw_bitmap(vrep_button, al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width, 2, 0);
        al_draw_bitmap(bluetooth_button, al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width, 2, 0);

        //desenha todas as molduras dos botões que criam blocos e daí todos os botões
        for (int i = 0; i < 6; i++) {
            al_draw_bitmap(mini_menu[i], 0, (al_get_bitmap_height(play_button)+4) + i * al_get_bitmap_height(mini_menu[i]), 0);
        }
		al_draw_bitmap(action_walk, 0, al_get_bitmap_height(play_button) + 4 + 5*al_get_bitmap_height(mini_menu[0]) + 0 * al_get_bitmap_height(action_walk), 0);
		al_draw_bitmap(action_turn, 0, al_get_bitmap_height(play_button) + 4 + 5*al_get_bitmap_height(mini_menu[0]) + 1 * al_get_bitmap_height(action_walk), 0);
		al_draw_bitmap(sensor_black, 0, al_get_bitmap_height(play_button) + 4 + 5*al_get_bitmap_height(mini_menu[0]) + 2 * al_get_bitmap_height(action_walk), 0);
		al_draw_bitmap(sensor_ultrasonic, 0, al_get_bitmap_height(play_button) + 4 + 5*al_get_bitmap_height(mini_menu[0]) + 3 * al_get_bitmap_height(action_walk), 0);
		al_draw_bitmap(NUMBER[2], 0, al_get_bitmap_height(play_button) + 4 + 5*al_get_bitmap_height(mini_menu[0]) + 4 * al_get_bitmap_height(action_walk), 0); //desenha o 2 pois o bloco criado vai começar com o valor 2 (já que repetir 0 ou 1 vez não faz sentido)
		al_draw_bitmap(logic_true, 0, al_get_bitmap_height(play_button) + 4 + 5*al_get_bitmap_height(mini_menu[0]) + 5 * al_get_bitmap_height(action_walk), 0);
        al_draw_bitmap(logic_false, 0, al_get_bitmap_height(play_button) + 4 + 5*al_get_bitmap_height(mini_menu[0]) + 6 * al_get_bitmap_height(action_walk), 0);


        al_flip_display();

        ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_display_event_source(display));

        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            executing = false;
        }

    }
    delete this;
}

void Interface :: load_bitmap(ALLEGRO_BITMAP **bitmap, char *adress) {
    if(!al_load_bitmap(adress)) {
        //char text = "Erro ao carregar bitmap";
        //strcpy(text, &adress)
        al_show_native_message_box(NULL, NULL, NULL, adress, NULL, NULL);
        executing = false;
    } else {
        *bitmap = al_load_bitmap(adress);
    }
}
