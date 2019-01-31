#include "fluxprog.h"
#include <cstring>
#define inicializar(X, Y) if (!X){cout << "erro na inicializaçao do"<< Y << endl;}
int main(int argc, char **argv)
{

    //inicialização do allegro e de seus plugins
    inicializar(al_init(), "allegro");
	inicializar(al_init_image_addon(), "plugin de imagens");
	inicializar(al_init_primitives_addon(), "plugin de desenho de primitivos");
	inicializar(al_init_native_dialog_addon(), "plugin de caixas de diálogo nativo");
	inicializar(al_install_keyboard(), "teclado");
	inicializar(al_install_mouse(), "mouse");
	al_init_font_addon();
	inicializar(al_init_ttf_addon(), "plugin de fontes truetype");

	inic_fluxprog();

    //inicialização das cores
    preto = al_map_rgb(0, 0, 0);
    cinza = al_map_rgb(128, 128, 128);
    branco = al_map_rgb(255, 255, 255);
	//inicialização do display
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	tela = al_create_display(dimensoes_tela_inicial_x, dimensoes_tela_inicial_y);
	verificar_alocacao(tela, "tela");
	al_set_window_title(tela, "fluxprog");

	//inicialização dos bitmaps
	#define carregar_bitmap(X, Y) {ALLEGRO_BITMAP* temporario; temporario = al_load_bitmap(Y); if (!temporario) {cout << "erro carregando bitmap " << Y << endl; return -1;} else X = temporario;}
    ALLEGRO_BITMAP *botao_opcao, *botao_bloco, *menu, *rodar, *pausar, *parar, *salvar, *carregar, *salvar_como, *vrep, *miniaturas[6];
    carregar_bitmap(botao_opcao,"images/botao_opcao.png")
    carregar_bitmap(botao_bloco,"images/botao_bloco.png")
    carregar_bitmap(menu,"images/menu.png")
    carregar_bitmap(rodar,"images/rodar.png")
    carregar_bitmap(pausar,"images/pausar.png")
    carregar_bitmap(parar,"images/parar.png")
    carregar_bitmap(salvar,"images/salvar.png")
    carregar_bitmap(carregar,"images/carregar.png")
    carregar_bitmap(salvar_como,"images/salvarcomo.png")
    carregar_bitmap(vrep,"images/vrep.png")
    carregar_bitmap(miniaturas[0],"images/miniatura_decisao.png")
    carregar_bitmap(miniaturas[1],"images/miniatura_acao.png")
    carregar_bitmap(miniaturas[2],"images/miniatura_inicio.png")
    carregar_bitmap(miniaturas[3],"images/miniatura_fim.png")
    carregar_bitmap(miniaturas[4],"images/miniatura_juncao.png")
    carregar_bitmap(miniaturas[5],"images/miniatura_repeticao.png")

    carregar_bitmap(seta,"images/seta.png")
    carregar_bitmap(voltita,"images/voltita.png")
    carregar_bitmap(ultrassom,"images/ultrassom.png")
    carregar_bitmap(logico_v,"images/logic_true.png")
    carregar_bitmap(logico_f,"images/logic_false.png")
    carregar_bitmap(fita,"images/fita.png")
    carregar_bitmap(PONTO[0],"images/point.png")
    carregar_bitmap(PONTO[1],"images/hpoint.png")
    carregar_bitmap(BLOCO[0][0],"images/block1.png")
    carregar_bitmap(BLOCO[0][1],"images/hblock1.png")
    carregar_bitmap(BLOCO[0][2],"images/sblock1.png")
    carregar_bitmap(BLOCO[0][3],"images/pblock1.png")
    carregar_bitmap(BLOCO[1][0],"images/block2.png")
    carregar_bitmap(BLOCO[1][1],"images/hblock2.png")
    carregar_bitmap(BLOCO[1][2],"images/sblock2.png")
    carregar_bitmap(BLOCO[1][3],"images/pblock2.png")
    carregar_bitmap(BLOCO[2][0],"images/block3.png")
    carregar_bitmap(BLOCO[2][1],"images/hblock3.png")
    carregar_bitmap(BLOCO[2][2],"images/sblock3.png")
    carregar_bitmap(BLOCO[2][3],"images/pblock3.png")
    carregar_bitmap(BLOCO[3][0],"images/block4.png")
    carregar_bitmap(BLOCO[3][1],"images/hblock4.png")
    carregar_bitmap(BLOCO[3][2],"images/sblock4.png")
    carregar_bitmap(BLOCO[3][3],"images/pblock4.png")
    carregar_bitmap(BLOCO[4][0],"images/block5.png")
    carregar_bitmap(BLOCO[4][1],"images/hblock5.png")
    carregar_bitmap(BLOCO[4][2],"images/sblock5.png")
    carregar_bitmap(BLOCO[4][3],"images/pblock5.png")
    carregar_bitmap(BLOCO[5][0],"images/block6.png")
    carregar_bitmap(BLOCO[5][1],"images/hblock6.png")
    carregar_bitmap(BLOCO[5][2],"images/sblock6.png")
    carregar_bitmap(BLOCO[5][3],"images/pblock6.png")
    carregar_bitmap(NUMERO[0],"images/NUMBER_0.png")
    carregar_bitmap(NUMERO[1],"images/NUMBER_1.png")
    carregar_bitmap(NUMERO[2],"images/NUMBER_2.png")
    carregar_bitmap(NUMERO[3],"images/NUMBER_3.png")
    carregar_bitmap(NUMERO[4],"images/NUMBER_4.png")
    carregar_bitmap(NUMERO[5],"images/NUMBER_5.png")
    carregar_bitmap(NUMERO[6],"images/NUMBER_6.png")
    carregar_bitmap(NUMERO[7],"images/NUMBER_7.png")
    carregar_bitmap(NUMERO[8],"images/NUMBER_8.png")
    carregar_bitmap(NUMERO[9],"images/NUMBER_9.png")

	coord.inic(); //calcula as constantes usadas pelo programa para desenhar

	fonte = al_load_font("OpenSans-Regular.ttf", 10, 0);
    verificar_alocacao(fonte, "fonte")

	//inicialização do mouse e de seu tratamento, além do tratamento do teclado e do botao de fechar o programa
	//o tratamento do mouse, teclado e outros meios de entrada são tratadas pelo allegro por meio de filas que recebem os eventos que
	//ocorrem desse tipo. Assim, para tratar os eventos (como uma tecla do teclado ser apertada ou o ponteiro do mouse ser movido)
	//eles são retirados da lista e entao avaliados
	al_set_system_mouse_cursor(tela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
	ALLEGRO_EVENT_QUEUE *fila_mouse = al_create_event_queue(), *fila_tela= al_create_event_queue(), *fila_teclado= al_create_event_queue();
	verificar_alocacao(fila_mouse, "fila_mouse")
	verificar_alocacao(fila_tela, "fila_tela")
	verificar_alocacao(fila_teclado, "fila_teclado")
	al_register_event_source(fila_mouse, al_get_mouse_event_source()); //associam as filas a uma fonte de eventos
	al_register_event_source(fila_tela, al_get_display_event_source(tela));
	al_register_event_source(fila_teclado, al_get_keyboard_event_source());

	//descobre o tamanho do monitor
	ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);
    monitor_altura = monitor.y2 - monitor.y1;
    monitor_comprimento = monitor.x2 - monitor.x1;

    //isso é usado para transformar as coordenadas dos blocos, o que é usado para fazer eles subirem e descerem de acordo com a barra de rolagem
    ALLEGRO_TRANSFORM posicao;

	lista_blocos *lista = new lista_blocos;
    verificar_alocacao(lista, "lista")
    //isso cria um timer para registrar o tempo, o que é usado para o clique duplo e controlar quanto a barra de rolagem sobe ou desce, por exemplo
	ALLEGRO_TIMER *temporizador = al_create_timer(intervalo_eventos_temporizador);
	verificar_alocacao(temporizador, "temporizador")
	ALLEGRO_EVENT_QUEUE *fila_tempo = al_create_event_queue();
	verificar_alocacao(fila_tempo,"fila_tempo")
	al_register_event_source(fila_tempo, al_get_timer_event_source(temporizador));

	//inicialização das memórias compartilhadas
	managed_shared_memory* memoria;
	shared_memory_object::remove(SENDER_MEMORY_NAME); //se o programa tiver terminado irregularmente da ultima vez e nao tenha destruido a memoria antiga, e necessario destrui-la para garantir
	memoria = new managed_shared_memory(open_or_create, SENDER_MEMORY_NAME, MEMORY_SIZE);
	verificar_alocacao(memoria, "memoria")
	command_var = memoria->construct<int>(COMMAND_VARIABLE_NAME)(); //cria as variaveis dentro das memorias
    verificar_alocacao(command_var, "command_var")
	*command_var = 0;
    feedback = memoria->construct<int>(FEEDBACK_VARIABLE_NAME)(); //cria as variaveis dentro das memorias
    verificar_alocacao(feedback, "feedback")
	*feedback = 0;
    bluetooth = memoria->construct<int>(BLUETOOTH_ENABLE)();//indica se o robô real sera usado ou nao
    verificar_alocacao(bluetooth, "bluetooth");
    *bluetooth = 0;

	//diversas flags usadas no programa
	bool barra_posicao_ativa = false;
	bool sair_do_programa = false;
	bool vrep_conectado = false;
	bool vrep_conectando = false;
	bool api_rodando = false;
	bool arquivo_escolhido = false;

    int ciclos_tempo = 0; //guarda quantas vezes ocorreu um evento do timer desde o ultimo loop
    int nova_posicao_y = posicao_y;
    float roda_atual = 0; //guarda a posição da roda do mouse

    //cria vetores de caracteres que armazenam
    char* caminho = new char[100];
    verificar_alocacao(caminho, "caminho")
    strcpy(caminho, ".fp");
    char* titulo  = new char[100];
    verificar_alocacao(titulo, "titulo")
    strcpy(titulo, "fluxprog");

    al_start_timer(temporizador);


	while (!sair_do_programa) {
        caractere_pressionado[0] = '\0';
        caractere_pressionado[1] = '\0';
        if (nova_posicao_y < 0) nova_posicao_y = 0;
        else if (nova_posicao_y > dimensao_vertical_maxima - dimensoes_tela_y) nova_posicao_y = dimensao_vertical_maxima - dimensoes_tela_y;
        posicao_y = nova_posicao_y;
		//parte da atualização dos botoes do mouse
		for (int i = 0; i < 3; i++) {
			if (mouse_clicar[i]) { //se no ultimo ciclo o botao do mouse foi apertado, nesse ciclo ele esta sendo segurado
				mouse_clicar[i] = false;
				mouse_segurar[i] = true;
			}
			mouse_soltar[i] = false; //se no ultimo ciclo o botao do mouse foi solto, nesse ele ja esta completamente solto
		}
		while (!al_is_event_queue_empty(fila_tela)) { //se a fila de eventos da tela nao estiver vazia
			ALLEGRO_EVENT eventos_tela; //cria um objeto que contem os eventos
			al_wait_for_event(fila_tela, &eventos_tela); //armazena o evento no objeto
			if (eventos_tela.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair_do_programa= true; //e se o tipo do evento for o que indica que o botão "X" foi clicado, faz com que o programa termine no final do loop
            if (eventos_tela.type == ALLEGRO_EVENT_DISPLAY_RESIZE) al_acknowledge_resize(tela);
		}
		while (!al_is_event_queue_empty(fila_mouse)) {
			ALLEGRO_EVENT eventos_mouse;
			al_wait_for_event(fila_mouse, &eventos_mouse);
			if (eventos_mouse.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) { //se um botão do mouse "subir", ou seja, for solto

				if (eventos_mouse.mouse.button == 1) { //e for o botao da esquerda(1) o botao da esquerda foi solto, e, portanto, nao esta mais sendo segurado
					mouse_soltar[mouse_esq] = true;
					mouse_segurar[mouse_esq] = false;
                    //zera as variáveis de offset, pois não há nenhum caso onde elas não são zeradas quando o mouse é solto
                    dmouse_x = 0;
                    dmouse_y = 0;
				}
				if (eventos_mouse.mouse.button == 2) {//o mesmo para o da esquerda(2)
					mouse_soltar[mouse_dir] = true;
					mouse_segurar[mouse_dir] = false;
				}
				if (eventos_mouse.mouse.button == 3) {//e para o do meio(3)
					mouse_soltar[mouse_meio] = true;
					mouse_segurar[mouse_meio] = false;
				}

			}
			if (eventos_mouse.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {//se um botão do mouse for "abaixado" (ou seja, pressionado)

				if (eventos_mouse.mouse.button == 1) {//e for o botão da esquerda, ele foi clicado, e, portanto, não está segurado
					mouse_clicar[mouse_esq] = true;
					mouse_segurar[mouse_esq] = false;
				}
				if (eventos_mouse.mouse.button == 2) {
					mouse_clicar[mouse_dir] = true;
					mouse_segurar[mouse_dir] = false;
				}
				if (eventos_mouse.mouse.button == 3) {
					mouse_clicar[mouse_meio] = true;
					mouse_segurar[mouse_meio] = false;
				}
			}
			if (eventos_mouse.type == ALLEGRO_EVENT_MOUSE_AXES) { //se o mouse for mexido
				//atualiza as variaveis globais que armazenam as coordenadas do mouse
				mouse_x = eventos_mouse.mouse.x;
				mouse_y = eventos_mouse.mouse.y;
				if (modo == desenhando) lista->checar_colisoes(eventos_mouse.mouse.x, eventos_mouse.mouse.y); //se o programa estiver em modo de desenho, checa se houve mudança em alguma colisão

                //atualiza a barra de rolagem para cima ou para baixo comparando a posição atual da roda do mouse com a anterior
                nova_posicao_y += (roda_atual - eventos_mouse.mouse.z)*sensibilidade_roda_mouse;
                roda_atual = eventos_mouse.mouse.z;
			}
		}

		while (!al_is_event_queue_empty(fila_teclado)) {
			ALLEGRO_EVENT eventos_teclado;
			al_wait_for_event(fila_teclado, &eventos_teclado);
			if (eventos_teclado.type == ALLEGRO_EVENT_KEY_DOWN) { //eventos do teclado que acontecem quando uma tecla é "abaixada" (apertada)
				/*códigos do teclado allegro:
                    1 a 26 : letras (ordem alfabética)
                    27 a 36 : números
                    37 a 46 : numpad
                    setas: esquerda: 82
                    direita : 83
                    cima 	: 84
                    baixo   : 85
                  código ASCII:
                    letras maiúsculas: 65 a 90
                    números: 48 a 57

                "offset" para números = 48 - 27 = 21
                "offset" para numpad = 48 - 37 = 11
                Para simplificar o tratamento, as setas são consideradas como os números equivalentes do numpad

                */
                if (eventos_teclado.keyboard.keycode < 27) caractere_pressionado[0] = 64 + eventos_teclado.keyboard.keycode;
                else if (eventos_teclado.keyboard.keycode < 37) {
                    caractere_pressionado[0] = 21 + eventos_teclado.keyboard.keycode;
                    caractere_pressionado[1] = 'n'; //para indicar que foi um numero
                    }
                else if (eventos_teclado.keyboard.keycode < 47) {
                    caractere_pressionado[0] = 11 + eventos_teclado.keyboard.keycode;
                    caractere_pressionado[1] = 'N'; //para indicar q foi no numpad
                }
                else if (eventos_teclado.keyboard.keycode >= 82 && eventos_teclado.keyboard.keycode <= 85) {
                    caractere_pressionado[1] = 'S'; //para indicar q foi uma das setas
                    if (eventos_teclado.keyboard.keycode == 82) caractere_pressionado[0] = '4';
                    else if (eventos_teclado.keyboard.keycode == 83) caractere_pressionado[0] = '6';
                    else if (eventos_teclado.keyboard.keycode == 84) caractere_pressionado[0] = '8';
                    else if (eventos_teclado.keyboard.keycode == 85) caractere_pressionado[0] = '2';
                }

                //se não houver bloco ativo, o teclado é usado para os atalhos que criam blocos
                if (!lista->ativo) switch (eventos_teclado.keyboard.keycode) {
					case ALLEGRO_KEY_ESCAPE :
                        sair_do_programa = true;
						break;
					case ALLEGRO_KEY_1 :
						lista->criar_bloco(bloco_decisao, meio_tela, false);
						break;
					case ALLEGRO_KEY_2:
						lista->criar_bloco(bloco_acao, meio_tela, false);
						break;
					case ALLEGRO_KEY_3:
                        if (blocos_inicio) al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "só pode haver um bloco de inicio no fluxograma", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                        else lista->criar_bloco(bloco_inicio, meio_tela, false);
						break;
					case ALLEGRO_KEY_4:
						lista->criar_bloco(bloco_fim, meio_tela, false);
						break;
					case ALLEGRO_KEY_5:
						lista->criar_bloco(bloco_juncao, meio_tela, false);
						break;
					case ALLEGRO_KEY_6:
						lista->criar_bloco(bloco_repeticao, meio_tela, false);
						break;
					case ALLEGRO_KEY_Q:
						lista->criar_bloco(bloco_andar, meio_tela, false);
						break;
					case ALLEGRO_KEY_W:
						lista->criar_bloco(bloco_virar, meio_tela, false);
						break;
					case ALLEGRO_KEY_E:
						lista->criar_bloco(bloco_fita, meio_tela, false);
						break;
					case ALLEGRO_KEY_R:
						lista->criar_bloco(bloco_som, meio_tela, false);
						break;
					case ALLEGRO_KEY_T:
						lista->criar_bloco(bloco_numero, meio_tela, false);
						break;
					case ALLEGRO_KEY_Y:
						lista->criar_bloco(bloco_VF, meio_tela, false);
						break;
				}

			}
		}
		//se o tempo que passou desde o ultimo ciclo é maior que o definido em intervalo_eventos_temporizador, incrementa a variável ciclos_tempo
        while (!al_is_event_queue_empty(fila_tempo)) {
			ALLEGRO_EVENT eventos_tempo;
			al_wait_for_event(fila_tempo, &eventos_tempo);
			if (eventos_tempo.type == ALLEGRO_EVENT_TIMER) {
                ciclos_tempo++;
                //verifica se já passou tempo suficiente para que um clique não possa mais ser considerado como o segundo de um clique duplo
                if (tempo_max_clique_duplo >= tempo_maximo_clique_duplo/intervalo_eventos_temporizador) {
                    tempo_max_clique_duplo = 0;
                    possibilidade_clique_duplo = false;
                }
                else tempo_max_clique_duplo++;
			}

		}


        //esse bloco de código muda a posição dos blocos para que eles sejam impressos de acordo com a barra lateral
        al_identity_transform(&posicao);
        al_translate_transform(&posicao, 0, -posicao_y);
        al_use_transform(&posicao);

		lista->desenhar();

		//esse bloco volta ao estado inicial para que os botões e opções não saiam do lugar
        al_identity_transform(&posicao);
        al_translate_transform(&posicao, 0, 0);
        al_use_transform(&posicao);

        //desenha o retangulo no qual ficam os botões de opções
        al_draw_filled_rectangle(0, 0, dimensoes_tela_x, 4 + altura(botao_opcao), al_map_rgb(176, 175, 175));

        //desenha a moldura do botão e dai o botão em si
        al_draw_bitmap(botao_opcao, 2 + 0*largura(botao_opcao), 2, 0);
        al_draw_bitmap(rodar, 14 + 0*largura(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 1*largura(botao_opcao), 2, 0);
        al_draw_bitmap(pausar, 14 + 1*largura(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 2*largura(botao_opcao), 2, 0);
        al_draw_bitmap(parar, 14 + 2*largura(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 3*largura(botao_opcao), 2, 0);
        al_draw_bitmap(salvar, 14 + 3*largura(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 4*largura(botao_opcao), 2, 0);
        al_draw_bitmap(carregar, 14 + 4*largura(botao_opcao), 14, 0);

        al_draw_bitmap(botao_opcao, 2 + 5*largura(botao_opcao), 2, 0);
        al_draw_bitmap(salvar_como, 14 + 5*largura(botao_opcao), 14, 0);

        //desenha o botão de conexão ao v-rep acompanhando o lado direito da tela
        al_draw_bitmap(botao_opcao, dimensoes_tela_x - 2 - largura(botao_opcao) - largura_barra_rolagem, 2, 0);
        al_draw_bitmap(vrep, dimensoes_tela_x + 10 - largura(botao_opcao) - largura_barra_rolagem, 14, 0);

        //desenha todas as molduras dos botões que criam blocos e daí todos os botões
        for (int i = 0; i <= 12; i++) al_draw_bitmap(botao_bloco, 0, altura(botao_opcao) + 4 + i * altura(botao_bloco), 0);
        for (int i = 0; i < 6; i++) al_draw_bitmap(miniaturas[i], 6, altura(botao_opcao) + 4 + i * altura(botao_bloco) + 6, 0);
				al_draw_bitmap(seta, 6, altura(botao_opcao) + 6 * altura(botao_bloco) + 4 + 6, 0);
				al_draw_bitmap(voltita, 6, altura(botao_opcao) + 7 * altura(botao_bloco) + 4 + 6, 0);
				al_draw_bitmap(fita, 6, altura(botao_opcao) + 8 * altura(botao_bloco) + 4 + 6, 0);
				al_draw_bitmap(ultrassom, 6, altura(botao_opcao) + 9 * altura(botao_bloco) + 4 + 6, 0);
				al_draw_bitmap(NUMERO[2], 6, altura(botao_opcao) + 10 * altura(botao_bloco) + 4 + 6, 0); //desenha o 2 pois o bloco criado vai começar com o valor 2 (já que repetir 0 ou 1 vez não faz sentido)
				al_draw_bitmap(logico_v, 6, altura(botao_opcao) + 11 * altura(botao_bloco) + 4 + 6, 0);

        //botão para limpar a tela
        al_draw_bitmap(botao_bloco, 0, dimensoes_tela_y - altura(botao_bloco), 0);
        al_draw_bitmap(logico_f, 6, dimensoes_tela_y - altura(botao_bloco)+10, 0);
        if(modo == desenhando && botao(6, dimensoes_tela_y - altura(botao_bloco)+10, largura(logico_f), altura(logico_f),0, lista->ativo) && mouse_clicar[mouse_esq]) {
            //ver referência em https://www.allegro.cc/manual/5/al_show_native_message_box
            if (al_show_native_message_box(al_get_current_display(), "Fluxprog", "Limpar tela!", "voce tem certeza que quer deletar o fluxograma atual?", NULL, ALLEGRO_MESSAGEBOX_YES_NO) == 1) {
                delete lista;
                lista = new lista_blocos;
            }
        }

		//parte responsável pela conecção com o API

		if (vrep_conectado) { //desenha um retângulo vermelho pra marcar que a conexão ocorreu e faz com que se o botão for apertado novamente a memória seja desconectada
		 //(não e preciso mudar as variáveis, pois elas serão refeitas quando o botao for apertado de novo, e não serão usadas se vrep_conectado for falso)
			al_draw_rectangle(dimensoes_tela_x + 10 - largura(botao_opcao) - largura_barra_rolagem, 14, dimensoes_tela_x + 10 - largura(botao_opcao) - largura_barra_rolagem + largura(rodar), 14 + altura(rodar), al_map_rgb(255, 0 , 0), 1);
            if (modo == desenhando && mouse_clicar[mouse_esq] && botao(dimensoes_tela_x + 10 - largura(botao_opcao) - largura_barra_rolagem, 14, largura(rodar), altura(rodar), offset_opcao, lista->ativo)) { //se o usuário clicar sobre o botão do VREP quando ele já estiver conectado, o desconecta
                vrep_conectado = false;
                vrep_conectando = false;
                api_rodando = false;
                *command_var = -10;
                al_show_native_message_box(al_get_current_display(), "Fluxprog", "desconectado!", "o programa foi desconectado da API", NULL, ALLEGRO_MESSAGEBOX_ERROR);
            } //se o programa da api fechar, também desconectada
            else if (*feedback == -10) {
                *feedback = -1;
                vrep_conectado = false;
                vrep_conectando = false;
                api_rodando = false;
                al_show_native_message_box(al_get_current_display(), "Fluxprog", "desconectado!", "o programa foi desconectado da API", NULL, ALLEGRO_MESSAGEBOX_ERROR);
            }
		} //se o botão for apertado, tenta começar a conectar (se já não estiver tentando)
		else if (botao(dimensoes_tela_x + 10 - largura(botao_opcao) - largura_barra_rolagem, 14, largura(rodar), altura(rodar), offset_opcao, lista->ativo) && mouse_clicar[mouse_esq] && !vrep_conectando) {
            vrep_conectando = true;
            *command_var = 0;
		} //se estiver tentando conectar, tenta abrir o programa da api
		if (vrep_conectando) {
            if (!api_rodando){
                //esse é um bloco de código específico para o linux, que basicamente duplica o programa atual
                //e transforma o programa filho em um programa diferente, que é o da api
                bool sucesso = true;
                pid_t pid;
                pid = fork();
                switch (pid) {
                    case -1 :
                        sucesso = false;
                        break;
                    case 0 :
                        execl(".//..//ProjetoRemoteApi//build//cppremoteapi", "cppremoteapi", NULL);
                        sucesso = false;
                        break;
                    default :
                        api_rodando = true;
                }
                if (!sucesso) { //se falhar em abrir o programa, aborta a conexão
                    vrep_conectando = false;
                al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "A comunicação não pôde ser estabelecida (o processo da API não foi criado corretamente)", NULL, ALLEGRO_MESSAGEBOX_ERROR);}
            }
            else if (*feedback == -4){//se a api mandar a mensagem de sucesso na conexão ao v-rep, conecta as variáveis (se houverem) ao programa
            	try {
                    if (modelo){
                        posicao_plana = memoria->find<float>(POSICAO);
                        posicao_angular = memoria->find<float>(ANGULAR);
                        distancias_ultrassom = memoria->find<float>(POSICAO_DETECTADA);
                        sensores_fita = memoria->find<unsigned char>(SENSOR_VISAO);
                        sensores_ultrassom = memoria->find<unsigned char>(SENSOR_ULTRASSOM);
                    }
				    vrep_conectado = true;
				    vrep_conectando = false;
                    al_show_native_message_box(al_get_current_display(), "Fluxprog", "Sucesso!", "conexão feita com sucesso", NULL, ALLEGRO_MESSAGEBOX_ERROR);
				}
			        catch(...) { //se falhar, como a biblioteca boost usa exceções, a detecção da exceção levará a exibição de uma mensagem de erro
			            al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "O programa não conseguiu detectar a cena do VREP", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                    }

            }
            else if (*feedback == -5) {//se o programa da api não conseguir se conectar ao v-rep, aborta a conexão
                vrep_conectando = false;
                api_rodando = false;
                al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "O programa não conseguiu detectar a cena do VREP (verificar se a cena está aberta e rodando)", NULL, ALLEGRO_MESSAGEBOX_ERROR);
            }
		}

        //botão de "play"

        //se estiver rodando, desenha um retangulo branco em volta do botão
        //se não estiver rodando, verifica se o programa está conectado ao v-rep
        //se estiver, entra no modo de rodar o fluxograma
		if (modo == rodando) al_draw_rectangle(14 - offset_opcao, 14 - offset_opcao, 14 + largura(rodar) + offset_opcao, 14 + altura(rodar) + offset_opcao, branco, 1);
		else if (botao(14, 14, largura(rodar), altura(rodar), offset_opcao, lista->ativo) && mouse_clicar[mouse_esq]) {
			if (!vrep_conectado) al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "O programa não está conectado ao V-REP", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			else if (!blocos_inicio) al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "O programa não conseguiu encontrar nenhum bloco de inicio", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			else {
				modo = rodando;
				if (vrep_conectado && !modelo) *feedback = -1;
				if (lista->ativo) resetar_lista_blocos();
			}
		}

        //botão de pausa

		//se estiver pausado, desenha um retangulo em volta do botão
		//se estiver rodando o fluxograma e o botão for apertado, pausa o programa
		//(termina a instrução que foi enviada, mas não manda a próxima nem permite a edição do fluxograma)
		if (modo == pausado) al_draw_rectangle(14 + 1*largura(botao_opcao) - offset_opcao, 14 - offset_opcao, 14 + largura(rodar) + 1*largura(botao_opcao) + offset_opcao, 14 + altura(rodar) + offset_opcao, branco, 1);
		else if (botao(14 + largura(botao_opcao), 14, largura(rodar), altura(rodar), offset_opcao, lista->ativo) && mouse_clicar[mouse_esq] && modo == rodando) modo = pausado;

		//botão de parar

		//se estiver desenhando, desenha um retangulo em volta do botão
		//se estiver rodando o fluxograma ou pausado e o botão for apertado, volta para o modo de desenhar
		if (modo == desenhando) al_draw_rectangle(14 + 2*largura(botao_opcao) - offset_opcao, 14 - offset_opcao, 14 + largura(rodar) + 2*largura(botao_opcao) + offset_opcao, 14 + altura(rodar) + offset_opcao, branco, 1);
		else if (botao(14 + 2*largura(botao_opcao), 14, largura(rodar), altura(rodar), offset_opcao, lista->ativo) && mouse_clicar[mouse_esq]) {
            modo = desenhando;
            if (lista->ativo) resetar_lista_blocos();
            bloco_enviado = false;
        }

		//botão de salvar

        //se estiver no modo de desenho e o botão for apertado
        //verifica se já existe um arquivo aberto(se o usuário já abriu um fluxograma ou salvou o atual). Se houver, salva nele.
        //se não houver, abre uma caixa de diálogo para selecionar o arquivo
        //e escreve o caminho para o arquivo no cabeçalho
		if (modo == desenhando && botao(14 + 3*largura(botao_opcao), 14, largura(rodar), altura(rodar), offset_opcao, lista->ativo) && mouse_clicar[mouse_esq]) {
            if (!arquivo_escolhido) {
                ALLEGRO_FILECHOOSER *selecionar_arquivo = al_create_native_file_dialog(caminho, "salvar fluxograma","*.fp*" , ALLEGRO_FILECHOOSER_SAVE);
                if (selecionar_arquivo != 0) {
                    al_show_native_file_dialog(tela, selecionar_arquivo);
                    if (al_get_native_file_dialog_count(selecionar_arquivo)) {//verifica se a caixa de dialogo retornou algum caminho
                        strcpy(caminho, al_get_native_file_dialog_path(selecionar_arquivo, 0));
                        arquivo_escolhido = true;
                        if (lista->salvar(caminho)) { //salva e atualiza o texto do cabeçalho
                            al_show_native_message_box(al_get_current_display(), "Fluxprog", "Sucesso!", "Seu programa foi salvo corretamente", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                            strcpy(titulo, "fluxprog - ");
                            strcat(titulo, caminho);
                            al_set_window_title(tela, titulo);
                        }
                        else al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não foi possível criar o arquivo", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    al_destroy_native_file_dialog(selecionar_arquivo);
                }
                else al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não foi possivel abrir a caixa de diálogo, por favor tente novamente", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			}
			else {
                    if (lista->salvar(caminho)) al_show_native_message_box(al_get_current_display(), "Fluxprog", "Sucesso!", "Seu programa foi salvo corretamente", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                    else al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não foi possível criar o arquivo", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                }
        }


		//carrega um fluxograma
		if (modo == desenhando && botao(14 + 4*largura(botao_opcao), 14, largura(rodar), altura(rodar), offset_opcao, lista->ativo) && mouse_clicar[mouse_esq]) {
            ALLEGRO_FILECHOOSER *selecionar_arquivo = al_create_native_file_dialog(caminho, "carregar fluxograma","*.fp*" , ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
            verificar_alocacao(selecionar_arquivo,"selecionar_arquivo")
            if (selecionar_arquivo != 0) {
                al_show_native_file_dialog(tela, selecionar_arquivo);
                if (al_get_native_file_dialog_count(selecionar_arquivo)) {
                    //destroi os blocos que atualmente estão na lista
                    delete lista;
                    lista = new lista_blocos;

                    //carrega o fluxograma escolhido e atualiza o nome no cabeçalho
                    strcpy(caminho, al_get_native_file_dialog_path(selecionar_arquivo, 0));
                    arquivo_escolhido = true;
                    if (!lista->carregar(caminho)) {
                        al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não foi possível abrir o arquivo", NULL, ALLEGRO_MESSAGEBOX_ERROR);;
                        delete lista;
                        lista = new lista_blocos;
                    }
                    else {
                        strcpy(titulo, "fluxprog - ");
                        strcat(titulo, caminho);
                        al_set_window_title(tela, titulo);
                    }
                }
                al_destroy_native_file_dialog(selecionar_arquivo);
            }
            else al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não foi possivel abrir a caixa de diálogo, por favor tente novamente", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        //salvar como

        //que nem o salvar normal, mas sempre chamando a caixa de diálogo

		if (modo == desenhando && botao(14 + 5*largura(botao_opcao), 14, largura(rodar), altura(rodar), offset_opcao, lista->ativo) && mouse_clicar[mouse_esq]) {
            ALLEGRO_FILECHOOSER *selecionar_arquivo = al_create_native_file_dialog(caminho, "salvar fluxograma","*.fp*" , ALLEGRO_FILECHOOSER_SAVE);
            if (selecionar_arquivo != 0) {
                al_show_native_file_dialog(tela, selecionar_arquivo);
                if (al_get_native_file_dialog_count(selecionar_arquivo)) {
                    strcpy(caminho, al_get_native_file_dialog_path(selecionar_arquivo, 0));
                    arquivo_escolhido = true;
                    if (lista->salvar(caminho)) {
                        al_show_native_message_box(al_get_current_display(), "Fluxprog", "Sucesso!", "Seu programa foi salvo corretamente", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                        strcpy(titulo, "fluxprog - ");
                        strcat(titulo, caminho);
                        al_set_window_title(tela, titulo);
                    }
                    else al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não foi possível criar o arquivo", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                }
                al_destroy_native_file_dialog(selecionar_arquivo);
            }
            else al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não foi possivel abrir a caixa de diálogo, por favor tente novamente", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		}
		if (modo == desenhando) {
			lista->atualizar();
			//blocos
            if (botao(6, altura(botao_opcao) +  0 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_decisao,   mouse_xy_ajustado, true);
			if (botao(6, altura(botao_opcao) +  1 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_acao,      mouse_xy_ajustado, true);
			if (botao(6, altura(botao_opcao) +  2 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) {
                if (blocos_inicio) al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "só pode haver um bloco de inicio no fluxograma", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                else lista->criar_bloco(bloco_inicio, mouse_xy_ajustado, true);
			}
			if (botao(6, altura(botao_opcao) +  3 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_fim,       mouse_xy_ajustado, true);
			if (botao(6, altura(botao_opcao) +  4 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_juncao,    mouse_xy_ajustado, true);
			if (botao(6, altura(botao_opcao) +  5 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_repeticao, mouse_xy_ajustado, true);
		    if (botao(6, altura(botao_opcao) +  6 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_andar,     mouse_xy_ajustado, true);
			if (botao(6, altura(botao_opcao) +  7 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_virar,     mouse_xy_ajustado, true);
			if (botao(6, altura(botao_opcao) +  9 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_som,       mouse_xy_ajustado, true);
			if (botao(6, altura(botao_opcao) +  8 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_fita,      mouse_xy_ajustado, true);
            if (botao(6, altura(botao_opcao) + 10 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_numero,    mouse_xy_ajustado, true);
            if (botao(6, altura(botao_opcao) + 11 * altura(botao_bloco) + 8 + offset_bloco, largura(miniaturas[0]), altura(miniaturas[0]), offset_bloco, lista->ativo) && mouse_clicar[mouse_esq]) lista->criar_bloco(bloco_VF,        mouse_xy_ajustado, true);
		}
//
		else if (blocos_inicio && vrep_conectado) { //se o programa estiver no modo de rodar (por isso o else), houver um bloco de inicio e a memória compartilhada tiver sido encontrada o fluxograma pode rodar
			if (*(feedback) == -10) { //se o programa da api retornar -10, ele terminou de executar
                vrep_conectado = false;
                api_rodando = false;
                modo = desenhando;
			}
			else {
                lista->rodar();
                if (!lista->ativo) { //se não houver bloco ativo depois de rodar o fluxograma ele chegou ao fim, então
                    modo = desenhando;
                    *command_var = 0; //e nenhum comando novo é mandado
                }
            }
        }

		//debug

//se há um bloco do topo, imprime seu ponteiro, seu tipo, sua posição,  e todos os campos existentes de var_int e var_pont
		scprintf(posicao_y, 600, 650);
		scprintf(nova_posicao_y, 600, 630);
		if (bloco_topo != NULL) {
			scprintf(bloco_topo, 600, 690);
			scprintf(bloco_topo->tipo, 600, 700);
			scprintf(bloco_topo->var_pont[0], 600, 710);
			scprintf(bloco_topo->pos_x, 600, 720);
			scprintf(bloco_topo->pos_y, 600, 730);

			if (bloco_topo->tipo < 3 || bloco_topo->tipo == bloco_juncao || bloco_topo->tipo == bloco_repeticao) {
				scprintf(bloco_topo->var_pont[1], 600, 740);
				if (bloco_topo->tipo != linha) scprintf(bloco_topo->var_pont[2], 600, 750);
			}
			if (bloco_topo->tipo == bloco_decisao || bloco_topo->tipo == bloco_repeticao) scprintf(bloco_topo->var_pont[3], 600, 760);
            if (bloco_topo->tipo == bloco_decisao) {
				scprintf(bloco_topo->var_pont[4], 600, 770);
			}


			if (bloco_topo->tipo >= primeiro_bloco_trava - 1 || bloco_topo->tipo == linha || bloco_topo->tipo == bloco_repeticao) scprintf(bloco_topo->var_int[0], 600, 780);
			if ((bloco_topo->tipo >= primeiro_bloco_trava - 1 && bloco_topo->tipo != bloco_som) || bloco_topo->tipo == linha || bloco_topo->tipo == bloco_repeticao) scprintf(bloco_topo->var_int[1], 600, 790);
			if (bloco_topo->tipo == bloco_fita || bloco_topo->tipo == bloco_numero || bloco_topo->tipo == linha) scprintf(bloco_topo->var_int[2], 600, 800);
			if (bloco_topo->tipo == linha) scprintf(bloco_topo->var_int[3], 600, 810);
				}
        //mostra os estados das variaveis de comunicação
            scprintf(*command_var, 650, 300);
            if (!modelo) scprintf(*(feedback), 650, 310);
        if (vrep_conectado) {
            if (modelo){
            scprintf((sensores_fita.first)[0], 180, 15);
            scprintf((sensores_fita.first)[1], 190, 15);
            scprintf((sensores_fita.first)[2], 200, 15);
            scprintf((sensores_fita.first)[3], 210, 15);
            scprintf((sensores_fita.first)[4], 220, 15);

            scprintf((sensores_ultrassom.first)[0], 180, 25);
            scprintf((sensores_ultrassom.first)[1], 190, 25);
            scprintf((sensores_ultrassom.first)[2], 200, 25);
            scprintf((sensores_ultrassom.first)[3], 210, 25);
            scprintf((sensores_ultrassom.first)[4], 220, 25);

            scprintf((distancias_ultrassom.first)[0], 90, 40);
            scprintf((distancias_ultrassom.first)[1], 145, 40);
            scprintf((distancias_ultrassom.first)[2], 200, 40);
            scprintf((distancias_ultrassom.first)[3], 255, 40);
            scprintf((distancias_ultrassom.first)[4], 310, 40);

            scprintf((posicao_plana.first)[0], 145, 50);
            scprintf((posicao_plana.first)[1], 200, 50);
            scprintf((posicao_plana.first)[2], 255, 50);

            scprintf((posicao_angular.first)[0], 145, 60);
            scprintf((posicao_angular.first)[1], 200, 60);
            scprintf((posicao_angular.first)[2], 255, 60);
            }
        }
        scprintf(mouse_x, 600, 660);
        scprintf(mouse_y, 600, 670);
        scprintf(dmouse_x, 630, 660);
        scprintf(dmouse_y, 630, 670);


        //barra de rolagem

        int sobre_barra = 0; //variável que armazena se o mouse está sobre a barra de rolagem
        if (mouse_dentro_ret(dimensoes_tela_x - largura_barra_rolagem, 0, dimensoes_tela_x, dimensoes_tela_y)) { //se o mouse estiver dentro da área onde a barra de rolagem fica, verifica o resto

            //verifica se o mouse realmente está sobre a barra
            if (mouse_y > nova_posicao_y/proporcao_pixel_maximo && mouse_y < (posicao_y + dimensoes_tela_y)/ proporcao_pixel_maximo && !barra_posicao_ativa) sobre_barra = 1;


            if (mouse_clicar[mouse_esq]) {
                if (!sobre_barra) {
                //se não estiver, a barra "pula" de forma a ficar centralizada no mouse
                    nova_posicao_y = mouse_y*proporcao_pixel_maximo - dimensoes_tela_y/2;
                    if (nova_posicao_y < 0) nova_posicao_y = 0;
                    else if (nova_posicao_y > dimensao_vertical_maxima - dimensoes_tela_y) nova_posicao_y = dimensao_vertical_maxima;
                }
                //e de qualquer jeito, se torna ativa
                barra_posicao_ativa = true;
                dmouse_y = mouse_y - nova_posicao_y/proporcao_pixel_maximo;
            }
		}
        if (barra_posicao_ativa) { //se a barra estiver ativa e o botão esquerdo do mouse estiver sendo segurado, continua atualizando
        //a posição da barra
            if (mouse_segurar[mouse_esq]) {
                nova_posicao_y = (mouse_y - dmouse_y)*proporcao_pixel_maximo;
            }
            else if (mouse_soltar[mouse_esq]) { //se o botão do mouse for solto, para de atualizar
                barra_posicao_ativa = false;
                dmouse_y = 0;
            }
        }

        //desenha a parte da tela ligeiramente mais escura onde fica a barra
		al_draw_filled_rectangle( dimensoes_tela_x - largura_barra_rolagem, 0, dimensoes_tela_x, dimensoes_tela_y, al_map_rgb(100, 100, 100));

        //desenha a barra em si
        al_draw_filled_rectangle(dimensoes_tela_x - largura_barra_rolagem , nova_posicao_y/proporcao_pixel_maximo, dimensoes_tela_x , (posicao_y + dimensoes_tela_y)/ proporcao_pixel_maximo, cinza);

		//desenha a moldura da barra de rolagem, em branco se o mouse estiver sobre ela e em preto em caso contrário
		al_draw_rectangle(dimensoes_tela_x - largura_barra_rolagem , nova_posicao_y/proporcao_pixel_maximo, dimensoes_tela_x , (posicao_y + dimensoes_tela_y)/ proporcao_pixel_maximo, al_map_rgb(255 * (sobre_barra | barra_posicao_ativa), 255 * (sobre_barra | barra_posicao_ativa), 255 * (sobre_barra | barra_posicao_ativa)), 1);


		//se um bloco estiver sendo arrastado para cima ou para baixo (saindo da tela), faz a tela rolar com ele
		//de acordo com quanto tempo passou

		if (lista->ativo && ciclos_tempo > 0) {

            //se o bloco ativo for uma linha, arrasta a tela de acordo com a posição do mouse, que também é o ponto solto da linha
            if (lista->bloco_ativo->tipo == linha) {
                if (mouse_y < 0) nova_posicao_y = posicao_y + (mouse_y)*velocidade_rolagem;
                if (mouse_y > dimensoes_tela_y) nova_posicao_y = posicao_y + ((mouse_y - dimensoes_tela_y))*velocidade_rolagem;
            }
            //se for um bloco normal, arrasta a tela de acordo com quanto do bloco está fora da tela
		    else if (lista->bloco_ativo->tipo < primeiro_bloco_trava) {
                if (lista->bloco_ativo->pos_y < nova_posicao_y ) nova_posicao_y = nova_posicao_y + (lista->bloco_ativo->pos_y - nova_posicao_y)*velocidade_rolagem;
                else if (lista->bloco_ativo->pos_y + al_get_bitmap_height(BLOCO[lista->bloco_ativo->tipo-1][0]) > dimensoes_tela_y + nova_posicao_y) nova_posicao_y = nova_posicao_y + (lista->bloco_ativo->pos_y + al_get_bitmap_height(BLOCO[lista->bloco_ativo->tipo-1][0]) - (dimensoes_tela_y + nova_posicao_y))*velocidade_rolagem;
            }
            //o mesmo para blocos de trava
            else {
                if (lista->bloco_ativo->pos_y < nova_posicao_y) nova_posicao_y = nova_posicao_y + (lista->bloco_ativo->pos_y - nova_posicao_y)*velocidade_rolagem;
                else if (lista->bloco_ativo->pos_y + al_get_bitmap_height(voltita) > dimensoes_tela_y + nova_posicao_y) nova_posicao_y = nova_posicao_y + (lista->bloco_ativo->pos_y + al_get_bitmap_height(voltita) - (dimensoes_tela_y + nova_posicao_y))*velocidade_rolagem;
            }
            //se a posição nova da tela ficar fora dos limites definidos (0 e dimensão_vertical_maxima), coloca elas nesses limites
            if (nova_posicao_y < 0) nova_posicao_y = 0;
            else if (nova_posicao_y > dimensao_vertical_maxima - dimensoes_tela_y) nova_posicao_y = dimensao_vertical_maxima - dimensoes_tela_y;
            //faz com que a posição do bloco ativo seja atualizada para continuar sendo desenhada no mesmo lugar na tela,
            //pois sem fazer isso o bloco ativo fica "pulando"
            if (nova_posicao_y != posicao_y && modo == desenhando) lista->bloco_ativo->pos_y += nova_posicao_y - posicao_y;
		}


        al_flip_display();
		al_clear_to_color(cinza); //cor provisória escolhida arbitrariamente
		if (ciclos_tempo > 0) ciclos_tempo--; //diz que um periodo de tempo já foi processado
	}
	//manda o comando -10, indicando para o receptor que o programa acabou
	*command_var = -10;
	//destroi os objetos usados pelo allegro
	al_destroy_bitmap(seta);
	al_destroy_bitmap(voltita);
	al_destroy_bitmap(ultrassom);
	al_destroy_bitmap(logico_v);
	al_destroy_bitmap(logico_f);
	al_destroy_bitmap(fita);
	al_destroy_bitmap(PONTO[0]);
	al_destroy_bitmap(PONTO[1]);
	for(int i = 0; i < primeiro_bloco_trava - 1; i++) {
        for(int j = 0; j < 4; j++) al_destroy_bitmap(BLOCO[i][j]);
	}
    for(int i = 0; i< 10; i++)	al_destroy_bitmap(NUMERO[i]);
	al_destroy_bitmap(vrep);
	al_destroy_bitmap(menu);
	al_destroy_display(tela);
	al_destroy_font(fonte);

    coord.destruir();

	delete lista; //destroi a lista e todos os blocos

    al_destroy_timer(temporizador);

	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
	al_shutdown_native_dialog_addon();
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	//destroi a memória compartilhada
	while (*feedback != -10 && api_rodando);
	shared_memory_object::remove(SENDER_MEMORY_NAME);
	delete memoria;

	return 0; //por enquanto só formalidade
}
