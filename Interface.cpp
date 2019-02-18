#include "Interface.hpp"

Interface :: Interface()
{

    //variavel que diz se o programa ainda está executando ou não
    executing = true;

    //inicializa o allegro, dá msg de erro caso falhe alguma inicializacao
    if(!al_init()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro", NULL, NULL);
        executing = false;
    }
    if(!al_init_image_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_image", NULL, NULL);
        executing = false;
    }
    if(!al_init_primitives_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_primitives", NULL, NULL);
        executing = false;
    }
    if(!al_init_native_dialog_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_native_dialog", NULL, NULL);
        executing = false;
    }
    if(!al_install_keyboard()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_keyboard", NULL, NULL);
        executing = false;
    }
    if(!al_install_mouse()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_mouse", NULL, NULL);
        executing = false;
    }
    if(!al_init_font_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_font", NULL, NULL);
        executing = false;
    }
    if(!al_init_ttf_addon()) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do Allegro_ttf", NULL, NULL);
        executing = false;
    }

    //CRIA A JANELA DO PROGRAMA E SETA AS VARIÁVEIS DA JANELA
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_MAXIMIZED);
    display = al_create_display(display_width, display_height);
    al_set_window_position(display, 40, 40);
    al_set_window_title(display, "Fluxprog_v2");
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_set_window_constraints(display, 670, 600, 5000, 5000);
    al_apply_window_constraints(display, true);


    //inicialização das cores
    black = al_map_rgb(0, 0, 0);
    grey = al_map_rgb(128, 128, 128);
    white = al_map_rgb(255, 255, 255);
    menu_color = al_map_rgb(176, 175, 175);

    // checa se o display foi inicializado corretamente, se não foi dá msg de erro
    if(!display) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao do display", NULL, NULL);
        executing = false;
    }

    // carrega todas as imagens usadas no programa, se alguma não for carregada dá msg de erro
    //load_bitmap(&menu, "images/menu.png");
    load_bitmap(&play_button, "images/menu_buttons/play_button.png");
    load_bitmap(&play_button_selected, "images/menu_buttons/play_button_selected.png");
    load_bitmap(&pause_button, "images/menu_buttons/pause_button.png");
    load_bitmap(&pause_button_selected, "images/menu_buttons/pause_button_selected.png");
    load_bitmap(&stop_button, "images/menu_buttons/stop_button.png");
    load_bitmap(&stop_button_selected, "images/menu_buttons/stop_button_selected.png");
    load_bitmap(&save_button, "images/menu_buttons/save_button.png");
    load_bitmap(&save_button_selected, "images/menu_buttons/save_button_selected.png");
    load_bitmap(&load_button, "images/menu_buttons/load_button.png");
    load_bitmap(&load_button_selected, "images/menu_buttons/load_button_selected.png");
    load_bitmap(&save_as_button, "images/menu_buttons/save_as_button.png");
    load_bitmap(&save_as_button_selected, "images/menu_buttons/save_as_button_selected.png");
    load_bitmap(&vrep_button, "images/menu_buttons/vrep_button.png");
    load_bitmap(&vrep_button_selected, "images/menu_buttons/vrep_button_selected.png");
    load_bitmap(&bluetooth_button, "images/menu_buttons/bluetooth_button.png");
    load_bitmap(&bluetooth_button_selected, "images/menu_buttons/bluetooth_button_selected.png");

    load_bitmap(&mini_menu[0], "images/blocks/decision_block/mini_decision_block.png");
    load_bitmap(&mini_menu[1], "images/blocks/function_block/mini_function_block.png");
    load_bitmap(&mini_menu[2], "images/blocks/start_block/mini_start_block.png");
    load_bitmap(&mini_menu[3], "images/blocks/end_block/mini_end_block.png");
    load_bitmap(&mini_menu[4], "images/blocks/merge_block/mini_merge_block.png");
    load_bitmap(&mini_menu[5], "images/blocks/loop_block/mini_loop_block.png");
    load_bitmap(&action_walk, "images/functions/mini_action_walk.png");
    load_bitmap(&action_turn, "images/functions/mini_action_turn.png");
    load_bitmap(&sensor_ultrasonic, "images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor.png");
    load_bitmap(&sensor_black, "images/blocks/black_sensor_block/mini_black_sensor.png");
    load_bitmap(&sensor_color, "images/blocks/color_sensor_block/mini_color_sensor.png");
    load_bitmap(&logic_true, "images/functions/mini_logic_true.png");
    load_bitmap(&logic_false, "images/functions/mini_logic_false.png");
    load_bitmap(&POINT[0], "images/point.png");
    load_bitmap(&POINT[1], "images/hpoint.png");

    load_bitmap(&DECISION_BLOCK[0], "images/blocks/decision_block/decision_block.png");
    load_bitmap(&DECISION_BLOCK[1], "images/blocks/decision_block/decision_block_dragging.png");
    load_bitmap(&DECISION_BLOCK[2], "images/blocks/decision_block/decision_block_executing.png");
    load_bitmap(&DECISION_BLOCK[3], "images/blocks/decision_block/decision_block_selected.png");
    load_bitmap(&DECISION_BLACK_SENSOR_BLOCK[0], "images/blocks/decision_block/decision_black_sensor_block.png");
    load_bitmap(&DECISION_BLACK_SENSOR_BLOCK[1], "images/blocks/decision_block/decision_black_sensor_block_dragging.png");
    load_bitmap(&DECISION_BLACK_SENSOR_BLOCK[2], "images/blocks/decision_block/decision_black_sensor_block_executing.png");
    load_bitmap(&DECISION_BLACK_SENSOR_BLOCK[3], "images/blocks/decision_block/decision_black_sensor_block_selected.png");
    load_bitmap(&DECISION_COLOR_SENSOR_BLOCK[0], "images/blocks/decision_block/decision_color_sensor_block.png");
    load_bitmap(&DECISION_COLOR_SENSOR_BLOCK[1], "images/blocks/decision_block/decision_color_sensor_block_dragging.png");
    load_bitmap(&DECISION_COLOR_SENSOR_BLOCK[2], "images/blocks/decision_block/decision_color_sensor_block_executing.png");
    load_bitmap(&DECISION_COLOR_SENSOR_BLOCK[3], "images/blocks/decision_block/decision_color_sensor_block_selected.png");
    load_bitmap(&DECISION_ULTRASONIC_SENSOR_BLOCK[0], "images/blocks/decision_block/decision_ultrasonic_sensor_block.png");
    load_bitmap(&DECISION_ULTRASONIC_SENSOR_BLOCK[1], "images/blocks/decision_block/decision_ultrasonic_sensor_block_dragging.png");
    load_bitmap(&DECISION_ULTRASONIC_SENSOR_BLOCK[2], "images/blocks/decision_block/decision_ultrasonic_sensor_block_executing.png");
    load_bitmap(&DECISION_ULTRASONIC_SENSOR_BLOCK[3], "images/blocks/decision_block/decision_ultrasonic_sensor_block_selected.png");
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
    load_bitmap(&BLACK_SENSOR_BLOCK[0], "images/blocks/black_sensor_block/black_sensor_block.png");
    load_bitmap(&BLACK_SENSOR_BLOCK[1], "images/blocks/black_sensor_block/black_sensor_block_dragging.png");
    load_bitmap(&BLACK_SENSOR_BLOCK[2], "images/blocks/black_sensor_block/black_sensor_block_executing.png");
    load_bitmap(&BLACK_SENSOR_BLOCK[3], "images/blocks/black_sensor_block/black_sensor_block_selected.png");
    load_bitmap(&COLOR_SENSOR_BLOCK[0], "images/blocks/color_sensor_block/color_sensor_block.png");
    load_bitmap(&COLOR_SENSOR_BLOCK[1], "images/blocks/color_sensor_block/color_sensor_block_dragging.png");
    load_bitmap(&COLOR_SENSOR_BLOCK[2], "images/blocks/color_sensor_block/color_sensor_block_executing.png");
    load_bitmap(&COLOR_SENSOR_BLOCK[3], "images/blocks/color_sensor_block/color_sensor_block_selected.png");
    load_bitmap(&ULTRASONIC_SENSOR_BLOCK[0], "images/blocks/ultrasonic_sensor_block/ultrasonic_sensor_block.png");
    load_bitmap(&ULTRASONIC_SENSOR_BLOCK[1], "images/blocks/ultrasonic_sensor_block/ultrasonic_sensor_block_dragging.png");
    load_bitmap(&ULTRASONIC_SENSOR_BLOCK[2], "images/blocks/ultrasonic_sensor_block/ultrasonic_sensor_block_executing.png");
    load_bitmap(&ULTRASONIC_SENSOR_BLOCK[3], "images/blocks/ultrasonic_sensor_block/ultrasonic_sensor_block_selected.png");

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

    //carrega a fonte, dá msg de erro caso não consiga ser carregada
    font = al_load_font("OpenSans-Regular.ttf", 10, 0);
    if(!font) {
        al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao da fonte", NULL, NULL);
        executing = false;
    }
    for(int i=0; i<100; i++) {
        blocks_list_to_print[i] = NULL;
    }

}

Interface :: ~Interface() {
    al_destroy_display(display);

    //destroi os objetos usados pelo allegro
	al_destroy_bitmap(play_button);
    al_destroy_bitmap(play_button_selected);
	al_destroy_bitmap(pause_button);
    al_destroy_bitmap(pause_button_selected);
	al_destroy_bitmap(stop_button);
    al_destroy_bitmap(stop_button_selected);
	al_destroy_bitmap(save_button);
    al_destroy_bitmap(save_button_selected);
    al_destroy_bitmap(save_as_button);
    al_destroy_bitmap(save_as_button_selected);
    al_destroy_bitmap(load_button);
    al_destroy_bitmap(load_button_selected);
    al_destroy_bitmap(vrep_button);
    al_destroy_bitmap(vrep_button_selected);
	al_destroy_bitmap(bluetooth_button);
    al_destroy_bitmap(bluetooth_button_selected);
    for(int i=0; i<6; i++) {
        al_destroy_bitmap(mini_menu[i]);
    }
    al_destroy_bitmap(action_walk);
    al_destroy_bitmap(action_turn);
    al_destroy_bitmap(sensor_ultrasonic);
    al_destroy_bitmap(sensor_black);
    al_destroy_bitmap(sensor_color);
    al_destroy_bitmap(logic_true);
    al_destroy_bitmap(logic_false);
    al_destroy_bitmap(POINT[0]);
	al_destroy_bitmap(POINT[1]);
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(DECISION_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(DECISION_BLACK_SENSOR_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(DECISION_COLOR_SENSOR_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(DECISION_ULTRASONIC_SENSOR_BLOCK[i]);
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
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(BLACK_SENSOR_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(COLOR_SENSOR_BLOCK[i]);
    }
    for(int i=0; i<4; i++) {
        al_destroy_bitmap(ULTRASONIC_SENSOR_BLOCK[i]);
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

        //desenha os botões do menu e checa a posição do mouse para ver se estão selecionados ou não
        if((mouseX < 14 + al_get_bitmap_width(play_button)) && (mouseX > 14) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(play_button))) {
            al_draw_bitmap(play_button_selected, 14 + 0*al_get_bitmap_width(play_button), 2, 0);
            menu_selected = 1;
        } else {
            al_draw_bitmap(play_button, 14 + 0*al_get_bitmap_width(play_button), 2, 0);
        }
        if((mouseX < 14 + 2*al_get_bitmap_width(pause_button)) && (mouseX > 14 + al_get_bitmap_width(pause_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(pause_button))) {
            al_draw_bitmap(pause_button_selected, 14 + 1*al_get_bitmap_width(pause_button), 2, 0);
            menu_selected = 2;
        } else {
            al_draw_bitmap(pause_button, 14 + 1*al_get_bitmap_width(pause_button), 2, 0);
        }
        if((mouseX < 14 + 3*al_get_bitmap_width(stop_button)) && (mouseX > 14 + 2*al_get_bitmap_width(stop_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(stop_button))) {
            al_draw_bitmap(stop_button_selected, 14 + 2*al_get_bitmap_width(stop_button), 2, 0);
            menu_selected = 3;
        } else {
            al_draw_bitmap(stop_button, 14 + 2*al_get_bitmap_width(stop_button), 2, 0);
        }
        if((mouseX < 14 + 4*al_get_bitmap_width(save_button)) && (mouseX > 14 + 3*al_get_bitmap_width(save_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(save_button))) {
            al_draw_bitmap(save_button_selected, 14 + 3*al_get_bitmap_width(save_button), 2, 0);
            menu_selected = 4;
        } else {
            al_draw_bitmap(save_button, 14 + 3*al_get_bitmap_width(save_button), 2, 0);
        }
        if((mouseX < 14 + 5*al_get_bitmap_width(load_button)) && (mouseX > 14 + 4*al_get_bitmap_width(load_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(load_button))) {
            al_draw_bitmap(load_button_selected, 14 + 4*al_get_bitmap_width(load_button), 2, 0);
            menu_selected = 5;
        } else {
            al_draw_bitmap(load_button, 14 + 4*al_get_bitmap_width(load_button), 2, 0);
        }
        if((mouseX < 14 + 6*al_get_bitmap_width(save_as_button)) && (mouseX > 14 + 5*al_get_bitmap_width(save_as_button)) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(save_as_button))) {
            al_draw_bitmap(save_as_button_selected, 14 + 5*al_get_bitmap_width(save_as_button), 2, 0);
            menu_selected = 6;
        } else {
            al_draw_bitmap(save_as_button, 14 + 5*al_get_bitmap_width(save_as_button), 2, 0);
        }

        //desenha o botão de conexão ao v-rep e bluetooth acompanhando o lado direito da tela
        if((mouseX < al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width) && (mouseX > al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(bluetooth_button))) {
            al_draw_bitmap(bluetooth_button_selected, al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width, 2, 0);
            menu_selected = 7;
        } else {
            al_draw_bitmap(bluetooth_button, al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width, 2, 0);
        }
        if((mouseX < al_get_display_width(display) - roll_bar_width) && (mouseX > al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width) && (mouseY >= 2) && (mouseY <= 2+ al_get_bitmap_height(vrep_button))) {
            al_draw_bitmap(vrep_button_selected, al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width, 2, 0);
            menu_selected = 8;
        } else {
            al_draw_bitmap(vrep_button, al_get_display_width(display) + 10 - al_get_bitmap_width(vrep_button) - roll_bar_width, 2, 0);
        }


        //desenha todos os botões que criam blocos
        for (int i = 0; i < 6; i++) {
            al_draw_bitmap(mini_menu[i], 0, (al_get_bitmap_height(play_button)+4) + i * al_get_bitmap_height(mini_menu[i]), 0);
        }

		al_draw_bitmap(sensor_black, 0, al_get_bitmap_height(play_button) + 4 + 6*al_get_bitmap_height(mini_menu[0]) + 0 * al_get_bitmap_height(action_walk), 0);
		al_draw_bitmap(sensor_color, 0, al_get_bitmap_height(play_button) + 4 + 6*al_get_bitmap_height(mini_menu[0]) + 1 * al_get_bitmap_height(action_walk), 0);
        al_draw_bitmap(sensor_ultrasonic, 0, al_get_bitmap_height(play_button) + 4 + 6*al_get_bitmap_height(mini_menu[0]) + 2 * al_get_bitmap_height(action_walk), 0);
        al_draw_bitmap(action_walk, 0, al_get_bitmap_height(play_button) + 4 + 6*al_get_bitmap_height(mini_menu[0]) + 3 * al_get_bitmap_height(action_walk), 0);
        al_draw_bitmap(action_turn, 0, al_get_bitmap_height(play_button) + 4 + 6*al_get_bitmap_height(mini_menu[0]) + 4 * al_get_bitmap_height(action_walk), 0);
        al_draw_bitmap(NUMBER[2], 0, al_get_bitmap_height(play_button) + 4 + 6*al_get_bitmap_height(mini_menu[0]) + 5 * al_get_bitmap_height(action_walk), 0); //desenha o 2 pois o bloco criado vai começar com o valor 2 (já que repetir 0 ou 1 vez não faz sentido)
		al_draw_bitmap(logic_true, 0, al_get_bitmap_height(play_button) + 4 + 6*al_get_bitmap_height(mini_menu[0]) + 6 * al_get_bitmap_height(action_walk), 0);
        al_draw_bitmap(logic_false, 0, al_get_bitmap_height(play_button) + 4 + 6*al_get_bitmap_height(mini_menu[0]) + 7 * al_get_bitmap_height(action_walk), 0);

        for(int i=0; i<15; i++) {
            // checa se o mouse está sobre o menu e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > 0) && (mouseX < al_get_bitmap_width(action_walk))) {
                if((mouseY > al_get_bitmap_height(play_button) + 4 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 4 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(0, al_get_bitmap_height(play_button) + 4 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+4 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 9 + i;
                }
            }
        }
        //percorre toda a lista de impressão dos blocos
        for(int i=0; i<100; i++) {
            //testa se na posição do array existe mesmo um bloco
            if(blocks_list_to_print[i] != NULL) {
                /*
                    tipo 1 = bloco de ação
                    tipo 2 = bloco de sensor de ultrassom
                    tipo 3 = bloco de sensor de fita preta
                    tipo 4 = bloco de sensor de cor
                    tipo 5 = bloco de fim
                    tipo 6 = bloco de início
                    tipo 7 = bloco de loop
                    tipo 8 = bloco de condição
                    tipo 9 = bloco de junção
                */
                if(blocks_list_to_print[i]->getType() == 1) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(FUNCTION_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(FUNCTION_BLOCK[0])))) {
                        al_draw_bitmap(FUNCTION_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);

                    } else {
                        al_draw_bitmap(FUNCTION_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(false);
                    }
                }
                if(blocks_list_to_print[i]->getType() == 2) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(ULTRASONIC_SENSOR_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(ULTRASONIC_SENSOR_BLOCK[0])))) {
                        al_draw_bitmap(ULTRASONIC_SENSOR_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(true);
                    } else {
                        al_draw_bitmap(ULTRASONIC_SENSOR_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(false);
                    }
                }
                if(blocks_list_to_print[i]->getType() == 3) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(BLACK_SENSOR_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(BLACK_SENSOR_BLOCK[0])))) {
                        al_draw_bitmap(BLACK_SENSOR_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(true);
                    } else {
                        al_draw_bitmap(BLACK_SENSOR_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(false);
                    }
                }
                if(blocks_list_to_print[i]->getType() == 4) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(COLOR_SENSOR_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(COLOR_SENSOR_BLOCK[0])))) {
                        al_draw_bitmap(COLOR_SENSOR_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(true);
                    } else {
                        al_draw_bitmap(COLOR_SENSOR_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(false);
                    }
                }
                if(blocks_list_to_print[i]->getType() == 5) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(END_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(END_BLOCK[0])))) {
                        al_draw_bitmap(END_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(true);
                    } else {
                        al_draw_bitmap(END_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(false);
                    }
                }
                if(blocks_list_to_print[i]->getType() == 6) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(START_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(START_BLOCK[0])))) {
                        al_draw_bitmap(START_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(true);
                    } else {
                        al_draw_bitmap(START_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(false);
                    }
                }
                if(blocks_list_to_print[i]->getType() == 7) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(LOOP_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(LOOP_BLOCK[0])))) {
                        al_draw_bitmap(LOOP_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(true);
                    } else {
                        al_draw_bitmap(LOOP_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(false);
                    }
                }
                if(blocks_list_to_print[i]->getType() == 8) {
                    //testa qual o tipo de sensor da comparação para imprimir a imagem certa
                    switch(blocks_list_to_print[i]->getTypeOfSensor()) {
                        case 1:
                            if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(DECISION_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(DECISION_BLOCK[0])))) {
                                al_draw_bitmap(DECISION_BLACK_SENSOR_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                                //blocks_list_to_print[i]->setSelected(true);
                            } else {
                                al_draw_bitmap(DECISION_BLACK_SENSOR_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                                //blocks_list_to_print[i]->setSelected(false);
                            }
                            break;
                        case 2:
                            if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(DECISION_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(DECISION_BLOCK[0])))) {
                                al_draw_bitmap(DECISION_COLOR_SENSOR_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                                //blocks_list_to_print[i]->setSelected(true);
                            } else {
                                al_draw_bitmap(DECISION_COLOR_SENSOR_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                                //blocks_list_to_print[i]->setSelected(false);
                            }
                            break;
                        case 3:
                            if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(DECISION_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(DECISION_BLOCK[0])))) {
                                al_draw_bitmap(DECISION_ULTRASONIC_SENSOR_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                                //blocks_list_to_print[i]->setSelected(true);
                            } else {
                                al_draw_bitmap(DECISION_ULTRASONIC_SENSOR_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                                //blocks_list_to_print[i]->setSelected(false);
                            }
                            break;
                        default:
                            if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(DECISION_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(DECISION_BLOCK[0])))) {
                                al_draw_bitmap(DECISION_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                                //blocks_list_to_print[i]->setSelected(true);
                            } else {
                                al_draw_bitmap(DECISION_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                                //blocks_list_to_print[i]->setSelected(false);
                            }
                    }
                }
                if(blocks_list_to_print[i]->getType() == 9) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + al_get_bitmap_width(MERGE_BLOCK[0]))) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + al_get_bitmap_height(MERGE_BLOCK[0])))) {
                        al_draw_bitmap(MERGE_BLOCK[3], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(true);
                    } else {
                        al_draw_bitmap(MERGE_BLOCK[0], blocks_list_to_print[i]->getX(), blocks_list_to_print[i]->getY(), 0);
                        //blocks_list_to_print[i]->setSelected(false);
                    }
                }

                if(blocks_list_to_print[i]->getSelected() == true) {
                    blocks_list_to_print[i]->setX(mouseX - mouse_aux_x);
                    blocks_list_to_print[i]->setY(mouseY - mouse_aux_y);
                    //cout<<"clicou em cima"<<endl;
                }
            }
        }

        int menu1_X_limit = al_get_display_width(display);
        int menu1_Y_limit = 4 + al_get_bitmap_height(play_button);
        int menu2_X_limit = al_get_bitmap_width(action_walk);
        int menu2_Y_limit = menu1_Y_limit + 12*al_get_bitmap_height(action_walk);
        //se o mouse não está sobre os menus seta a variável de teste para zero
        if(((mouseX < menu1_X_limit) && (mouseY < menu1_Y_limit)) || ((mouseX < menu2_X_limit) && (mouseY < menu2_Y_limit))) {

        } else {
            menu_selected = 0;
        }

        al_flip_display();

        ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_mouse_event_source());

        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            //se a janela for fechada o programa para de executar
            executing = false;
        }

        if(events.type == ALLEGRO_EVENT_MOUSE_AXES) {
            //atualiza as variaveis com as posições do mouse
            mouseX = events.mouse.x;
            mouseY = events.mouse.y;
        }
        if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

            for(int i=0; i<100; i++) {
                if(blocks_list_to_print[i] != NULL) {
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX()+blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY()+blocks_list_to_print[i]->getHeight()))) {
                        blocks_list_to_print[i]->setSelected(true);
                        mouse_aux_x = mouseX-blocks_list_to_print[i]->getX();
                        mouse_aux_y = mouseY-blocks_list_to_print[i]->getY();
                        //cout<<"clicou em cima"<<endl;
                    }
                }
            }



        }

        if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            /*
                menu_selected == 1 >> play
                menu_selected == 2 >> pause
                menu_selected == 3 >> stop
                menu_selected == 4 >> save
                menu_selected == 5 >> load
                menu_selected == 6 >> save_as
                menu_selected == 7 >> bluetooth
                menu_selected == 8 >> vrep
                menu_selected == 9 >> bloco de condição
                menu_selected == 10 >> bloco de ação
                menu_selected == 11 >> bloco de inicio
                menu_selected == 12 >> bloco de fim
                menu_selected == 13 >> bloco de junção
                menu_selected == 14 >> bloco de loop
                menu_selected == 15 >> bloco de sensor de fita
                menu_selected == 16 >> bloco de sensor de cor
                menu_selected == 17 >> bloco de sensor de ultrassom
                menu_selected == 18 >> ação de andar
                menu_selected == 19 >> ação de virar
                menu_selected == 20 >> numero 2
                menu_selected == 21 >> lógico V
                menu_selected == 22 >> lógico F
            */
            if(menu_selected == 1) {
                cout<<"play"<<endl;
            }
            if(menu_selected == 2) {
                cout<<"pause"<<endl;
            }
            if(menu_selected == 3) {
                cout<<"stop"<<endl;
            }
            if(menu_selected == 4) {
                cout<<"save"<<endl;
            }
            if(menu_selected == 5) {
                cout<<"load"<<endl;
            }
            if(menu_selected == 6) {
                cout<<"save_as"<<endl;
            }
            if(menu_selected == 7) {
                cout<<"bluetooth"<<endl;
            }
            if(menu_selected == 8) {
                cout<<"vrep"<<endl;
            }
            if(menu_selected == 9) {
                ConditionalBlock *aux = new ConditionalBlock();
                aux->setWidth(al_get_bitmap_width(DECISION_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(DECISION_BLOCK[0]));
                aux->setTypeOfSensor(0);
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 10) {
                ActionBlock *aux = new ActionBlock();
                aux->setWidth(al_get_bitmap_width(FUNCTION_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(FUNCTION_BLOCK[0]));
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 11) {
                StartBlock *aux = new StartBlock();
                aux->setWidth(al_get_bitmap_width(START_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(START_BLOCK[0]));
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 12) {
                EndBlock *aux = new EndBlock();
                aux->setWidth(al_get_bitmap_width(END_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(END_BLOCK[0]));
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 13) {
                MergeBlock *aux = new MergeBlock();
                aux->setWidth(al_get_bitmap_width(MERGE_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(MERGE_BLOCK[0]));
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 14) {
                LoopBlock *aux = new LoopBlock();
                aux->setWidth(al_get_bitmap_width(LOOP_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(LOOP_BLOCK[0]));
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 15) {
                BlackSensorBlock *aux = new BlackSensorBlock();
                aux->setWidth(al_get_bitmap_width(BLACK_SENSOR_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(BLACK_SENSOR_BLOCK[0]));
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 16) {
                ColorSensorBlock *aux = new ColorSensorBlock();
                aux->setWidth(al_get_bitmap_width(COLOR_SENSOR_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(COLOR_SENSOR_BLOCK[0]));
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 17) {
                UltrasonicSensorBlock *aux = new UltrasonicSensorBlock();
                aux->setWidth(al_get_bitmap_width(ULTRASONIC_SENSOR_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(ULTRASONIC_SENSOR_BLOCK[0]));
                aux->setX(50);
                aux->setY(100);
                add_block(aux);
            }
            if(menu_selected == 18) {
                cout<<"andar"<<endl;
            }
            if(menu_selected == 19) {
                cout<<"virar"<<endl;
            }
            if(menu_selected == 20) {
                cout<<"numero 2"<<endl;
            }
            if(menu_selected == 21) {
                cout<<"verdadeiro"<<endl;
            }
            if(menu_selected == 22) {
                cout<<"falso"<<endl;
            }

            for(int i=0; i<100; i++) {
                if(blocks_list_to_print[i] != NULL) {
                    if(blocks_list_to_print[i]->getSelected() == true) {
                        blocks_list_to_print[i]->setSelected(false);
                    }
                }
            }

        }


    }
    //quando o programa finaliza ele chama a destrutora
    delete this;
}

void Interface :: load_bitmap(ALLEGRO_BITMAP **bitmap, char *adress) {
    //se não achar a imagem no diretorio especificado dá msg de erro e para a execução
    if(!al_load_bitmap(adress)) {
        al_show_native_message_box(NULL, NULL, NULL, adress, NULL, NULL);
        executing = false;
    } else {
        *bitmap = al_load_bitmap(adress);
    }
}

void Interface :: add_block(Block *b) {
    for(int i=0; i<100; i++) {
        cout << i << endl;
        if(blocks_list_to_print[i] == NULL) {
            blocks_list_to_print[i] = b;
            cout<< "adicionou na lista" << endl;
            break;
        }
    }

}
