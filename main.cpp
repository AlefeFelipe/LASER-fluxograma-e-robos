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

#define larguraDisplay 800
#define alturaDisplay 600

int main()
{

    ALLEGRO_DISPLAY *display;

    if(!al_init()) {
        al_show_native_message_box(NULL, NULL, NULL, "Could not initialize Allegro", NULL, NULL);
        return -1;
    }

    display = al_create_display(larguraDisplay, alturaDisplay);

    if(!display) {
        al_show_native_message_box(display, "Sample Title", "Display Settings", "Display Window was not created", NULL, NULL);
    }

    al_show_native_message_box(display, "Message Box Title", "Error", "Display windoiw teste", NULL, NULL);
	return 0; //por enquanto só formalidade
}
