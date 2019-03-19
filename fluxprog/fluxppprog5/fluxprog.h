#ifndef FLUXPROG_H_INCLUDED
#define FLUXPROG_H_INCLUDED
#include <allegro5/allegro.h> //biblioteca matriz do allegro
#include <allegro5/allegro_color.h> //biblioteca para rotinas de cor
#include <allegro5/allegro_primitives.h> //biblioteca para desenho de formas geométricas
#include <allegro5/allegro_image.h> // biblioteca para importação e desenho de imagens
#include <allegro5/allegro_native_dialog.h> //biblioteca para uso de diálogos nativos
#include <allegro5/allegro_font.h> // biblioteca para escrita/ uso de fontes
#include <allegro5/allegro_ttf.h> // biblioteca para uso de fontes TrueType
#include <iostream> // usada para debugging
#include <boost/interprocess/managed_shared_memory.hpp> //biblioteca para memória compartilhada
#include <boost/interprocess/offset_ptr.hpp> //biblioteca para memória compartilhada
#include <math.h>
#include <fstream>

#define debug(X) if(1)cout<<X<<endl<<flush

const int modelo = 0; //determina se o programa deve usar o modelo de comunicação com o vrep
//q apenas manda os comandos (0)ou o que controla ele e recebe os sensores(1)

using namespace boost::interprocess;
using namespace std;
class bloco;
extern ALLEGRO_COLOR preto, branco, cinza; //objetos contendo cores
extern ALLEGRO_DISPLAY *tela;
extern ALLEGRO_BITMAP *seta, *voltita, *fita, *ultrassom, *NUMERO[10], *logico_v, *logico_f; //bitmaps para os blocos de travamento (os que travam nos blocos maiores)
extern ALLEGRO_BITMAP *BLOCO[6][4], *PONTO[2]; //bitmaps para os blocos e suas variações (inativo, destacado pelo mouse, sendo movido e sendo rodado) e para os pontos de junção dos blocos (inativo e destacado)
extern ALLEGRO_FONT *fonte; // variável que abriga a fonte usada
extern int modo; // se estiver em -1, o programa está sendo construído. Se em 0, está pausado, e se em 1, rodando
extern bool blocos_inicio; //monitora o número de blocos de início: se não houver ao menos 1, não é possível começar a rodar o programa;
extern int dmouse_x, dmouse_y; //variáveis usadas no controle do mouse
extern bloco* bloco_topo; // se um bloco estiver sendo desenhado acima de todos os outros nos quais há colisão com o mouse, esse bloco será indicado aqui
extern float escala; //utilizado para mostrar o nível de zoom ~(não implementado)
extern bool bloco_enviado; //controla se o bloco ativo atual enquanto o programa roda já enviou comandos para a memória compartilhada
extern bool mouse_clicar[3]; //diz se, entre o último ciclo e o presente, o usuário apertou o botão da esquerda, direita ou meio do mouse
extern bool mouse_segurar[3]; //diz se, entre o ciclo anterior e o presente, o usuário não parou de segurar algum dos  botões do mouse
extern bool mouse_soltar[3]; //diz se, entre o ciclo anterior e o presente, o usuário soltou algum dos  botões do mouse
extern int mouse_x, mouse_y; //indicam a posição atual do mouse nos eixos x e y
extern int *command_var, *feedback, *bluetooth; //variaveis que mandam comandos
extern float posicao_y;
extern float monitor_comprimento, monitor_altura;
extern char caractere_pressionado[2]; //o primeiro campo guarda qual caractere foi pressionado, o segundo marca se foi uma tecla normal '\0'
//um número no numpad 'N' , um numero normal 'n' ou uma seta 'S'
//extern std::pair<int*, managed_shared_memory::size_type> feedback; //variável que recebe a resposta ao comando do simulador
//posicao e retorno dos sensores do robô do simulador
extern std::pair<float*, managed_shared_memory::size_type> posicao_plana, posicao_angular, distancias_ultrassom;
extern std::pair<unsigned char*, managed_shared_memory::size_type> sensores_fita, sensores_ultrassom;
extern float posicao_plana_inical[3], posicao_angular_inicial[3];
extern bool possibilidade_clique_duplo;
extern uint8_t tempo_max_clique_duplo;

//defines para os modos
#define desenhando -1
#define pausado 0
#define rodando 1

//verifica se uma variável foi alocada corretamente
#define verificar_alocacao(X, Y) if(!X) {cout << "problema na inicializacao de " << Y <<endl; return -1;}

//dimensões da tela quando o programa abre
#define dimensoes_tela_inicial_x 700
#define dimensoes_tela_inicial_y 900

//determinam o tamanho atual da tela
#define dimensoes_tela_x al_get_display_width(tela)
#define dimensoes_tela_y al_get_display_height(tela)

//máximo que pode ser descido com a barra de rolagem
#define dimensao_vertical_maxima 4000.0

//quanto um pixel na barra de rolagem corresponde ao que vai descer na tela
#define proporcao_pixel_maximo (dimensao_vertical_maxima / dimensoes_tela_y )

#define velocidade_rolagem 0.4 // velocidade com a qual se rola a tela com a roda do mouse

#define intervalo_eventos_temporizador 0.04 //dado em segundos
//tempo máximo que o usuário pode esperar entre dois cliques para que seja considerado um clique duplo
#define tempo_maximo_clique_duplo 0.5

//usados para calcular distância absoluta
#define MAX(X, Y) ((X>=Y) ? X : Y)
#define MIN(X, Y) ((X<=Y) ? X : Y)
#define DIS(X, Y) (MAX(X, Y) - MIN(X, Y))

#define mouse_cima 1 //utilizado quando se quer testar se a roda do mouse foi rodada significativamente para cima
#define mouse_baixo -1 //o mesmo para baixo
#define mouse_rola 0 //o mesmo independentemente da direção
#define mouse_esq 0 // usada em conjunto com press_mouse, release_mouse e hold_mouse quando se quer testar o botão da esquerda
#define mouse_dir 1 //idem para o botão da direita
#define mouse_meio 2 //idem para o botão do meio

//"apelidos" mais curtos para as funções
#define largura al_get_bitmap_width
#define altura al_get_bitmap_height

//usados para referenciar os tipos de bloco
#define linha           0
#define bloco_decisao   1
#define bloco_acao      2
#define bloco_inicio    3
#define bloco_fim       4
#define bloco_juncao    5
#define bloco_repeticao 6
#define bloco_andar     7
#define bloco_virar     8
#define bloco_fita      9
#define bloco_som       10
#define bloco_numero    11
#define bloco_VF        12

#define primeiro_bloco_trava 7 //usado quando se quer especificar que os blocos a serem tratados são os normais ou os que travam
#define primeiro_bloco_trava_logico 9 //usado para especificar que os blocos a serem tratados são os que travam no bloco lógico (ou não)

//nomes para as junções dos blocos (XYZ X= tipo de bloco Y= entrada(0) ou saída(1) Z= número)
//logo, ponto/100 = tipo de bloco; ((ponto%100)/10) = entrada ou saída; ponto%10 -1 = numero
// E = entrada, S = saida
#define dec_E        101
#define dec_S_nao    112
#define dec_S_sim    113
#define acao_E       201
#define acao_S       212
#define ini_S        311
#define fim_E        401
#define jun_E_esq    501
#define jun_E_dir    502
#define jun_S        513
#define rep_E        601
#define rep_loop_ini 612
#define rep_loop_fim 603
#define rep_S        614


//definem as coordenadas do canto superior esquerdo dos quadrados onde ficam os blocos que travam
#define qua_acao_x 34 //qua=quadrado, acao = bloco de ação
#define qua_acao_y 8
#define qua_log1_x 32 //log1 = quadrado da esquerda do bloco lógico
#define qua_log2_x 64
#define qua_log_y  22 //como o primeiro e segundo estão alinhados no eixo y só é necessária uma constante dessa
#define qua_rep_x  27
#define qua_rep1_y 16
#define qua_rep2_y 48
//definem as coordenadas dos 5 sensores de fita dentro do bloco do sensor de fita (para desenho)
#define snsr1_x 2
#define snsr2_x 7
#define snsr3_x 11
#define snsr4_x 15
#define snsr5_x 20

//como os blocos 1 e 5 estao alinhados no eixo y, só é necessário uma constante para a coordenada y de ambos (o mesmo vale para 2 e 4)
#define snsr15_y 4
#define snsr24_y 2
#define snsr3_y  1

//número de ponteiros necesários no vetor de ponteiros para cada bloco (todos os de trava precisam de apenas 1 ponteiro (por enquanto))
#define ponts_linha     2
#define ponts_decisao   5
#define ponts_acao      3
#define ponts_inicio    1
#define ponts_fim       1
#define ponts_juncao    3
#define ponts_repeticao 4
#define ponts_trava     1

//numero de posições necessárias no vetor de colisão para cada bloco (todos os de trava precisam de apenas uma posição (por enquanto))
#define cols_linha     1
#define cols_decisao   4
#define cols_acao      3
#define cols_inicio    2
#define cols_fim       2
#define cols_juncao    4
#define cols_repeticao 5
#define cols_trava     1

//numero de posições necessárias no vetor de inteiros para cada bloco (por enquanto necessario para a linha e os de trava)
#define ints_linha     4
#define ints_repeticao 2
#define ints_andar     2
#define ints_virar     2
#define ints_fita      3
#define ints_som       4
#define ints_VF        2

//nomes para as posições de var_int de cada tipo de bloco
#define lin_pos2_x      var_int[0] //coordenada x do segundo ponto
#define lin_pos2_y      var_int[1] //coordenada y do segundo ponto
#define lin_ponto1      var_int[2] //juncao a qual o primeiro ponto está ligado
#define lin_ponto2      var_int[3] //juncao a qual o segundo ponto está ligado
#define rep_it_total    var_int[0] //numero total de iterações
#define rep_it_faltando var_int[1] //numero que falta de iterações
#define trava_posicao   var_int[0] // posição a qual um bloco de trava está ligado ( -1 = solto, 0 = acao, 1 = decisao na esquerda, 2 - decisao na direita)
#define and_sentido     var_int[1] //sentido indicado pelo bloco de andar ( -1 = para trás, 1 = para frente)
#define vir_dir         var_int[1] //direção para qual o robô deve virar ( -1 = direita, 1 = esquerda)
#define fita_sensor     var_int[1] // sensor de fita que deve ser testado
#define fita_cor        var_int[2] //cor que deve ser testada
#define som_sensor      var_int[1] //sensor de ultrassom que deve ser testado
#define VF_estado       var_int[1] //define se o bloco indica verdadeiro (1) ou falso (-1)


c

#define sensibilidade_roda_mouse 20 //define quantos pixels a tela sobe ou desce para cada ponto na rolagem do mouse
#define tolerancia_linha 10 // define a quantos pixels de distância o mouse pode estar de uma linha para que o programa considere que ele está sobre a linha

//offset entre a moldura do botão e o o botão em si
#define offset_opcao 4
#define offset_bloco 2

#define arquivo_backup "backup.fp"
//macro para ter certeza que não haverá bloco ativo
#define resetar_lista_blocos() {lista->ativo = false; lista->bloco_ativo->ativo = false; lista->bloco_ativo = NULL;}

//macro para inicializar as variáveis globais
#define inic_fluxprog() dmouse_x = 0; dmouse_y = 0; blocos_inicio = 0; modo = desenhando; escala = 1; mouse_clicar[0] = false; mouse_clicar[1] = false; mouse_clicar[2] = false; mouse_segurar[0] = false; mouse_segurar[1] = false; mouse_segurar[2] = false; mouse_soltar[0] = false; mouse_soltar[1] = false; mouse_soltar[2] = false; posicao_y = 0;possibilidade_clique_duplo = false;tempo_max_clique_duplo = 0;


//determina se o ponteiro do mouse está dentro do retângulo determinado pelos dois pontos
#define mouse_dentro_ret(X1, Y1, X2, Y2) ( mouse_x > X1 && mouse_y > Y1 && mouse_x < X2 && mouse_y < Y2)

//usados para diminuir o tamanho das linhas de código, já que esses termos são repetidos centenas de vezes
#define mouse_xy mouse_x, mouse_y
#define mouse_xy_ajustado mouse_x, mouse_y + posicao_y
#define mouse_y_ajustado mouse_y + posicao_y
#define bloco_xy pos_x, pos_y
#define bloco_ativo_xy  l->bloco_ativo->pos_x, l->bloco_ativo->pos_y
#define pos_ponto(X) pos_x + coord.pontos_x(X), pos_y + coord.pontos_y(X)
#define pos_linha(X) pos_x + coord.linhas_x(X), pos_y + coord.linhas_y(X)
#define meio_tela dimensoes_tela_x/2, dimensoes_tela_y/2

//macros que definem os nomes das memorias compartilhadas e suas variáveis (necessário para o funcionamento da memória)
#define SENDER_MEMORY_NAME      "memoria"
#define RECEIVER_MEMORY_NAME    "memoria2"
#define MEMORY_SIZE             65536
#define COMMAND_VARIABLE_NAME   "comando1"
#define FEEDBACK_VARIABLE_NAME  "comando2"
#define SENSOR_VISAO            "reading_VS"
#define SENSOR_ULTRASSOM        "readin_U"
#define POSICAO_DETECTADA       "detectedObjet_U"
#define POSICAO                 "linPosition"
#define ANGULAR                 "angPosition"
#define BLUETOOTH_ENABLE        "bluetooth"

//classe que contem a lista onde os blocos são armazenados
class lista_blocos {
public:
	bloco* inicio; //guarda o inicio da lista encadeada
	bloco* bloco_ativo; // ponteiro para o atual bloco ativo
	bool ativo; // marca se há um bloco ativo no momento
	lista_blocos();
	~lista_blocos();
	void atualizar(); //atualiza a lista de blocos
    bool carregar(const char* caminho); //carrega um fluxograma salvo
	void checar_colisoes(int x, int y); // verifica qual é o bloco do topo e com quais blocos (ou pontos de junção dentro dos blocos) houve colisão com o mouse
	//função sobrecarregada cuja função é adicionar um novo bloco a lista. a primeira versão adiciona blocos normais e os de trava, e a segunda é especifica para as linhas
	bloco* criar_bloco(int tipo_novo_bloco, int x, int y, bool esta_ativo);
	bloco* criar_bloco(int tipo_novo_bloco, int x, int y); //cria blocos para a função de carregar: sempre inativos e sem fazer backup
	bloco* criar_bloco(int point, bloco* xbloco);
	void desenhar(); //desenha os blocos
	void rodar(); //roda o fluxograma
    bool salvar(const char* caminho); //salva um fluxograma

};

//classe que contem cada bloco individual
class bloco {
public:
	bool ativo, marcado_para_destruir;
	bool *col; // ponteiro usado para criar o vetor que armazena se houve colisão com o bloco ou uma de suas partes ou não. Como diferentes blocos tem diferente número de junções, economiza-se memória alocando apenas o suficiente para o tipo de bloco específico
	short int tipo; // mostra qual é o tipo do bloco.
	int pos_x, pos_y;
	int *var_int; //ponteiro usado para criar o vetor que armazena as variáveis inteiras usadas dentro do bloco (mesma situacao que col)
	bloco** var_pont; //ponteiro usado para criar vetor de ponteiros para outros blocos, usada para indicar a quais outros blocos um bloco está ligado (mesma situacao que col e var_int)
	bloco* prox_bloco;
	//as proximas funcoes sao analogas as da classe lista_blocos, mas agindo no bloco individual. mover() serve apenas para mover o bloco
	bloco(int tipo_novo_bloco, int x, int y, bool esta_ativo);
	bloco(int ponto, bloco* xbloco);
	bloco();
	int atualizar(lista_blocos* l);
	void desenhar(lista_blocos *l);
	void destruir(lista_blocos* l);
	void mover();
	void prender(lista_blocos* l, int ponto); //usado para travar uma linha na juncao do bloco
	bloco* rodar();
};

bool colisao(int x1, int y1, ALLEGRO_BITMAP* obj1, int x2, int y2, ALLEGRO_BITMAP* obj2); //retorna se dois blocos estão se sobrepondo pelo método "pixel perfect"
bool mouse_sobre(int x, int y, ALLEGRO_BITMAP* obj); //retorna se o mouse está sobre um bloco
bool na_linha(int x1, int y1, int x2, int y2, int x, int y); //retorna se um ponto está acima de um segmento de linha (de acordo com o valor de sensibilidade_linha)

//scprintf = screen print function
void scprintf(int n, float x, float y); //transforma um número em um vetor de caracteres para a função de escrever do allegro, que só aceita esse formato, e então escreve o número na coordenada dada
void scprintf(float f, float x, float y); //a mesma coisa que a anterior, mas para um float
void scprintf(double d, float x, float y); //a mesma coisa que a anterior, mas para um double
void scprintf(bloco* p, float x, float y); //a mesma coisa que a anterior, mas com um ponteiro para um bloco

bool botao(float x, float y, float dx, float dy, float offset, bool ativo); //verifica se o mouse está sobre um botão, desenha o retângulo de destaque se estiver, e retorna verdadeiro ou falso correspondentemente

//objeto que armazena as constantes usadas no desenho
class cord {
    private:
        int ***lin; //ponteiro para vetor multidimensional que armazena as coordenadas relativas (a origem do bloco a qual estao ligadas) das linhas
        int ***pon; //ponteiro para vetor multidimensional que armazena as coordenadas relativas (a origem do bloco a qual estao ligadas) das junçoes
    public :
        void inic(); // inicializa as variáveis
        void destruir(); //desaloca a memória utilizada
        //retorna o valor do offset de desenho de uma linha de acordo com o ponto na qual está conectada
        int linhas_x(int X) {return lin[X/100 - 1][X%10-1][0];}
        int linhas_y(int X) {return lin[X/100 - 1][X%10-1][1];}
        //retorna o valor do offset de desenho de uma ponto de junção de acordo com o ponto na qual está conectada
        int pontos_x(int X) {return pon[X/100 - 1][X%10-1][0];}
        int pontos_y(int X) {return pon[X/100 - 1][X%10-1][1];}
};
extern cord coord;

#endif // FLUXPROG_H_INCLUDED
#pragma once
