#include "Interface.hpp"

Interface :: Interface() {

    //variavel que diz se o programa ainda está executando ou não
    executing = true;

    //inicializa o allegro, dá msg de erro caso falhe alguma inicializacao
    if(!al_init()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_init_image_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_image", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_init_primitives_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_primitives", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_init_native_dialog_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_native_dialog", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_install_keyboard()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_keyboard", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_install_mouse()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_mouse", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_init_font_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_font", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    if(!al_init_ttf_addon()) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do Allegro_ttf", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
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

    timer = al_create_timer(0.5);
   if(!timer) {
       al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do timer", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
       executing = false;
   }

    //inicialização das cores
    black = al_map_rgb(0, 0, 0);
    backgroud_color = al_map_rgb(128, 128, 128);
    white = al_map_rgb(255, 255, 255);
    primary_menu_color = al_map_rgb(176, 175, 175);
    blocks_menu_color = al_map_rgb(255, 0, 0);
    functions_menu_color = al_map_rgb(255, 0, 0);
    sensors_menu_color = al_map_rgb(0, 0, 255);

    reset_dragging_variables();

    black_sensor_menu_selected = false;
    color_sensor_menu_selected = false;
    ultrasonic_sensor_menu_selected = false;
    sub_menu = false;
    drawing_line = false;
    temporary_line_X = 0;
    temporary_line_Y = 0;
    executing_fluxogram = false;
    current_executing_block = NULL;
    program_connected = false;

    // checa se o display foi inicializado corretamente, se não foi dá msg de erro
    if(!display) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao do display", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }

    // carrega todas as imagens usadas no programa, se alguma não for carregada dá msg de erro
    load_program_images();

    //carrega a fonte, dá msg de erro caso não consiga ser carregada
    font = al_load_font("OpenSans-Regular.ttf", 10, 0);
    if(!font) {
        al_show_native_message_box(NULL, "Fluxprog", "ERRO", "Erro na inicializacao da fonte", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    }
    for(int i=0; i<valor_maximo_blocos; i++) {
        blocks_list_to_print[i] = NULL;
    }

    communication = new Communication();

}

Interface :: ~Interface() {

    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            delete blocks_list_to_print[i];
        }
    }

    //destroi os objetos usados pelo allegro
	destroy_program_images();

	al_destroy_font(font);

	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
	al_shutdown_native_dialog_addon();
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
    al_destroy_display(display);

    delete communication;
}

void Interface :: start() {

    while(executing) {
        //avisa o allegro sobre a mudança de tamanho do display para que as variáveis sejam atualizadas
        al_acknowledge_resize(display);
        //seta a cor de fundo da tela
        al_clear_to_color(backgroud_color);

        //desenha
        draw_everything();

        //atualiza o display
        al_flip_display();

        //executa o fluxograma
        if(executing_fluxogram == true) {
            execute();
        }

        //cria a lista com eventos
        ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_mouse_event_source());
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        al_register_event_source(event_queue, al_get_timer_event_source(timer));

        al_start_timer(timer);

        //espera pelo próximo evento
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            //se a janela for fechada o programa para de executar
            executing = false;
            communication->setCommand(CLOSE_PROGRAM);
        }

        if(events.type == ALLEGRO_EVENT_MOUSE_AXES) {
            //atualiza as variaveis com as posições do mouse
            mouseX = events.mouse.x;
            mouseY = events.mouse.y;
        }

        if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

            //checa se a flecha foi solta na entrada de algum bloco
            if(drawing_line == true) {
                for(int i=0; i<valor_maximo_blocos; i++) {
                    if(blocks_list_to_print[i] != NULL) {

                        check_mouse_on_points(blocks_list_to_print[i]);
                        if(blocks_list_to_print[block_selected] != NULL) {
                            if(blocks_list_to_print[i]->getIn1Selected() == true) {
                                if(number_of_selected_out == 1) {
                                    blocks_list_to_print[block_selected]->setNext1(blocks_list_to_print[i]);
                                    blocks_list_to_print[i]->setPrevious1(blocks_list_to_print[block_selected]);
                                } else {
                                    blocks_list_to_print[block_selected]->setNext2(blocks_list_to_print[i]);
                                    blocks_list_to_print[i]->setPrevious1(blocks_list_to_print[block_selected]);
                                }

                            } else if(blocks_list_to_print[i]->getIn2Selected() == true) {
                                if(number_of_selected_out == 1) {
                                    blocks_list_to_print[block_selected]->setNext1(blocks_list_to_print[i]);
                                    blocks_list_to_print[i]->setPrevious2(blocks_list_to_print[block_selected]);
                                } else {
                                    blocks_list_to_print[block_selected]->setNext2(blocks_list_to_print[i]);
                                    blocks_list_to_print[i]->setPrevious2(blocks_list_to_print[block_selected]);
                                }
                            }
                        }
                    }
                }
            }

            bool line_test = drawing_line;
            //testa se pressionou o mouse sobre algum bloco existente,
            //se sim seta a variável dizendo que o bloco está selecionado para poder arrastá-lo
            for(int i=0; i<valor_maximo_blocos; i++) {
                if(blocks_list_to_print[i] != NULL) {

                    check_mouse_on_points(blocks_list_to_print[i]);
                    //checa se clicou nas bolinhas de saida dos blocos, se sim, seta as variáveis para desenhar as linhas de ligação
                    if(blocks_list_to_print[i]->getOut1Selected() == true) {
                        temporary_line_X = mouseX;
                        temporary_line_Y = mouseY;
                        drawing_line = true;
                        block_selected = i;
                        number_of_selected_out = 1;
                    } else if(blocks_list_to_print[i]->getOut2Selected() == true) {
                        temporary_line_X = mouseX;
                        temporary_line_Y = mouseY;
                        drawing_line = true;
                        block_selected = i;
                        number_of_selected_out = 2;
                    } else if(drawing_line == line_test) {
                        temporary_line_X = 0;
                        temporary_line_Y = 0;
                        drawing_line = false;
                    }
                    //checa se clicou sobre os blocos, se sim, seta as variáveis de seleção e seta as variáveis para poder arrastar
                    if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX()+blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY()+blocks_list_to_print[i]->getHeight()))) {
                        if((blocks_list_to_print[i]->getIn1Selected() == false) && (blocks_list_to_print[i]->getOut1Selected() == false) && (blocks_list_to_print[i]->getIn2Selected() == false) && (blocks_list_to_print[i]->getOut2Selected() == false)) {
                            blocks_list_to_print[i]->setDragging(true);
                            blocks_list_to_print[i]->setSelected(true);
                            mouse_aux_x = mouseX-blocks_list_to_print[i]->getX();
                            mouse_aux_y = mouseY-blocks_list_to_print[i]->getY();
                        }
                    } else {
                        blocks_list_to_print[i]->setSelected(false);
                    }
                }
            }

            // caso tenha clicado sobre um dos menus de ação ou sensor, seta as variáveis para arrastar com o mouse.
            switch(menu_selected) {
                case 18:
                    dragging_walk_foward = true;
                    break;
                case 19:
                    dragging_turn_left = true;
                    break;
                case 20:
                    dragging_turn_right = true;
                    break;
                case 24:
                    dragging_black_sensor1 = true;
                    break;
                case 25:
                    dragging_black_sensor2 = true;
                    break;
                case 26:
                    dragging_black_sensor3 = true;
                    break;
                case 27:
                    dragging_black_sensor4 = true;
                    break;
                case 28:
                    dragging_black_sensor5 = true;
                    break;
                case 29:
                    dragging_color_sensor1 = true;
                    break;
                case 30:
                    dragging_color_sensor2 = true;
                    break;
                case 31:
                    dragging_ultrasonic_sensor1 = true;
                    break;
                case 32:
                    dragging_ultrasonic_sensor2 = true;
                    break;
                case 33:
                    dragging_ultrasonic_sensor3 = true;
                    break;
            }
            if(menu_selected == 9) {
                mouse_aux_x = 60;
                mouse_aux_y = 35;
                ConditionalBlock *aux = new ConditionalBlock();
                aux->setWidth(al_get_bitmap_width(DECISION_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(DECISION_BLOCK[0]));
                aux->setTypeOfSensor(0);
                aux->setX(mouseX-60);
                aux->setY(mouseY-35);
                aux->setSelected(true);
                aux->setDragging(true);
                aux->setName("bloco condicional");
                add_block(aux);
            }
            if(menu_selected == 10) {
                mouse_aux_x = 45;
                mouse_aux_y = 20;
                ActionBlock *aux = new ActionBlock();
                aux->setWidth(al_get_bitmap_width(FUNCTION_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(FUNCTION_BLOCK[0]));
                aux->setFunction(0);
                aux->setX(mouseX-45);
                aux->setY(mouseY-20);
                aux->setSelected(true);
                aux->setDragging(true);
                aux->setName("bloco de ação");
                add_block(aux);
            }
            if(menu_selected == 11) {
                mouse_aux_x = 40;
                mouse_aux_y = 15;
                StartBlock *aux = new StartBlock();
                aux->setWidth(al_get_bitmap_width(START_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(START_BLOCK[0]));
                aux->setName("bloco inicio");
                aux->setX(mouseX-40);
                aux->setY(mouseY-15);
                aux->setSelected(true);
                aux->setDragging(true);
                add_block(aux);
            }
            if(menu_selected == 12) {
                mouse_aux_x = 40;
                mouse_aux_y = 15;
                EndBlock *aux = new EndBlock();
                aux->setWidth(al_get_bitmap_width(END_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(END_BLOCK[0]));
                aux->setX(mouseX-40);
                aux->setY(mouseY-15);
                aux->setName("bloco de fim");
                aux->setSelected(true);
                aux->setDragging(true);
                add_block(aux);
            }
            if(menu_selected == 13) {
                mouse_aux_x = 15;
                mouse_aux_y = 10;
                MergeBlock *aux = new MergeBlock();
                aux->setWidth(al_get_bitmap_width(MERGE_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(MERGE_BLOCK[0]));
                aux->setX(mouseX-15);
                aux->setY(mouseY-10);
                aux->setName("bloco de junção");
                aux->setSelected(true);
                aux->setDragging(true);
                add_block(aux);
            }
            if(menu_selected == 14) {
                mouse_aux_x = 30;
                mouse_aux_y = 40;
                LoopBlock *aux = new LoopBlock();
                aux->setWidth(al_get_bitmap_width(LOOP_BLOCK[0]));
                aux->setHeight(al_get_bitmap_height(LOOP_BLOCK[0]));
                aux->setX(mouseX-30);
                aux->setY(mouseY-40);
                aux->setName("bloco de loop");
                aux->setLimitedLoop(true);
                aux->setNumberOfLoops(2);
                aux->setSelected(true);
                aux->setDragging(true);
                add_block(aux);
            }

            //se o sub menu estava aberto e clicou fora, fecha o sub menu
            if(menu_selected != 15) {
                black_sensor_menu_selected = false;
            }
            if(menu_selected != 16) {
                color_sensor_menu_selected = false;
            }
            if(menu_selected != 17) {
                ultrasonic_sensor_menu_selected = false;
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
                menu_selected == 15 >> menu de sensor de fita
                menu_selected == 16 >> menu de sensor de cor
                menu_selected == 17 >> menu de sensor de ultrassom
                menu_selected == 18 >> ação de andar
                menu_selected == 19 >> ação de virar esquerda
                menu_selected == 20 >> ação de virar direita
                menu_selected == 21 >> numero 2
                menu_selected == 21 >> lógico V
                menu_selected == 22 >> lógico F
            */
            if(menu_selected == 1) {
                //cout<<"play"<<endl;
                executing_fluxogram = true;
                //teste bloco de inicio
                if(check_if_only_one_startblock_exists() == false) {
                    //erro blocos de inicio
                    al_show_native_message_box(display, "Fluxprog", "ERRO", "Deve existir um, e apenas um, bloco de início.", "Ok", ALLEGRO_MESSAGEBOX_WARN);
                    executing_fluxogram = false;
                }
                //teste blocos de fim
                if(check_if_at_least_one_endblock_exist() == false) {
                    //erro blocos de fim
                    al_show_native_message_box(display, "Fluxprog", "ERRO", "Deve existir pelo menos um bloco de fim.", "Ok", ALLEGRO_MESSAGEBOX_WARN);
                    executing_fluxogram = false;
                }
                //teste se todos os blocos estão ligados
                if(check_if_all_the_blocks_have_connections() == false) {
                    //erro blocos de fim
                    al_show_native_message_box(display, "Fluxprog", "ERRO", "Todos os blocos devem estar interligados.", "Ok", ALLEGRO_MESSAGEBOX_WARN);
                    executing_fluxogram = false;
                }
                //teste se todos os blocos estão com as funções setadas
                if(check_if_all_blocks_have_functions_or_sensors() == false) {
                    //erro blocos de fim
                    al_show_native_message_box(display, "Fluxprog", "ERRO", "Todos os blocos devem estar com as suas funções ou sensores determinados.", "Ok", ALLEGRO_MESSAGEBOX_WARN);
                    executing_fluxogram = false;
                }
                //reseta as variáveis do loops para a execução
                for(int i=0; i<valor_maximo_blocos; i++) {
                    if(blocks_list_to_print[i] != NULL) {
                        if(blocks_list_to_print[i]->getType() == 7) {
                            blocks_list_to_print[i]->reset_loop_variables();
                        }
                    }
                }
                reset_fluxogram_execution();
            }
            if(menu_selected == 2) {
                //cout<<"pause"<<endl;
                executing_fluxogram = false;
            }
            if(menu_selected == 3) {
                //cout<<"stop"<<endl;
                executing_fluxogram = false;
                reset_fluxogram_execution();
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
                //cout<<"bluetooth"<<endl;
                connect_robot();
                connect();
            }
            if(menu_selected == 8) {
                //cout<<"vrep"<<endl;
                connect_simulator();
                connect();
            }
            if(menu_selected == 15) {
                black_sensor_menu_selected = true;
            }
            if(menu_selected == 16) {
                color_sensor_menu_selected = true;
            }
            if(menu_selected == 17) {
                ultrasonic_sensor_menu_selected = true;
            }
            if(menu_selected == 22) {
                cout<<"verdadeiro"<<endl;
            }
            if(menu_selected == 23) {
                cout<<"falso"<<endl;
            }
            //testa se soltou o bloco na lixeira
            for(int i=0; i<valor_maximo_blocos; i++) {
                if(blocks_list_to_print[i] != NULL) {
                    if(blocks_list_to_print[i]->getDragging() == true) {
                        if((mouseX > 0) && (mouseX < 70) && (mouseY > (al_get_display_height(display)-70)) && (mouseY < 0, al_get_display_height(display))) {
                            remove_block(blocks_list_to_print[i]);
                        } else {
                            blocks_list_to_print[i]->setDragging(false);
                        }
                    }
                }
            }
            //checa se soltou alguma funçao ou sensor
            check_dragging();

            reset_dragging_variables();

        }

        if(events.type == ALLEGRO_EVENT_KEY_DOWN) {

            //cout<<"tecla apertada: " << events.keyboard.keycode <<endl;
            if((events.keyboard.keycode == 77) || (events.keyboard.keycode == 90)){
                if(drawing_line == false) {
                    for(int i=0; i<valor_maximo_blocos; i++) {
                        if(blocks_list_to_print[i] != NULL) {
                            if(blocks_list_to_print[i]->getSelected() == true) {
                                remove_block(blocks_list_to_print[i]);
                            }
                        }
                    }
                }
                delete_connections();
            }
        }

        while(check_colisions());
    }
}

void Interface :: load_bitmap(ALLEGRO_BITMAP **bitmap, char *adress) {
    //se não achar a imagem no diretorio especificado dá msg de erro e para a execução
    if(!al_load_bitmap(adress)) {
        char* str1 = "Não encontrou imagem: ";
        char * str = (char *) malloc(1 + strlen(str1)+ strlen(adress));
        strcpy(str, str1);
        strcat(str, adress);
        al_show_native_message_box(display, "Fluxprog", "ERRO", str, "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        executing = false;
    } else {
        *bitmap = al_load_bitmap(adress);
    }
}
void Interface :: add_block(Block *b) {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] == NULL) {
            blocks_list_to_print[i] = b;
            //cout<< "adicionou na lista" << endl;
            break;
        }
    }

}
void Interface :: remove_block(Block *b) {
    //percorre a lista de blocos em busca do bloco a ser excluido
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            //se o bloco tiver relação com algum elimina essa relação
            if(blocks_list_to_print[i]->getNext1() == b) {
                blocks_list_to_print[i]->setNext1(NULL);
                //cout<<"exclui ligação next1"<<endl;
            }
            if(blocks_list_to_print[i]->getNext2() == b) {
                blocks_list_to_print[i]->setNext2(NULL);
                //cout<<"exclui ligação next2"<<endl;
            }
            if(blocks_list_to_print[i]->getPrevious1() == b) {
                blocks_list_to_print[i]->setPrevious1(NULL);
                //cout<<"exclui ligação previous1"<<endl;
            }
            if(blocks_list_to_print[i]->getPrevious2() == b) {
                blocks_list_to_print[i]->setPrevious2(NULL);
                //cout<<"exclui ligação previous2"<<endl;
            }
        }
    }
    //cout<<"removeu ligações"<<endl;
    //percorre a lista de blocos em busca do bloco a ser excluido
    for(int i=0; i<valor_maximo_blocos; i++) {
        //encontra o bloco passado como parametro
        if(blocks_list_to_print[i] == b) {
            blocks_list_to_print[i] = NULL;
            delete b;
            //cout<< "removeu da lista: " << i << endl;
            break;
        }
    }
}
void Interface :: print_primary_menu() {
    //desenha o retangulo no qual ficam os botões de opções
    al_draw_filled_rectangle(0, 0, al_get_display_width(display), (4 + al_get_bitmap_height(play_button)), primary_menu_color);

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

}
void Interface :: print_secondary_menu() {

    int blocks_menu_Y = (al_get_bitmap_height(play_button)+4);
    int sensors_menu_Y = blocks_menu_Y + 15 + 6*al_get_bitmap_height(mini_menu[0]);
    int actions_menu_Y = sensors_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION)+20;
    int extra_menu_Y = actions_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION)+20;

    //desenha o retangulo no qual ficam os botões de opções
    al_draw_filled_rectangle(0, blocks_menu_Y, al_get_bitmap_width(mini_menu[0])+5, blocks_menu_Y + 6*al_get_bitmap_height(mini_menu[0])+5, primary_menu_color);
    al_draw_filled_rectangle(0, sensors_menu_Y, al_get_bitmap_width(mini_menu[0])+5, sensors_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10, sensors_menu_color);
    al_draw_filled_rectangle(0, actions_menu_Y, al_get_bitmap_width(mini_menu[0])+5, actions_menu_Y + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION)+10, functions_menu_color);


    //desenha todos os botões que criam blocos
    for (int i = 0; i < 6; i++) {
        al_draw_bitmap(mini_menu[i], 0, blocks_menu_Y + i * al_get_bitmap_height(mini_menu[i]), 0);
    }
    //desenha botoes de sensores
    al_draw_bitmap(BLACK_SENSOR_FUNCTION, 0, sensors_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(COLOR_SENSOR_FUNCTION, 0, sensors_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(ULTRASONIC_SENSOR_FUNCTION, 0, sensors_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    //desenha botoes de acao
    al_draw_bitmap(WALK_FOWARD_ACTION, 0, actions_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(TURN_LEFT_ACTION, 0, actions_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(TURN_RIGHT_ACTION, 0, actions_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    //desenha botoes extras
    al_draw_bitmap(logic_true, 0, extra_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(logic_false, 0, extra_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);
    al_draw_bitmap(NUMBER[2], 0, extra_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION) + 5, 0);

    //variaveis usadas para facilitar a impressao das imagens
    int black_sensor_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int black_sensor_submenu_Y = sensors_menu_Y + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;
    int color_sensor_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int color_sensor_submenu_Y = sensors_menu_Y + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;
    int ultrasonic_sensor_submenu_X = al_get_bitmap_width(mini_menu[0]) + 5;
    int ultrasonic_sensor_submenu_Y = sensors_menu_Y + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION)+5;

    //imprime submenus
    if(black_sensor_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(BLACK_SENSOR_1_FUNCTION, black_sensor_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        al_draw_bitmap(BLACK_SENSOR_2_FUNCTION, black_sensor_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        al_draw_bitmap(BLACK_SENSOR_3_FUNCTION, black_sensor_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        al_draw_bitmap(BLACK_SENSOR_4_FUNCTION, black_sensor_submenu_X + 3 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        al_draw_bitmap(BLACK_SENSOR_5_FUNCTION, black_sensor_submenu_X + 4 * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, 0);
        for(int i=0; i<5; i++) {
            // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > black_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < black_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > black_sensor_submenu_Y) && (mouseY < black_sensor_submenu_Y+5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(black_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y, black_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), black_sensor_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 24 + i;
                }
            }
        }
    }

    if(color_sensor_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(COLOR_SENSOR_1_FUNCTION, color_sensor_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), color_sensor_submenu_Y, 0);
        al_draw_bitmap(COLOR_SENSOR_2_FUNCTION, color_sensor_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), color_sensor_submenu_Y, 0);
        for(int i=0; i<2; i++) {
            // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > color_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < color_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > color_sensor_submenu_Y) && (mouseY < color_sensor_submenu_Y+5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(color_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), color_sensor_submenu_Y, color_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), color_sensor_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 29 + i;
                }
            }
        }
    }

    if(ultrasonic_sensor_menu_selected == true) {
        sub_menu = true;
        al_draw_bitmap(ULTRASONIC_SENSOR_1_FUNCTION, ultrasonic_sensor_submenu_X + 0 * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y, 0);
        al_draw_bitmap(ULTRASONIC_SENSOR_2_FUNCTION, ultrasonic_sensor_submenu_X + 1 * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y, 0);
        al_draw_bitmap(ULTRASONIC_SENSOR_3_FUNCTION, ultrasonic_sensor_submenu_X + 2 * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y, 0);
        for(int i=0; i<3; i++) {
            // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
            if((mouseX > ultrasonic_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION)) && (mouseX < ultrasonic_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION))) {
                if((mouseY > ultrasonic_sensor_submenu_Y) && (mouseY < ultrasonic_sensor_submenu_Y+5 + al_get_bitmap_height(mini_menu[0]))) {
                    al_draw_rectangle(ultrasonic_sensor_submenu_X + i * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y, ultrasonic_sensor_submenu_X + (i+1) * al_get_bitmap_height(WALK_FOWARD_ACTION), ultrasonic_sensor_submenu_Y + al_get_bitmap_height(mini_menu[0]), black, 3);
                    menu_selected = 31 + i;
                }
            }
        }
    }

    for(int i=0; i<6; i++) {
        // checa se o mouse está sobre o menu de blocos e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 4 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 4 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 4 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+4 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i;
            }
        }
    }

    for(int i=6; i<9; i++) {
        // checa se o mouse está sobre o menu de sensores e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 24 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 24 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 24 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+ 24 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i;
            }
        }
    }

    for(int i=9; i<12; i++) {
        // checa se o mouse está sobre o menu e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 44 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 44 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 44 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+ 44 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i;
            }
        }
    }

    for(int i=12; i<15; i++) {
        // checa se o mouse está sobre o menu e desenha um retangulo selecionando o botao que o mouse está sobre
        if((mouseX > 0) && (mouseX < al_get_bitmap_width(WALK_FOWARD_ACTION))) {
            if((mouseY > al_get_bitmap_height(play_button) + 64 + i*al_get_bitmap_height(mini_menu[0])) && (mouseY < al_get_bitmap_height(play_button) + 64 + (i+1)*al_get_bitmap_height(mini_menu[0]))) {
                al_draw_rectangle(0, al_get_bitmap_height(play_button) + 64 + i*al_get_bitmap_height(mini_menu[0]), al_get_bitmap_width(mini_menu[0]), al_get_bitmap_height(play_button)+ 64 + (i+1)*al_get_bitmap_height(mini_menu[0]), black, 3);
                menu_selected = 9 + i;
            }
        }
    }

    al_draw_bitmap(trash, 0, al_get_display_height(display)-70, 0);
}
void Interface :: print_function_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        al_draw_bitmap(FUNCTION_BLOCK[1], b->getX(), b->getY(), 0);

    } else if(b->getSelected() == true) {
        al_draw_bitmap(FUNCTION_BLOCK[2], b->getX(), b->getY(), 0);
    } else if(b->getExecuting() == true) {
        al_draw_bitmap(FUNCTION_BLOCK[3], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(FUNCTION_BLOCK[0], b->getX(), b->getY(), 0);
    }

    //testa qual o tipo de sensor da comparação para imprimir a imagem certa
    switch(b->getFunction()) {
        case 1:
            al_draw_bitmap(MICRO_WALK_FOWARD, b->getX()+32, b->getY()+7, 0);
            break;
        case 2:
            al_draw_bitmap(MICRO_TURN_LEFT, b->getX()+32, b->getY()+7, 0);
            break;
        case 3:
            al_draw_bitmap(MICRO_TURN_RIGHT, b->getX()+32, b->getY()+7, 0);
            break;
    }

    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+40) && (mouseX < (b->getX() + 53)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[1], b->getX()+40, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+40, b->getY()+35, 0);
    } else if((mouseX > b->getX()+40) && (mouseX < (b->getX() + 53)) && (mouseY > b->getY()+35) && (mouseY < (b->getY()+48))) {
        al_draw_bitmap(POINT[0], b->getX()+40, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+40, b->getY()+35, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+40, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+40, b->getY()+35, 0);
    }
}
void Interface :: print_end_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        al_draw_bitmap(END_BLOCK[1], b->getX(), b->getY(), 0);
    } else if(b->getSelected() == true) {
        al_draw_bitmap(END_BLOCK[2], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(END_BLOCK[0], b->getX(), b->getY(), 0);
    }
    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+37) && (mouseX < (b->getX() + 50)) && (mouseY > b->getY()-5) && (mouseY < (b->getY() + 8))) {
        al_draw_bitmap(POINT[1], b->getX()+37, b->getY()-5, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+37, b->getY()-5, 0);
    }
}
void Interface :: print_start_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        al_draw_bitmap(START_BLOCK[1], b->getX(), b->getY(), 0);
    } else if(b->getSelected() == true) {
        al_draw_bitmap(START_BLOCK[2], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(START_BLOCK[0], b->getX(), b->getY(), 0);
    }
    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+37) && (mouseX < (b->getX() + 50)) && (mouseY > b->getY()+27) && (mouseY < (b->getY() + 40))) {
        al_draw_bitmap(POINT[1], b->getX()+37, b->getY()+27, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+37, b->getY()+27, 0);
    }
}
void Interface :: print_loop_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        al_draw_bitmap(LOOP_BLOCK[1], b->getX(), b->getY(), 0);
    } else if(b->getSelected() == true) {
        al_draw_bitmap(LOOP_BLOCK[2], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(LOOP_BLOCK[0], b->getX(), b->getY(), 0);
    }
    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+35) && (mouseX < (b->getX() + 48)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[1], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()+81, 0);
    } else if((mouseX > b->getX()+35) && (mouseX < (b->getX() + 48)) && (mouseY > b->getY()+81) && (mouseY < (b->getY()+94))) {
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()+81, 0);
    } else if((mouseX > b->getX()+72) && (mouseX < (b->getX() + 85)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[1], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()+81, 0);
    } else if((mouseX > b->getX()+72) && (mouseX < (b->getX() + 85)) && (mouseY > b->getY()+81) && (mouseY < (b->getY()+94))) {
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+72, b->getY()+81, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+35, b->getY()+81, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+72, b->getY()+81, 0);
    }
}
void Interface :: print_decision_block(Block *b) {

    //desenha o bloco
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        al_draw_bitmap(DECISION_BLOCK[1], b->getX(), b->getY(), 0);
    } else if(b->getSelected() == true) {
        al_draw_bitmap(DECISION_BLOCK[2], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(DECISION_BLOCK[0], b->getX(), b->getY(), 0);
    }
    //testa qual o tipo de sensor da comparação para imprimir a imagem certa
    switch(b->getTypeOfSensor()) {
        case 1:
            al_draw_bitmap(MICRO_BLACK_SENSOR1, b->getX()+47, b->getY()+19, 0);
            break;

        case 2:
            al_draw_bitmap(MICRO_BLACK_SENSOR2, b->getX()+47, b->getY()+19, 0);
            break;
        case 3:
            al_draw_bitmap(MICRO_BLACK_SENSOR3, b->getX()+47, b->getY()+19, 0);
            break;
        case 4:
            al_draw_bitmap(MICRO_BLACK_SENSOR4, b->getX()+47, b->getY()+19, 0);
            break;
        case 5:
            al_draw_bitmap(MICRO_BLACK_SENSOR5, b->getX()+47, b->getY()+19, 0);
            break;
        case 6:
            al_draw_bitmap(MICRO_COLOR_SENSOR1, b->getX()+47, b->getY()+19, 0);
            break;
        case 7:
            al_draw_bitmap(MICRO_COLOR_SENSOR2, b->getX()+47, b->getY()+19, 0);
            break;
        case 8:
            al_draw_bitmap(MICRO_ULTRASONIC_SENSOR1, b->getX()+47, b->getY()+19, 0);
            break;
        case 9:
            al_draw_bitmap(MICRO_ULTRASONIC_SENSOR2, b->getX()+47, b->getY()+19, 0);
            break;
        case 10:
            al_draw_bitmap(MICRO_ULTRASONIC_SENSOR3, b->getX()+47, b->getY()+19, 0);
            break;
    }

    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()+53) && (mouseX < (b->getX() + 66)) && (mouseY > b->getY()-6) && (mouseY < (b->getY()+7))) {
        al_draw_bitmap(POINT[1], b->getX()+53, b->getY()-6, 0);
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()+62, 0);
        al_draw_bitmap(POINT[0], b->getX()+112, b->getY()+28, 0);
    } else if((mouseX > b->getX()+53) && (mouseX < (b->getX() + 66)) && (mouseY > b->getY()+62) && (mouseY < (b->getY()+75))) {
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()-6, 0);
        al_draw_bitmap(POINT[1], b->getX()+53, b->getY()+62, 0);
        al_draw_bitmap(POINT[0], b->getX()+112, b->getY()+28, 0);
    } else if((mouseX > b->getX()+112) && (mouseX < (b->getX() + 125)) && (mouseY > b->getY()+28) && (mouseY < (b->getY()+41))) {
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()-6, 0);
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()+62, 0);
        al_draw_bitmap(POINT[1], b->getX()+112, b->getY()+28, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()-6, 0);
        al_draw_bitmap(POINT[0], b->getX()+53, b->getY()+62, 0);
        al_draw_bitmap(POINT[0], b->getX()+112, b->getY()+28, 0);
    }
}
void Interface :: print_merge_block(Block *b) {
    if((mouseX > b->getX()) && (mouseX < (b->getX() + b->getWidth())) && (mouseY > b->getY()) && (mouseY < (b->getY() + b->getHeight()))) {
        al_draw_bitmap(MERGE_BLOCK[1], b->getX(), b->getY(), 0);
    } else if(b->getSelected() == true) {
        al_draw_bitmap(MERGE_BLOCK[2], b->getX(), b->getY(), 0);
    } else {
        al_draw_bitmap(MERGE_BLOCK[0], b->getX(), b->getY(), 0);
    }
    //desenha as bolinhas onde são ligadas as linhas
    if((mouseX > b->getX()-5) && (mouseX < (b->getX() + 8)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[1], b->getX()-5, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+22, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+8, b->getY()+17, 0);
    } else if((mouseX > b->getX()+22) && (mouseX < (b->getX() + 35)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
        al_draw_bitmap(POINT[0], b->getX()-5, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+22, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+8, b->getY()+17, 0);
    } else if((mouseX > b->getX()+8) && (mouseX < (b->getX() + 21)) && (mouseY > b->getY()+17) && (mouseY < (b->getY()+30))) {
        al_draw_bitmap(POINT[0], b->getX()-5, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+22, b->getY()-5, 0);
        al_draw_bitmap(POINT[1], b->getX()+8, b->getY()+17, 0);
    } else {
        al_draw_bitmap(POINT[0], b->getX()-5, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+22, b->getY()-5, 0);
        al_draw_bitmap(POINT[0], b->getX()+8, b->getY()+17, 0);
    }
}
void Interface :: load_program_images() {
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
    load_bitmap(&WALK_FOWARD_ACTION, "images/functions/mini_action_walk_foward.png");
    load_bitmap(&MICRO_WALK_FOWARD, "images/functions/micro_action_walk_foward.png");
    load_bitmap(&TURN_LEFT_ACTION, "images/functions/mini_action_turn_left.png");
    load_bitmap(&MICRO_TURN_LEFT, "images/functions/micro_action_turn_left.png");
    load_bitmap(&TURN_RIGHT_ACTION, "images/functions/mini_action_turn_right.png");
    load_bitmap(&MICRO_TURN_RIGHT, "images/functions/micro_action_turn_right.png");
    load_bitmap(&logic_true, "images/functions/mini_logic_true.png");
    load_bitmap(&logic_false, "images/functions/mini_logic_false.png");
    load_bitmap(&POINT[0], "images/point.png");
    load_bitmap(&POINT[1], "images/hpoint.png");
    load_bitmap(&arrow, "images/arrow.png");

    load_bitmap(&DECISION_BLOCK[0], "images/blocks/decision_block/decision_block.png");
    load_bitmap(&DECISION_BLOCK[1], "images/blocks/decision_block/decision_block_mouse.png");
    load_bitmap(&DECISION_BLOCK[2], "images/blocks/decision_block/decision_block_selected.png");
    load_bitmap(&DECISION_BLOCK[3], "images/blocks/decision_block/decision_block_executing.png");
    load_bitmap(&END_BLOCK[0], "images/blocks/end_block/end_block.png");
    load_bitmap(&END_BLOCK[1], "images/blocks/end_block/end_block_mouse.png");
    load_bitmap(&END_BLOCK[2], "images/blocks/end_block/end_block_selected.png");
    load_bitmap(&END_BLOCK[3], "images/blocks/end_block/end_block_executing.png");
    load_bitmap(&FUNCTION_BLOCK[0], "images/blocks/function_block/function_block.png");
    load_bitmap(&FUNCTION_BLOCK[1], "images/blocks/function_block/function_block_mouse.png");
    load_bitmap(&FUNCTION_BLOCK[2], "images/blocks/function_block/function_block_selected.png");
    load_bitmap(&FUNCTION_BLOCK[3], "images/blocks/function_block/function_block_executing.png");
    load_bitmap(&LOOP_BLOCK[0], "images/blocks/loop_block/loop_block2.png");
    load_bitmap(&LOOP_BLOCK[1], "images/blocks/loop_block/loop_block2_mouse.png");
    load_bitmap(&LOOP_BLOCK[2], "images/blocks/loop_block/loop_block2_selected.png");
    load_bitmap(&LOOP_BLOCK[3], "images/blocks/loop_block/loop_block2_executing.png");
    load_bitmap(&MERGE_BLOCK[0], "images/blocks/merge_block/merge_block.png");
    load_bitmap(&MERGE_BLOCK[1], "images/blocks/merge_block/merge_block_mouse.png");
    load_bitmap(&MERGE_BLOCK[2], "images/blocks/merge_block/merge_block_selected.png");
    load_bitmap(&MERGE_BLOCK[3], "images/blocks/merge_block/merge_block_executing.png");
    load_bitmap(&START_BLOCK[0], "images/blocks/start_block/start_block.png");
    load_bitmap(&START_BLOCK[1], "images/blocks/start_block/start_block_mouse.png");
    load_bitmap(&START_BLOCK[2], "images/blocks/start_block/start_block_selected.png");
    load_bitmap(&START_BLOCK[3], "images/blocks/start_block/start_block_executing.png");
    load_bitmap(&BLACK_SENSOR_FUNCTION, "images/blocks/black_sensor_block/mini_black_sensor.png");
    load_bitmap(&BLACK_SENSOR_1_FUNCTION, "images/blocks/black_sensor_block/mini_black_sensor1.png");
    load_bitmap(&BLACK_SENSOR_2_FUNCTION, "images/blocks/black_sensor_block/mini_black_sensor2.png");
    load_bitmap(&BLACK_SENSOR_3_FUNCTION, "images/blocks/black_sensor_block/mini_black_sensor3.png");
    load_bitmap(&BLACK_SENSOR_4_FUNCTION, "images/blocks/black_sensor_block/mini_black_sensor4.png");
    load_bitmap(&BLACK_SENSOR_5_FUNCTION, "images/blocks/black_sensor_block/mini_black_sensor5.png");
    load_bitmap(&MICRO_BLACK_SENSOR1, "images/blocks/black_sensor_block/micro_black_sensor1.png");
    load_bitmap(&MICRO_BLACK_SENSOR2, "images/blocks/black_sensor_block/micro_black_sensor2.png");
    load_bitmap(&MICRO_BLACK_SENSOR3, "images/blocks/black_sensor_block/micro_black_sensor3.png");
    load_bitmap(&MICRO_BLACK_SENSOR4, "images/blocks/black_sensor_block/micro_black_sensor4.png");
    load_bitmap(&MICRO_BLACK_SENSOR5, "images/blocks/black_sensor_block/micro_black_sensor5.png");
    load_bitmap(&COLOR_SENSOR_FUNCTION, "images/blocks/color_sensor_block/mini_color_sensor.png");
    load_bitmap(&COLOR_SENSOR_1_FUNCTION, "images/blocks/color_sensor_block/mini_color_sensor1.png");
    load_bitmap(&COLOR_SENSOR_2_FUNCTION, "images/blocks/color_sensor_block/mini_color_sensor2.png");
    load_bitmap(&MICRO_COLOR_SENSOR1, "images/blocks/color_sensor_block/micro_color_sensor1.png");
    load_bitmap(&MICRO_COLOR_SENSOR2, "images/blocks/color_sensor_block/micro_color_sensor2.png");
    load_bitmap(&ULTRASONIC_SENSOR_FUNCTION, "images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor.png");
    load_bitmap(&ULTRASONIC_SENSOR_1_FUNCTION, "images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor1.png");
    load_bitmap(&ULTRASONIC_SENSOR_2_FUNCTION, "images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor2.png");
    load_bitmap(&ULTRASONIC_SENSOR_3_FUNCTION, "images/blocks/ultrasonic_sensor_block/mini_ultrasonic_sensor3.png");
    load_bitmap(&MICRO_ULTRASONIC_SENSOR1, "images/blocks/ultrasonic_sensor_block/micro_ultrasonic_sensor1.png");
    load_bitmap(&MICRO_ULTRASONIC_SENSOR2, "images/blocks/ultrasonic_sensor_block/micro_ultrasonic_sensor2.png");
    load_bitmap(&MICRO_ULTRASONIC_SENSOR3, "images/blocks/ultrasonic_sensor_block/micro_ultrasonic_sensor3.png");

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

    load_bitmap(&trash, "images/trash_icon.png");
}
void Interface :: destroy_program_images() {
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
    al_destroy_bitmap(WALK_FOWARD_ACTION);
    al_destroy_bitmap(MICRO_WALK_FOWARD);
    al_destroy_bitmap(TURN_LEFT_ACTION);
    al_destroy_bitmap(MICRO_TURN_LEFT);
    al_destroy_bitmap(TURN_RIGHT_ACTION);
    al_destroy_bitmap(MICRO_TURN_RIGHT);
    al_destroy_bitmap(logic_true);
    al_destroy_bitmap(logic_false);
    al_destroy_bitmap(POINT[0]);
	al_destroy_bitmap(POINT[1]);
    al_destroy_bitmap(arrow);
    al_destroy_bitmap(BLACK_SENSOR_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_1_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_2_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_3_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_4_FUNCTION);
    al_destroy_bitmap(BLACK_SENSOR_5_FUNCTION);
    al_destroy_bitmap(MICRO_BLACK_SENSOR1);
    al_destroy_bitmap(MICRO_BLACK_SENSOR2);
    al_destroy_bitmap(MICRO_BLACK_SENSOR3);
    al_destroy_bitmap(MICRO_BLACK_SENSOR4);
    al_destroy_bitmap(MICRO_BLACK_SENSOR5);
    al_destroy_bitmap(COLOR_SENSOR_FUNCTION);
    al_destroy_bitmap(COLOR_SENSOR_1_FUNCTION);
    al_destroy_bitmap(COLOR_SENSOR_2_FUNCTION);
    al_destroy_bitmap(MICRO_COLOR_SENSOR1);
    al_destroy_bitmap(MICRO_COLOR_SENSOR2);
    al_destroy_bitmap(ULTRASONIC_SENSOR_FUNCTION);
    al_destroy_bitmap(ULTRASONIC_SENSOR_1_FUNCTION);
    al_destroy_bitmap(ULTRASONIC_SENSOR_2_FUNCTION);
    al_destroy_bitmap(ULTRASONIC_SENSOR_3_FUNCTION);
    al_destroy_bitmap(MICRO_ULTRASONIC_SENSOR1);
    al_destroy_bitmap(MICRO_ULTRASONIC_SENSOR2);
    al_destroy_bitmap(MICRO_ULTRASONIC_SENSOR3);

    al_destroy_bitmap(trash);
    for(int i=0; i<3; i++) {
        al_destroy_bitmap(DECISION_BLOCK[i]);
    }
    for(int i=0; i<3; i++) {
        al_destroy_bitmap(END_BLOCK[i]);
    }
    for(int i=0; i<3; i++) {
        al_destroy_bitmap(FUNCTION_BLOCK[i]);
    }
    for(int i=0; i<3; i++) {
        al_destroy_bitmap(LOOP_BLOCK[i]);
    }
    for(int i=0; i<3; i++) {
        al_destroy_bitmap(MERGE_BLOCK[i]);
    }
    for(int i=0; i<3; i++) {
        al_destroy_bitmap(START_BLOCK[i]);
    }
    for(int i=0; i<10; i++) {
        al_destroy_bitmap(NUMBER[i]);
    }
}
void Interface :: reset_dragging_variables() {
    dragging_black_sensor1 = false;
    dragging_black_sensor2 = false;
    dragging_black_sensor3 = false;
    dragging_black_sensor4 = false;
    dragging_black_sensor5 = false;
    dragging_color_sensor1 = false;
    dragging_color_sensor2 = false;
    dragging_ultrasonic_sensor1 = false;
    dragging_ultrasonic_sensor2 = false;
    dragging_ultrasonic_sensor3 = false;
    dragging_walk_foward = false;
    dragging_turn_left = false;
    dragging_turn_right = false;
}
void Interface :: check_dragging() {

    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == 8) {
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //cout<<"soltou sobre o bloco certo"<<endl;
                    if(dragging_black_sensor1 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(1);
                    } else if(dragging_black_sensor2 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(2);
                    } else if(dragging_black_sensor3 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(3);
                    } else if(dragging_black_sensor4 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(4);
                    } else if(dragging_black_sensor5 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(5);
                    } else if(dragging_color_sensor1 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(6);
                    } else if(dragging_color_sensor2 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(7);
                    } else if(dragging_ultrasonic_sensor1 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(8);
                    } else if(dragging_ultrasonic_sensor2 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(9);
                    } else if(dragging_ultrasonic_sensor3 == true) {
                        blocks_list_to_print[i]->setTypeOfSensor(10);
                    } else if((dragging_walk_foward == true) || (dragging_turn_left == true) || (dragging_turn_right == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "Essa ação não pode ser usado como entrada no bloco de decisão", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    }
                }
            }
            if(blocks_list_to_print[i]->getType() == 1) {
                if((mouseX > blocks_list_to_print[i]->getX()) && (mouseX < (blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth())) && (mouseY > blocks_list_to_print[i]->getY()) && (mouseY < (blocks_list_to_print[i]->getY() + blocks_list_to_print[i]->getHeight()))) {
                    //cout<<"soltou sobre o bloco certo"<<endl;
                    if((dragging_black_sensor1 == true) || (dragging_black_sensor2 == true) || (dragging_black_sensor3 == true)  || (dragging_black_sensor4 == true) || (dragging_black_sensor5 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de fita preta não pode ser usado como entrada no bloco de função", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    }  else if((dragging_color_sensor1 == true) || (dragging_color_sensor2 == true)){
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de cor não pode ser usado como entrada no bloco de função", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if((dragging_ultrasonic_sensor1 == true) || (dragging_ultrasonic_sensor2 == true) || (dragging_ultrasonic_sensor3 == true)) {
                        //erro
                        al_show_native_message_box(display, "Fluxprog", "ERRO", "O sensor de ultrassom não pode ser usado como entrada no bloco de função", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
                    } else if(dragging_walk_foward == true) {
                        blocks_list_to_print[i]->setFunction(1);
                    } else if(dragging_turn_left == true) {
                        blocks_list_to_print[i]->setFunction(2);
                    } else if(dragging_turn_right == true) {
                        blocks_list_to_print[i]->setFunction(3);
                    }
                }
            }
        }
    }
}
void Interface :: draw_dragging() {
    if(dragging_black_sensor1 == true) {
        al_draw_bitmap(BLACK_SENSOR_1_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_black_sensor2 == true) {
        al_draw_bitmap(BLACK_SENSOR_2_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_black_sensor3 == true) {
        al_draw_bitmap(BLACK_SENSOR_3_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_black_sensor4 == true) {
        al_draw_bitmap(BLACK_SENSOR_4_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_black_sensor5 == true) {
        al_draw_bitmap(BLACK_SENSOR_5_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_color_sensor1 == true) {
        al_draw_bitmap(COLOR_SENSOR_1_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_color_sensor2 == true) {
        al_draw_bitmap(COLOR_SENSOR_2_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_ultrasonic_sensor1 == true) {
        al_draw_bitmap(ULTRASONIC_SENSOR_1_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_ultrasonic_sensor2 == true) {
        al_draw_bitmap(ULTRASONIC_SENSOR_2_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_ultrasonic_sensor3 == true) {
        al_draw_bitmap(ULTRASONIC_SENSOR_3_FUNCTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_walk_foward == true) {
        al_draw_bitmap(WALK_FOWARD_ACTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_turn_left == true) {
        al_draw_bitmap(TURN_LEFT_ACTION, mouseX-20, mouseY-20, 0);
    }
    if(dragging_turn_right == true) {
        al_draw_bitmap(TURN_RIGHT_ACTION, mouseX-20, mouseY-20, 0);
    }
}
void Interface :: refresh_executing_block() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i] == current_executing_block) {
                blocks_list_to_print[i]->setExecuting(true);
            } else {
                blocks_list_to_print[i]->setExecuting(false);
            }
        }
    }
}
void Interface :: check_mouse_on_menus() {
    menu1_X_limit = 14 + 6*al_get_bitmap_width(play_button);
    menu1_Y_limit = 4 + al_get_bitmap_height(play_button);

    menu_connect_X_begin = al_get_display_width(display) - 73.5 - al_get_bitmap_width(bluetooth_button) - roll_bar_width;
    menu_connect_X_limit = al_get_display_width(display) - roll_bar_width;

    menu2_X_limit = al_get_bitmap_width(WALK_FOWARD_ACTION);

    menu_blocks_Y_begin = 4 + al_get_bitmap_height(play_button);
    menu_blocks_Y_limit = menu1_Y_limit + 6*al_get_bitmap_height(WALK_FOWARD_ACTION);

    menu_sensors_Y_begin = menu_blocks_Y_limit + 20;
    menu_sensors_Y_limit = menu_sensors_Y_begin + 3*al_get_bitmap_height(WALK_FOWARD_ACTION);

    menu_actions_Y_begin = menu_sensors_Y_limit + 20;
    menu_actions_Y_limit = menu_actions_Y_begin + 3*al_get_bitmap_height(WALK_FOWARD_ACTION);

    menu_extra_Y_begin = menu_actions_Y_limit + 20;
    menu_extra_Y_limit = menu_extra_Y_begin + 3*al_get_bitmap_height(WALK_FOWARD_ACTION);

    //se o mouse não está sobre os menus seta a variável de teste para zero
    if(mouseY < menu1_Y_limit) {
        if((mouseX > 14) && (mouseX < menu1_X_limit)) {
            //menu 1
        } else if((mouseX < menu_connect_X_limit) && (mouseX > menu_connect_X_begin)) {
            //menu conectar
        } else {
            menu_selected = 0;
        }
    } else if(mouseX < menu2_X_limit) {
        if((mouseY > menu_blocks_Y_begin) && (mouseY < menu_blocks_Y_limit)) {
            //menu de blocos
        } else if((mouseY > menu_sensors_Y_begin) && (mouseY < menu_sensors_Y_limit)) {
            //menu de sensores
        } else if((mouseY > menu_actions_Y_begin) && (mouseY < menu_actions_Y_limit)) {
            //menu de acoes
        } else if((mouseY > menu_extra_Y_begin) && (mouseY < menu_extra_Y_limit)) {
            //menu extra
        } else {
            menu_selected = 0;
        }
    } else if((black_sensor_menu_selected == true) && ((mouseX < 6*menu2_X_limit) && (mouseY > menu_sensors_Y_begin) && (mouseY < (menu_sensors_Y_begin+al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu sensor de fita
    } else if((color_sensor_menu_selected == true) && ((mouseX < 3*menu2_X_limit) && (mouseY > (menu_sensors_Y_begin+al_get_bitmap_width(WALK_FOWARD_ACTION))) && (mouseY < (menu_sensors_Y_begin+2*al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu sensor de cor
    } else if((ultrasonic_sensor_menu_selected == true) && ((mouseX < 4*menu2_X_limit) && (mouseY > (menu_sensors_Y_begin+2*al_get_bitmap_width(WALK_FOWARD_ACTION))) && (mouseY < (menu_sensors_Y_begin+3*al_get_bitmap_height(WALK_FOWARD_ACTION))))) {
        //submenu sensor de ultrassom
    } else {
        menu_selected = 0;
    }
}
void Interface :: print_list_of_blocks() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        //testa se na posição do array existe mesmo um bloco
        if(blocks_list_to_print[i] != NULL) {
            //check_colisions(blocks_list_to_print[i], i);
            /*
                tipo 1 = bloco de ação
                tipo 2 = bloco de sensor de ultrassom
                tipo 3 = bloco de sensor de fita preta
                tipo 4 = bloco de sensor de cor
                tipo 5 = bloco de fim
                tipo 6 = bloco de início
                tipo 7 = bloco de loop
                tipo 8 = bloco de decisão
                tipo 9 = bloco de junção
            */
            if(blocks_list_to_print[i]->getType() == 1) {
                print_function_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == 2) {

            }
            if(blocks_list_to_print[i]->getType() == 3) {

            }
            if(blocks_list_to_print[i]->getType() == 4) {

            }
            if(blocks_list_to_print[i]->getType() == 5) {
                print_end_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == 6) {
                print_start_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == 7) {
                print_loop_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == 8) {
                print_decision_block(blocks_list_to_print[i]);
            }
            if(blocks_list_to_print[i]->getType() == 9) {
                print_merge_block(blocks_list_to_print[i]);
            }
            //se o block estiver sendo arrastado, atualiza as cordenadas
            if(blocks_list_to_print[i]->getDragging() == true) {
                blocks_list_to_print[i]->setX(mouseX - mouse_aux_x);
                blocks_list_to_print[i]->setY(mouseY - mouse_aux_y);
            } else if(blocks_list_to_print[i]->getX() <= menu2_X_limit) {
                blocks_list_to_print[i]->setX(menu2_X_limit + 20);
            } else if(blocks_list_to_print[i]->getY() <= menu1_Y_limit) {
                blocks_list_to_print[i]->setY(menu1_Y_limit + 20);
            }
        }
    }
}
void Interface :: check_mouse_on_points(Block *b) {
    //função
    if(b->getType() == 1) {
        if((mouseX > b->getX()+40) && (mouseX < (b->getX() + 53)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            //cout<<"clicou na entrada do bloco de funcao"<<endl;
            b->setIn1Selected(true);
            b->setOut1Selected(false);
        } else if((mouseX > b->getX()+40) && (mouseX < (b->getX() + 53)) && (mouseY > b->getY()+35) && (mouseY < (b->getY()+48))) {
            //cout<<"clicou na saida do bloco de funcao"<<endl;
            b->setIn1Selected(false);
            b->setOut1Selected(true);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
        }
    }
    //fim
    if(b->getType() == 5) {
        if((mouseX > b->getX()+37) && (mouseX < (b->getX() + 50)) && (mouseY > b->getY()-5) && (mouseY < (b->getY() + 8))) {
            //cout<<"clicou na entrada do bloco de fim"<<endl;
            b->setIn1Selected(true);
        } else {
            b->setIn1Selected(false);
        }
    }
    //inicio
    if(b->getType() == 6) {
        if((mouseX > b->getX()+37) && (mouseX < (b->getX() + 50)) && (mouseY > b->getY()+27) && (mouseY < (b->getY() + 40))) {
            b->setOut1Selected(true);
        } else {
            b->setOut1Selected(false);
        }
    }
    //loop
    if(b->getType() == 7) {
        if((mouseX > b->getX()+35) && (mouseX < (b->getX() + 48)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+35) && (mouseX < (b->getX() + 48)) && (mouseY > b->getY()+81) && (mouseY < (b->getY()+94))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+72) && (mouseX < (b->getX() + 85)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(true);
        } else if((mouseX > b->getX()+72) && (mouseX < (b->getX() + 85)) && (mouseY > b->getY()+81) && (mouseY < (b->getY()+94))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(true);
            b->setOut2Selected(false);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
            b->setOut2Selected(false);
        }
    }
    //decisao
    if(b->getType() == 8) {

        if((mouseX > b->getX()+53) && (mouseX < (b->getX() + 66)) && (mouseY > b->getY()-6) && (mouseY < (b->getY()+7))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+53) && (mouseX < (b->getX() + 66)) && (mouseY > b->getY()+62) && (mouseY < (b->getY()+75))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
            b->setOut2Selected(false);
        } else if((mouseX > b->getX()+112) && (mouseX < (b->getX() + 125)) && (mouseY > b->getY()+28) && (mouseY < (b->getY()+41))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setOut2Selected(true);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setOut2Selected(false);
        }
    }
    //junção
    if(b->getType() == 9) {

        if((mouseX > b->getX()-5) && (mouseX < (b->getX() + 8)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            b->setIn1Selected(true);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
        } else if((mouseX > b->getX()+22) && (mouseX < (b->getX() + 35)) && (mouseY > b->getY()-5) && (mouseY < (b->getY()+8))) {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(true);
        } else if((mouseX > b->getX()+8) && (mouseX < (b->getX() + 21)) && (mouseY > b->getY()+17) && (mouseY < (b->getY()+30))) {
            b->setIn1Selected(false);
            b->setOut1Selected(true);
            b->setIn2Selected(false);
        } else {
            b->setIn1Selected(false);
            b->setOut1Selected(false);
            b->setIn2Selected(false);
        }
    }
}
void Interface :: draw_temporary_line() {
    if(drawing_line == true) {
        float angulo = -(atan2((mouseX - temporary_line_X), (mouseY - temporary_line_Y)));
        al_draw_line(temporary_line_X, temporary_line_Y, mouseX, mouseY, black, 2);
        al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, mouseX, mouseY, angulo, 0);
    }
}
void Interface :: delete_connections() {
    if(drawing_line == true) {
        if(blocks_list_to_print[block_selected] != NULL) {
            if(number_of_selected_out == 1) {
                int aux=0;
                //procura bloco que tem relação
                for(int i=0; i<valor_maximo_blocos; i++) {
                    if(blocks_list_to_print[i] != NULL) {
                        if(blocks_list_to_print[i] == blocks_list_to_print[block_selected]->getNext1()) {
                            //achou o bloco
                            aux = i;
                        }
                    }
                }
                blocks_list_to_print[block_selected]->setNext1(NULL);
                if(blocks_list_to_print[aux]->getPrevious1() == blocks_list_to_print[block_selected]) {
                    blocks_list_to_print[aux]->setPrevious1(NULL);
                } else {
                    blocks_list_to_print[aux]->setPrevious2(NULL);
                }
            } else {
                int aux=0;
                //procura bloco que tem relação
                for(int i=0; i<valor_maximo_blocos; i++) {
                    if(blocks_list_to_print[i] != NULL) {
                        if(blocks_list_to_print[i] == blocks_list_to_print[block_selected]->getNext2()) {
                            //achou o bloco
                            aux = i;
                        }
                    }
                }
                blocks_list_to_print[block_selected]->setNext2(NULL);
                if(blocks_list_to_print[aux]->getPrevious1() == blocks_list_to_print[block_selected]) {
                    blocks_list_to_print[aux]->setPrevious1(NULL);
                } else {
                    blocks_list_to_print[aux]->setPrevious2(NULL);
                }
            }
        }
        drawing_line = false;
    }
}
void Interface :: draw_everything() {
    //imprime menu
    print_primary_menu();
    //imprime menu de blocos
    print_secondary_menu();

    //checa se o mouse está sobre os menus, para setar a variável de controle do menu_selected
    check_mouse_on_menus();

    //percorre toda a lista de impressão dos blocos
    print_list_of_blocks();

    //desenha objetos sendo arrastados
    draw_dragging();
    //desenha linha de ligação
    draw_temporary_line();

    //desenha ligações entre blocos
    draw_lines();
}
bool Interface :: check_colisions() {
    int selected_block;
    //procura o bloco selecionado
    for(int i = 0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getSelected() == true) {
                //cout<<"encontrou o selecionado"<<endl;
                selected_block = i;
                break;
            } else {
                selected_block = -1;
            }
        }
    }
    //percorre todos os blocos
    for(int i=0; i<valor_maximo_blocos; i++) {
        //testa se tem algum selecionado
        if(selected_block != -1) {
            //testa se os blocos são válidos
            if((blocks_list_to_print[i] != NULL) && (blocks_list_to_print[selected_block] != NULL)) {
                //cout<<"passou teste de null"<<endl;
                //exclui o bloco selecionado do teste para n testar colisão dele com ele mesmo
                if(i != selected_block) {
                    //cout<<"passou teste de diferente"<<endl;
                    //teste se está sendo arrastado, pois o teste de colisão ocorre quando não está sendo arrastado
                    if(blocks_list_to_print[selected_block]->getDragging() == false) {
                        int selected_block_1_x = blocks_list_to_print[selected_block]->getX();
                        int selected_block_1_y = blocks_list_to_print[selected_block]->getY();
                        int selected_block_2_y = blocks_list_to_print[selected_block]->getY()+blocks_list_to_print[selected_block]->getHeight();

                        int selected_block_3_x = blocks_list_to_print[selected_block]->getX()+blocks_list_to_print[selected_block]->getWidth();
                        int selected_block_3_y = blocks_list_to_print[selected_block]->getY();
                        int selected_block_4_y = blocks_list_to_print[selected_block]->getY()+blocks_list_to_print[selected_block]->getHeight();

                        int begin_x = blocks_list_to_print[i]->getX();
                        int begin_y = blocks_list_to_print[i]->getY();
                        int limit_x = blocks_list_to_print[i]->getX()+blocks_list_to_print[i]->getWidth();
                        int limit_y = blocks_list_to_print[i]->getY()+blocks_list_to_print[i]->getHeight();
                        //casos 1 e 2
                        if(selected_block_1_x >= begin_x) {
                            if(selected_block_1_x < limit_x) {
                                //caso 1
                                if(selected_block_1_y >= begin_y) {
                                    if(selected_block_1_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 1"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 2
                                if(selected_block_2_y >= begin_y) {
                                    if(selected_block_2_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 2"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                            }
                        }
                        //casos 3 e 4
                        if(selected_block_3_x >= begin_x) {
                            if(selected_block_3_x < limit_x) {
                                //caso 3
                                if(selected_block_3_y >= begin_y) {
                                    if(selected_block_3_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 3"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 4
                                if(selected_block_4_y >= begin_y) {
                                    if(selected_block_4_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 4"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                            }
                        }
                        //caso 5, 6 e 7
                        if(selected_block_1_x < begin_x) {
                            if(selected_block_3_x > limit_x) {
                                //caso 5
                                if(selected_block_1_y < begin_y) {
                                    if(selected_block_2_y > limit_y) {
                                        //cout<<"realizou operação de colisão caso 5"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 6
                                if(selected_block_1_y > begin_y) {
                                    if(selected_block_1_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 6"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 7
                                if(selected_block_2_y > begin_y) {
                                    if(selected_block_2_y < limit_y) {
                                        //cout<<"realizou operação de colisão caso 7"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                            }
                        }
                        //caso 8 e 9
                        if(selected_block_1_y < begin_y) {
                            if(selected_block_2_y > limit_y) {
                                //caso 8
                                if(selected_block_1_x > begin_x) {
                                    if(selected_block_1_x < limit_x) {
                                        //cout<<"realizou operação de colisão caso 8"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                                //caso 9
                                if(selected_block_3_x > begin_x) {
                                    if(selected_block_3_x < limit_x) {
                                        //cout<<"realizou operação de colisão caso 9"<<endl;
                                        blocks_list_to_print[selected_block]->setX(blocks_list_to_print[i]->getX() + blocks_list_to_print[i]->getWidth() + 10);
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}
void Interface :: draw_lines() {
    bool special_case = false;

    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            //caso especial
            if(blocks_list_to_print[i]->getNext1() != NULL) {
                if(blocks_list_to_print[i]->getNext2() != NULL) {
                    if(blocks_list_to_print[i]->getNext1()->getPrevious1() != NULL) {
                        if(blocks_list_to_print[i]->getNext1()->getPrevious2() != NULL) {
                            if(blocks_list_to_print[i]->getNext2()->getPrevious1() != NULL) {
                                if(blocks_list_to_print[i]->getNext2()->getPrevious2() != NULL) {
                                    if(blocks_list_to_print[i]->getNext1()->getPrevious1() == blocks_list_to_print[i]) {
                                        if(blocks_list_to_print[i]->getNext1()->getPrevious2() == blocks_list_to_print[i]) {
                                            if(blocks_list_to_print[i]->getNext2()->getPrevious1() == blocks_list_to_print[i]) {
                                                if(blocks_list_to_print[i]->getNext2()->getPrevious2() == blocks_list_to_print[i]) {
                                                    int start_x = blocks_list_to_print[i]->getPointOut1X();
                                                    int start_y = blocks_list_to_print[i]->getPointOut1Y();
                                                    int end_x = blocks_list_to_print[i]->getNext1()->getPointIn1X();
                                                    int end_y = blocks_list_to_print[i]->getNext1()->getPointIn1Y();
                                                    float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                                                    al_draw_line(start_x, start_y, end_x, end_y, black, 2);
                                                    al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                                                    start_x = blocks_list_to_print[i]->getPointOut2X();
                                                    start_y = blocks_list_to_print[i]->getPointOut2Y();
                                                    end_x = blocks_list_to_print[i]->getNext2()->getPointIn2X();
                                                    end_y = blocks_list_to_print[i]->getNext2()->getPointIn2Y();
                                                    angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                                                    al_draw_line(start_x, start_y, end_x, end_y, black, 2);
                                                    al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                                                    special_case = true;
                                                }
                                            }
                                        } else {
                                            special_case = false;
                                        }
                                    } else {
                                        special_case = false;
                                    }
                                } else {
                                    special_case = false;
                                }
                            } else {
                                special_case = false;
                            }
                        } else {
                            special_case = false;
                        }
                    } else {
                        special_case = false;
                    }
                } else {
                    special_case = false;
                }
            } else {
                special_case = false;
            }
            if(special_case == false) {
                if(blocks_list_to_print[i]->getNext1() != NULL) {
                    if(blocks_list_to_print[i]->getNext1()->getPrevious1() != NULL) {
                        if(blocks_list_to_print[i]->getNext1()->getPrevious1() == blocks_list_to_print[i]) {
                            int start_x = blocks_list_to_print[i]->getPointOut1X();
                            int start_y = blocks_list_to_print[i]->getPointOut1Y();
                            int end_x = blocks_list_to_print[i]->getNext1()->getPointIn1X();
                            int end_y = blocks_list_to_print[i]->getNext1()->getPointIn1Y();
                            float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                            al_draw_line(start_x, start_y, end_x, end_y, black, 2);
                            al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                        }
                    }
                    if(blocks_list_to_print[i]->getNext1()->getPrevious2() != NULL) {
                        if(blocks_list_to_print[i]->getNext1()->getPrevious2() == blocks_list_to_print[i]) {
                            int start_x = blocks_list_to_print[i]->getPointOut1X();
                            int start_y = blocks_list_to_print[i]->getPointOut1Y();
                            int end_x = blocks_list_to_print[i]->getNext1()->getPointIn2X();
                            int end_y = blocks_list_to_print[i]->getNext1()->getPointIn2Y();
                            float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                            al_draw_line(start_x, start_y, end_x, end_y, black, 2);
                            al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                        }
                    }
                }
                if(blocks_list_to_print[i]->getNext2() != NULL) {
                    if(blocks_list_to_print[i]->getNext2()->getPrevious1() != NULL) {
                        if(blocks_list_to_print[i]->getNext2()->getPrevious1() == blocks_list_to_print[i]) {
                            int start_x = blocks_list_to_print[i]->getPointOut2X();
                            int start_y = blocks_list_to_print[i]->getPointOut2Y();
                            int end_x = blocks_list_to_print[i]->getNext2()->getPointIn1X();
                            int end_y = blocks_list_to_print[i]->getNext2()->getPointIn1Y();
                            float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                            al_draw_line(start_x, start_y, end_x, end_y, black, 2);
                            al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                        }
                    }
                    if(blocks_list_to_print[i]->getNext2()->getPrevious2() != NULL) {
                        if(blocks_list_to_print[i]->getNext2()->getPrevious2() == blocks_list_to_print[i]) {
                            int start_x = blocks_list_to_print[i]->getPointOut2X();
                            int start_y = blocks_list_to_print[i]->getPointOut2Y();
                            int end_x = blocks_list_to_print[i]->getNext2()->getPointIn2X();
                            int end_y = blocks_list_to_print[i]->getNext2()->getPointIn2Y();
                            float angulo = -(atan2((end_x - start_x), (end_y - start_y)));
                            al_draw_line(start_x, start_y, end_x, end_y, black, 2);
                            al_draw_rotated_bitmap(arrow, (al_get_bitmap_width(arrow))/2, (al_get_bitmap_height(arrow))/2, end_x, end_y, angulo, 0);
                        }
                    }
                }
            }
        }
    }
}
void Interface :: execute() {
    if(current_executing_block != NULL) {

        if(program_connected == true) {
            //se for do tipo 8 = condicional
            //tem q fazer leitura de sensores para setar as variáveis de comparação
            if(current_executing_block->getType() == 8) {
                int* black_sensor_reading = communication->getBlackTypeReading();
                int* ultrasonic_sensor_reading = communication->getUltrasonicReading();
                //checa tipo de sensor
                switch(current_executing_block->getTypeOfSensor()) {
                    case 1:
                        //black sensor 1
                        current_executing_block->setParameter1(black_sensor_reading[0]);
                        current_executing_block->setParameter2(1);
                        break;

                    case 2:
                        //black sensor 2
                        current_executing_block->setParameter1(black_sensor_reading[1]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 3:
                        //black sensor 3
                        current_executing_block->setParameter1(black_sensor_reading[2]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 4:
                        //black sensor 4
                        current_executing_block->setParameter1(black_sensor_reading[3]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 5:
                        //black sensor 5
                        current_executing_block->setParameter1(black_sensor_reading[4]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 6:
                        //color sensor 1
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 7:
                        //color sensor 2
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 8:
                        //ultrasonic sensor 1
                        current_executing_block->setParameter1(ultrasonic_sensor_reading[0]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 9:
                        //ultrasonic sensor 2
                        current_executing_block->setParameter1(ultrasonic_sensor_reading[1]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 10:
                        //ultrasonic sensor 3
                        current_executing_block->setParameter1(ultrasonic_sensor_reading[2]);
                        current_executing_block->setParameter2(1);
                        break;
                }
            }
            //se for 1 significa que terminou execução ou está pronto para receber
            if((communication->getFeedback() == 1) || (communication->getFeedback() == -4)) {
                //testa se o próximo é não nulo
                if(current_executing_block->getExecutingNext() != NULL) {
                    cout<<"executou bloco: "<<current_executing_block->getName()<<endl;
                    current_executing_block = current_executing_block->getExecutingNext();
                    refresh_executing_block();
                    //bloco de ação
                    if(current_executing_block->getType() == 1) {
                        communication->setCommand(current_executing_block->getCommand());
                    }
                } else {
                    cout<<"executou bloco: "<<current_executing_block->getName()<<endl;
                    executing_fluxogram = false;
                }
            }
        } else {
            //se for do tipo 8 = condicional
            //tem q fazer leitura de sensores para setar as variáveis de comparação
            if(current_executing_block->getType() == 8) {
                //checa tipo de sensor
                switch(current_executing_block->getTypeOfSensor()) {
                    case 1:
                        //black sensor 1
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;

                    case 2:
                        //black sensor 2
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 3:
                        //black sensor 3
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 4:
                        //black sensor 4
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 5:
                        //black sensor 5
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 6:
                        //color sensor 1
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 7:
                        //color sensor 2
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 8:
                        //ultrasonic sensor 1
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 9:
                        //ultrasonic sensor 2
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 10:
                        //ultrasonic sensor 3
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                }
            }
            if(current_executing_block->getExecutingNext() != NULL) {
                cout<<"executou bloco: "<<current_executing_block->getName()<<endl;
                current_executing_block = current_executing_block->getExecutingNext();
                refresh_executing_block();
            } else {
                cout<<"executou bloco: "<<current_executing_block->getName()<<endl;
                executing_fluxogram = false;
            }
        }

    }
}
bool Interface :: check_if_only_one_startblock_exists() {
    int start_test = 0;
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == 6) {
                start_test = start_test + 1;
            }
        }
    }
    if(start_test == 1) {
        return true;
    } else {
        return false;
    }
}
bool Interface :: check_if_at_least_one_endblock_exist() {
    int end_test = 0;
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == 5) {
                end_test = end_test + 1;
            }
        }
    }
    if(end_test >= 1) {
        return true;
    } else {
        return false;
    }
}
bool Interface :: check_if_all_the_blocks_have_connections() {
    int connections_test = 1;
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == 1) {
                //function
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == 5) {
                //end
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == 6) {
                //start
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == 7) {
                //loop
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getNext2() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious2() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == 8) {
                //decision
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getNext2() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == 9) {
                //merge
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious2() == NULL) {
                    connections_test = 0;
                    break;
                }
            }
        }
    }
    if(connections_test == 1) {
        return true;
    } else {
        return false;
    }
}
bool Interface :: check_if_all_blocks_have_functions_or_sensors() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == 1) {
                //ação
                if(blocks_list_to_print[i]->getFunction() == 0) {
                    return false;
                }
            } else if(blocks_list_to_print[i]->getType() == 8) {
                //decisão
                if(blocks_list_to_print[i]->getTypeOfSensor() == 0) {
                    return false;
                }
            }

        }
    }
    return true;
}
void Interface :: reset_fluxogram_execution() {
    int id_start = 0;
    //procura pelo bloco de inicio
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == 6) {
                id_start = i;
                break;
            }
        }
    }
    //seta start como bloco atual
    current_executing_block = blocks_list_to_print[id_start];
}
void Interface :: connect_simulator() {
    communication->setIfVirtual(1);
    robot_connected = false;
    simulator_connected = true;
}
void Interface :: connect_robot() {
    communication->setIfVirtual(0);
    robot_connected = true;
    simulator_connected = false;
}
void Interface :: connect() {
    int feedback = 10;
    pid_t pid;
    pid = fork();
    switch (pid) {
        case -1 :
            //erro no fork
            feedback = -1;
            break;
        case 0 :
            //processo filho
            execl(".//..//FluxProgBackend//build//fluxprogbackend", "fluxprogbackend", NULL);
            //cout<<"caminho incorreto"<<endl;
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Falha ao abrir o programa BackEnd", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            kill(getpid(), SIGKILL);
            break;
        default :
            //processo pai
            feedback = 0;
    }

    if(feedback == 0) {
        sleep(2);
        communication->upadateReadings();
        //cout<<"abriu o programa"<<endl;
        feedback = communication->getFeedback();
        if(feedback == ERROR){
            //não abriu o v-rep ou não tem bluetooth
            //cout<<"não abriu o v-rep ou não tem bluetooth"<<endl;
            if(robot_connected == true) {
                al_show_native_message_box(display, "Fluxprog", "ERRO", "Bluetooth não encontrado", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            } else {
                al_show_native_message_box(display, "Fluxprog", "ERRO", "V-Rep não encontrado", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
            }
        } else if(feedback == CONNECTED) {
            //deu certo
            //cout<<"deu certo"<<endl;
            al_show_native_message_box(display, "Fluxprog", NULL, "Conectado com sucesso", "Ok", NULL);
            program_connected = true;
        }
        else{
            //cout << "ainda deu problema, rein vr"<<endl;
            al_show_native_message_box(display, "Fluxprog", "ERRO", "Reinicie a simulação do V-Rep e tente novamente", "Ok", ALLEGRO_MESSAGEBOX_ERROR);
        }
    }
}

//1. fazer submenu de numberos
//2. setar bloco de loop com numeros
//3. checar ligações entre blocos
