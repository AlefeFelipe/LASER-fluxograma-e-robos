#include "Interface.hpp"

Interface :: Interface()
{
    //variavel que diz se o programa ainda está executando ou não
    bool executing = true;

    //inicializa o allegro, dá msg de erro caso falhe alguma inicializacao
    inicialize(al_init(), "allegro");
	inicialize(al_init_image_addon(), "plugin de imagens");
	inicialize(al_init_primitives_addon(), "plugin de desenho de primitivos");
	inicialize(al_init_native_dialog_addon(), "plugin de caixas de diálogo nativo");
	inicialize(al_install_keyboard(), "teclado");
	inicialize(al_install_mouse(), "mouse");
	al_init_font_addon();
	inicialize(al_init_ttf_addon(), "plugin de fontes truetype");

    //CRIA A JANELA DO PROGRAMA E SETA AS VARIÁVEIS DA JANELA
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    display = al_create_display(display_width, display_height);
    al_set_window_position(display, 50, 50);
    al_set_window_title(display, "Fluxprog_v2");
    al_clear_to_color(al_map_rgb(0, 0, 0));


    //inicialização das cores
    black = al_map_rgb(0, 0, 0);
    grey = al_map_rgb(128, 128, 128);
    white = al_map_rgb(255, 255, 255);
    menu_color = al_map_rgb(176, 175, 175);

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));

    check_alocation(display, "display");

    load_bitmap(botao_opcao,"images/botao_opcao.png")
    load_bitmap(botao_bloco,"images/botao_bloco.png")
    load_bitmap(menu,"images/menu.png")
    load_bitmap(rodar,"images/rodar.png")
    load_bitmap(pausar,"images/pausar.png")
    load_bitmap(parar,"images/parar.png")
    load_bitmap(salvar,"images/salvar.png")
    load_bitmap(carregar,"images/carregar.png")
    load_bitmap(salvar_como,"images/salvarcomo.png")
    load_bitmap(vrep,"images/vrep.png")
    load_bitmap(bt_bluetooth, "images/bluetooth.png")
    load_bitmap(miniaturas[0],"images/miniatura_decisao.png")
    load_bitmap(miniaturas[1],"images/miniatura_acao.png")
    load_bitmap(miniaturas[2],"images/miniatura_inicio.png")
    load_bitmap(miniaturas[3],"images/miniatura_fim.png")
    load_bitmap(miniaturas[4],"images/miniatura_juncao.png")
    load_bitmap(miniaturas[5],"images/miniatura_repeticao.png")

    load_bitmap(seta,"images/seta.png")
    load_bitmap(voltita,"images/voltita.png")
    load_bitmap(ultrassom,"images/ultrassom.png")
    load_bitmap(logico_v,"images/logic_true.png")
    load_bitmap(logico_f,"images/logic_false.png")
    load_bitmap(fita,"images/fita.png")
    load_bitmap(PONTO[0],"images/point.png")
    load_bitmap(PONTO[1],"images/hpoint.png")
    load_bitmap(BLOCO[0][0],"images/block1.png")
    load_bitmap(BLOCO[0][1],"images/hblock1.png")
    load_bitmap(BLOCO[0][2],"images/sblock1.png")
    load_bitmap(BLOCO[0][3],"images/pblock1.png")
    load_bitmap(BLOCO[1][0],"images/block2.png")
    load_bitmap(BLOCO[1][1],"images/hblock2.png")
    load_bitmap(BLOCO[1][2],"images/sblock2.png")
    load_bitmap(BLOCO[1][3],"images/pblock2.png")
    load_bitmap(BLOCO[2][0],"images/block3.png")
    load_bitmap(BLOCO[2][1],"images/hblock3.png")
    load_bitmap(BLOCO[2][2],"images/sblock3.png")
    load_bitmap(BLOCO[2][3],"images/pblock3.png")
    load_bitmap(BLOCO[3][0],"images/block4.png")
    load_bitmap(BLOCO[3][1],"images/hblock4.png")
    load_bitmap(BLOCO[3][2],"images/sblock4.png")
    load_bitmap(BLOCO[3][3],"images/pblock4.png")
    load_bitmap(BLOCO[4][0],"images/block5.png")
    load_bitmap(BLOCO[4][1],"images/hblock5.png")
    load_bitmap(BLOCO[4][2],"images/sblock5.png")
    load_bitmap(BLOCO[4][3],"images/pblock5.png")
    load_bitmap(BLOCO[5][0],"images/block6.png")
    load_bitmap(BLOCO[5][1],"images/hblock6.png")
    load_bitmap(BLOCO[5][2],"images/sblock6.png")
    load_bitmap(BLOCO[5][3],"images/pblock6.png")
    load_bitmap(NUMERO[0],"images/NUMBER_0.png")
    load_bitmap(NUMERO[1],"images/NUMBER_1.png")
    load_bitmap(NUMERO[2],"images/NUMBER_2.png")
    load_bitmap(NUMERO[3],"images/NUMBER_3.png")
    load_bitmap(NUMERO[4],"images/NUMBER_4.png")
    load_bitmap(NUMERO[5],"images/NUMBER_5.png")
    load_bitmap(NUMERO[6],"images/NUMBER_6.png")
    load_bitmap(NUMERO[7],"images/NUMBER_7.png")
    load_bitmap(NUMERO[8],"images/NUMBER_8.png")
    load_bitmap(NUMERO[9],"images/NUMBER_9.png")

    font = al_load_font("OpenSans-Regular.ttf", 10, 0);
    check_alocation(font, "font");


    //al_destroy_display(display);
    //al_show_native_message_box(display, "Message Box Title", "Error", "Display windoiw teste", NULL, NULL);

    return 0; //por enquanto só formalidade
}

Interface :: ~Interface() {
    al_destroy_display(display);
}

void Interface :: start() {
    //variavel que diz se o programa ainda está executando ou não
    bool executing = true;

    while(executing) {

        al_acknowledge_resize(display);

        al_clear_to_color(al_map_rgb(0, 0, 0));

        //desenha o retangulo no qual ficam os botões de opções
        al_draw_filled_rectangle(0, 0, al_get_display_width(display), (4 + al_get_bitmap_height(botao_opcao)), menu_color);
        cout << al_get_display_width(display) << endl;
        //desenha a moldura do botão e dai o botão em si
        al_draw_bitmap(botao_opcao, 2 + 0*al_get_bitmap_width(botao_opcao), 2, 0);
        al_draw_bitmap(rodar, 14 + 0*al_get_bitmap_width(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 1*al_get_bitmap_width(botao_opcao), 2, 0);
        al_draw_bitmap(pausar, 14 + 1*al_get_bitmap_width(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 2*al_get_bitmap_width(botao_opcao), 2, 0);
        al_draw_bitmap(parar, 14 + 2*al_get_bitmap_width(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 3*al_get_bitmap_width(botao_opcao), 2, 0);
        al_draw_bitmap(salvar, 14 + 3*al_get_bitmap_width(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 4*al_get_bitmap_width(botao_opcao), 2, 0);
        al_draw_bitmap(carregar, 14 + 4*al_get_bitmap_width(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 5*al_get_bitmap_width(botao_opcao), 2, 0);
        al_draw_bitmap(salvar_como, 14 + 5*al_get_bitmap_width(botao_opcao), 14, 0);

        //desenha o botão de conexão ao v-rep e bluetooth acompanhando o lado direito da tela
        al_draw_bitmap(botao_opcao, al_get_display_width(display) - 2 - al_get_bitmap_width(botao_opcao) - roll_bar_width, 2, 0);
        al_draw_bitmap(vrep, al_get_display_width(display) + 10 - al_get_bitmap_width(botao_opcao) - roll_bar_width, 14, 0);
        al_draw_bitmap(botao_opcao, al_get_display_width(display) - 85 - al_get_bitmap_width(botao_opcao) - roll_bar_width, 2, 0);
        al_draw_bitmap(bt_bluetooth, al_get_display_width(display) - 73.5 - al_get_bitmap_width(botao_opcao) - roll_bar_width, 13.5, 0);

        //desenha todas as molduras dos botões que criam blocos e daí todos os botões
        for (int i = 0; i <= 12; i++) al_draw_bitmap(botao_bloco, 0, al_get_bitmap_height(botao_opcao) + 4 + i * al_get_bitmap_height(botao_bloco), 0);
        for (int i = 0; i < 6; i++) al_draw_bitmap(miniaturas[i], 6, al_get_bitmap_height(botao_opcao) + 4 + i * al_get_bitmap_height(botao_bloco) + 6, 0);
				al_draw_bitmap(seta, 6, al_get_bitmap_height(botao_opcao) + 6 * al_get_bitmap_height(botao_bloco) + 4 + 6, 0);
				al_draw_bitmap(voltita, 6, al_get_bitmap_height(botao_opcao) + 7 * al_get_bitmap_height(botao_bloco) + 4 + 6, 0);
				al_draw_bitmap(fita, 6, al_get_bitmap_height(botao_opcao) + 8 * al_get_bitmap_height(botao_bloco) + 4 + 6, 0);
				al_draw_bitmap(ultrassom, 6, al_get_bitmap_height(botao_opcao) + 9 * al_get_bitmap_height(botao_bloco) + 4 + 6, 0);
				al_draw_bitmap(NUMERO[2], 6, al_get_bitmap_height(botao_opcao) + 10 * al_get_bitmap_height(botao_bloco) + 4 + 6, 0); //desenha o 2 pois o bloco criado vai começar com o valor 2 (já que repetir 0 ou 1 vez não faz sentido)
				al_draw_bitmap(logico_v, 6, al_get_bitmap_height(botao_opcao) + 11 * al_get_bitmap_height(botao_bloco) + 4 + 6, 0);

        //botão para limpar a tela
        al_draw_bitmap(botao_bloco, 0, al_get_display_height(display) - al_get_bitmap_height(botao_bloco), 0);
        al_draw_bitmap(logico_f, 6, al_get_display_height(display) - al_get_bitmap_height(botao_bloco)+10, 0);

        al_flip_display();


        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            executing = false;
        }

    }
    ~Interface();
}
