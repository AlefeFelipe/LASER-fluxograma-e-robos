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

#define inicialize(X, Y) if (!X){al_show_native_message_box(NULL, NULL, NULL, "Erro na inicializacao", NULL, NULL);}
#define check_alocation(X, Y) if(!X) {al_show_native_message_box(NULL, NULL, NULL, "Erro na alocacao", NULL, NULL);}
#define load_bitmap(X, Y) {ALLEGRO_BITMAP* temporario; temporario = al_load_bitmap(Y); if (!temporario) {al_show_native_message_box(NULL, NULL, NULL, "Erro ao carregar bitmap", NULL, NULL); return -1;} else X = temporario;}


#define display_width 700
#define display_height 900
#define roll_bar_width 20

class Interface {
    ALLEGRO_COLOR black;
    ALLEGRO_COLOR grey;
    ALLEGRO_COLOR white;
    ALLEGRO_COLOR menu_color;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_BITMAP *botao_opcao, *botao_bloco, *menu, *rodar, *pausar, *parar, *salvar, *carregar, *salvar_como, *vrep, *bt_bluetooth, *miniaturas[6];
    ALLEGRO_BITMAP *seta, *voltita, *fita, *ultrassom, *NUMERO[10], *logico_v, *logico_f; //bitmaps para os blocos de travamento (os que travam nos blocos maiores)
    ALLEGRO_BITMAP *BLOCO[6][4], *PONTO[2]; //bitmaps para os blocos e suas variações (inativo, destacado pelo mouse, sendo movido e sendo rodado) e para os pontos de junção dos blocos (inativo e destacado)
    ALLEGRO_FONT *font;

public:

    Interface();
    ~Interface();
    void start();
};
#endif
