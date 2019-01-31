#include "fluxprog.h"
ALLEGRO_COLOR preto, branco, cinza;
ALLEGRO_DISPLAY *tela;
ALLEGRO_BITMAP *seta, *voltita, *fita, *ultrassom, *BLOCO[6][4], *PONTO[2], *NUMERO[10], *logico_v, *logico_f;
ALLEGRO_FONT *fonte;
int modo,  dmouse_x, dmouse_y;
bloco* bloco_topo;
float escala;
bool bloco_enviado, blocos_inicio;
bool mouse_clicar[3];
bool mouse_segurar[3];
bool mouse_soltar[3];
short int mouse_rolar;
int mouse_x, mouse_y;
int* command_var, *feedback, *bluetooth;
float posicao_y;
float monitor_comprimento, monitor_altura;
char caractere_pressionado[2];
bool possibilidade_clique_duplo;
uint8_t tempo_max_clique_duplo;
cord coord;
//std::pair<int*, managed_shared_memory::size_type> feedback;
std::pair<float*, managed_shared_memory::size_type> posicao_plana, posicao_angular, distancias_ultrassom;
std::pair<unsigned char*, managed_shared_memory::size_type> sensores_fita, sensores_ultrassom;
float posicao_plana_inical[3], posicao_angular_inicial[3];

lista_blocos::lista_blocos() {
	inicio = NULL;
	bloco_ativo = NULL;
	ativo = false;
}

lista_blocos::~lista_blocos() {
    bloco* aux;
	while (inicio != NULL) { //percorre a lista desalocando a memoria para os vetores usados no bloco e então para o bloco em si
		aux = inicio;
		inicio = inicio->prox_bloco;
		delete aux->col;
		delete aux->var_pont;
		if (aux->tipo == linha || aux->tipo>= primeiro_bloco_trava || aux->tipo == bloco_repeticao) delete aux->var_int;
		delete aux;
	}
}

void lista_blocos::atualizar() {
	bloco* aux = inicio; //percorre a lista
	bloco* aux2 = inicio; //armazena o penultimo bloco a ser atualizado, pois e necessario saber qual é o bloco anterior para remover um bloco da lista
	bloco* anterior_ao_ativo = NULL; //como a lista é desenhada com o primeiro da lista sendo desenhado por primeiro, ela fica atrás de todos
	//os outros, que são impressos depois. Por isso, é necessário que o bloco ativo seja o último da lista, para que seja desenhado por
	//último e por isso fique por cima. Essa variavel é usada para "arrumar" a lista quando o bloco ativo é jogado para o fim.
	bool varaux;//armazena se o bloco sendo atualizado é ativo ou não
	while (aux != NULL) {
		if (!aux->marcado_para_destruir) {
			varaux = aux->atualizar(this);
			//se o bloco atualizado é o ativo, guarda o endereço dele e o do anterior para atualizar a lista
			if (varaux) {
				bloco_ativo = aux;
				if (aux2 != aux) anterior_ao_ativo = aux2;
			}
            //atualiza as variaveis para continuar percorrendo a lista
			aux2 = aux;
			aux = aux->prox_bloco;
		}
		else {
			if (aux == aux2) { //se aux == aux2, o bloco avaliado é o primeiro da lista (pois o valor dessas variaveis só passa a ser diferente no final do primeiro loop)
				inicio = inicio->prox_bloco; //atualiza o inicio da lista
				aux->destruir(this);
				aux2 = inicio; //faz com que a função recomeçe a percorrer a lista com o novo inicio
				aux = inicio;
			}
			else {
				aux2->prox_bloco = aux->prox_bloco; //faz o bloco anterior ao bloco que deve ser destruído apontar para o bloco após o bloco a ser destruido, fechando o espaço que seria deixado pelo bloco destruido na lista
				aux->destruir(this);
				aux = aux2->prox_bloco;
			}
		}
	}
	//como o loop para quando aux = NULL, aux2 será o último bloco da lista

	//se um bloco que não for o último da lista for o ativo, o coloca como ultimo da lista
	if (bloco_ativo != NULL && bloco_ativo != aux2) {

		//atualiza o bloco anterior ao ativo (ou o início da lista para que apontem para o bloco após o ativo, mantendo o fluxo da lista)
		if (anterior_ao_ativo == NULL) inicio = bloco_ativo->prox_bloco;
		else anterior_ao_ativo->prox_bloco = bloco_ativo->prox_bloco;
		//faz o último bloco apontar pára o bloco ativo
		aux2->prox_bloco = bloco_ativo;
		//faz o bloco ativo apontar para nada (mostrando que ele é o ultimo da lista)

		bloco_ativo->prox_bloco = NULL;
	}

	if (bloco_ativo != NULL) ativo = true;
	else ativo = false;
}

bool lista_blocos::carregar(const char* caminho) {
    ifstream arquivo (caminho); // abre o arquivo a ser lido
    if(!arquivo.is_open()) return false; //verifica se ele foi aberto
    int n_blocos;
    string linha_arquivo;
    size_t pos; //variável que armazenará qual  a posição logo apos um int terminar dentro de uma string (ver definição de stoi)
    if (!getline(arquivo, linha_arquivo)) return false; //carrega a primeira linha do arquivo para a string, verificando se houve sucesso
    n_blocos = stoi(linha_arquivo, &pos);//transforma a primeira linha (que contem apenas o numero de blocos na lista)
    bloco* vetor_blocos[n_blocos]; //cria um vetor que conterá todos os blocos (pois assim é possível relacionar o indice de cada bloco na lista com o numero associado aos ponteiros no arquivo)
    int ponteiros[n_blocos][5]; //cria uma matriz contendo os numeros correspondentes aos ponteiros do bloco
    for (int i = 0; i < n_blocos; i++){
        int tipo = -1, pos_x = -1, pos_y = -1; //variáveis que armazenam as variáveis que precisam ser utilizadas para criar os blocos
        if (!getline(arquivo, linha_arquivo)) return false;//carrega a linha correspondente ao bloco atual em linha_arquivo
        else {
            tipo = stoi(linha_arquivo, &pos); //lê o primeiro int encontrado dentro da string e armazena-o na variável que diz qual será o tipo do bloco
            linha_arquivo = linha_arquivo.substr(pos); //tira da string esse primeiro int, para que a proxima vez que stoi seja chamada encontre o próximo int
            pos_x = stoi(linha_arquivo, &pos); //o mesmo para a posição no eixo x
            linha_arquivo = linha_arquivo.substr(pos);
            pos_y = stoi(linha_arquivo, &pos); // eixo y
            linha_arquivo = linha_arquivo.substr(pos);
            ponteiros[i][0]= stoi(linha_arquivo, &pos); //primeiro ponteiro
            linha_arquivo = linha_arquivo.substr(pos);
            if (tipo!=linha) {
                vetor_blocos[i] = criar_bloco(tipo, pos_x, pos_y); //se o tipo não for linha (função de criação diferente), cria o bloco normalmente
                //e então armazena os valores das próximas variáveis
                switch (tipo) {
                    case bloco_decisao :
                        for (int j = 1; j < ponts_decisao; j++) { // le da string as posições dos blocos qual esse bloco está ligado e as armazena para que os ponteiros
                        //sejam ligados devidamente quando  todos os blocos tiverem sido criados
                            ponteiros[i][j]= stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                    case bloco_acao :
                        for (int j = 1; j < ponts_acao; j++) {
                            ponteiros[i][j]= stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                        /* como esses blocos só tem um ponteiro e nenhuma variavel inteira, não precisam de tratamento específico
                    case bloco_inicio :
                        break;
                    case bloco_fim :
                        break;
                        */
                    case bloco_juncao :
                        for (int j = 1; j < ponts_juncao; j++) {
                            ponteiros[i][j]= stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                    case bloco_repeticao :
                        for (int j = 1; j < ponts_repeticao; j++) {
                            ponteiros[i][j]= stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        for (int j = 0; j < ints_repeticao; j++) {
                            vetor_blocos[i]->var_int[j] = stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                    //os blocos de trava tambem só tem um ponteiro, e por isso só é preciso guardar o valor das variaveis inteiras de cada um
                    case bloco_andar :
                        for (int j = 0; j < ints_andar; j++) {
                            vetor_blocos[i]->var_int[j] = stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                    case bloco_virar :
                        for (int j = 0; j < ints_virar; j++) {
                            vetor_blocos[i]->var_int[j] = stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                    case bloco_fita :
                        for (int j = 0; j < ints_fita; j++) {
                            vetor_blocos[i]->var_int[j] = stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                    case bloco_som :
                        for (int j = 0; j < ints_som; j++) {
                            vetor_blocos[i]->var_int[j] = stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                    case bloco_numero :
                        for (int j = 0; j < 3; j++) {
                            vetor_blocos[i]->var_int[j] = stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                    case bloco_VF :
                        for (int j = 0; j < ints_VF; j++) {
                            vetor_blocos[i]->var_int[j] = stoi(linha_arquivo, &pos);
                            linha_arquivo = linha_arquivo.substr(pos);
                        }
                        break;
                }
            }
            else { //como a função de criação das linhas tem como parâmetro o ponteiro do bloco ao qual deverá ser ligado, não pode ser utilizada, pois não
            //é possível saber qual será esse ponteiro. Portanto, foi preciso copiar o conteúdo das funçoes lista_blocos :: criar_bloco(int ponto, bloco* xbloco)
            // e bloco :: bloco(int ponto, bloco* xbloco), adaptando-a de acordo com as necessidades atuais
            	vetor_blocos[i] = new bloco(); //aloca memória para o novo bloco
                //coloca ele no início da lista
                vetor_blocos[i]->prox_bloco = inicio;
                inicio = vetor_blocos[i];
                vetor_blocos[i]->ativo = false;
                vetor_blocos[i]->marcado_para_destruir = false;
                vetor_blocos[i]->tipo = linha;
                vetor_blocos[i]->pos_x = pos_x;
                vetor_blocos[i]->pos_y = pos_y;
                vetor_blocos[i]->col = new bool[cols_linha];
                vetor_blocos[i]->col[0] = false;
                vetor_blocos[i]->var_pont = new bloco*[ponts_linha];
                ponteiros[i][1]= stoi(linha_arquivo, &pos);
                linha_arquivo = linha_arquivo.substr(pos);
                vetor_blocos[i]->var_int = new int[ints_linha];
                for (int j = 0; j < ints_linha; j++) {
                    vetor_blocos[i]->var_int[j] = stoi(linha_arquivo, &pos);
                    linha_arquivo = linha_arquivo.substr(pos);
                }
            }

        }
    }
    //depois de criar todos os blocos, escreve os endereços corretos dos ponteiros dos blocos
    for(int i = 0; i < n_blocos; i++) {
        //como todo bloco tem ao menos um ponteiro (e muitos apenas 1), isso pode ser feito independente do tipo de bloco
        vetor_blocos[i]->var_pont[0] = ((ponteiros[i][0]!=-1) ? vetor_blocos[ponteiros[i][0]] : NULL);
        //como os blocos em vetor_blocos estão na ordem em que estão no arquivo, com as posições armazenadas na matriz ponteiros podemos atribuir os valores corretos dos
        //ponteiros usando o ponto correto da matriz como indíce da matriz vetor_blocos, e armazenando o ponteiro localizado nessa posição do vetor na variável do bloco inicial
        //se o valor do ponto na matriz for -1, o bloco não está ligado a nada naquele ponto, e a variável recebe um ponteiro nulo

        switch (vetor_blocos[i]->tipo) {
            case linha :
                for (int j = 1; j < ponts_linha; j++) vetor_blocos[i]->var_pont[j] = ((ponteiros[i][j]!=-1) ? vetor_blocos[ponteiros[i][j]] : NULL);
                break;
            case bloco_decisao :
                for (int j = 1; j < ponts_decisao; j++) vetor_blocos[i]->var_pont[j] = ((ponteiros[i][j]!=-1) ? vetor_blocos[ponteiros[i][j]] : NULL);
                break;
            case bloco_acao :
                for (int j = 1; j < ponts_acao; j++)  vetor_blocos[i]->var_pont[j] = ((ponteiros[i][j]!=-1) ? vetor_blocos[ponteiros[i][j]] : NULL);
                break;
            /* como esses blocos só tem um ponteiro, não precisam de tratamento específico
            case bloco_inicio :
                break;
            case bloco_fim :
                break;
            */
            case bloco_juncao :
                for (int j = 1; j < ponts_juncao; j++) vetor_blocos[i]->var_pont[j] = ((ponteiros[i][j]!=-1) ? vetor_blocos[ponteiros[i][j]] : NULL);
                break;
            case bloco_repeticao :
                for (int j = 1; j < ponts_repeticao; j++) vetor_blocos[i]->var_pont[j] = ((ponteiros[i][j]!=-1) ? vetor_blocos[ponteiros[i][j]] : NULL);
                break;
            /*os blocos de trava tambem só tem um ponteiro
            case bloco_andar :
                break;
            case bloco_virar :
                break;
            case bloco_fita :
                break;
            case bloco_som :
                break;
            case bloco_numero :
                break;
            case bloco_VF :
                break;
            */
        }
   }
    return true;
}

void lista_blocos::checar_colisoes(int x, int y) {
	bloco* aux = inicio; //cria variável para percorrer a lista encadeada
	bloco_topo = NULL;
	while (aux != NULL) {
		if (aux->tipo >= primeiro_bloco_trava) { //se o bloco for um bloco de trava, como ele não tem nenhuma junção, só precisa avaliar se o mouse está sobre ele
			if (mouse_sobre(aux->pos_x, aux->pos_y, seta)) {
				bloco_topo = aux;
				aux->col[0] = true;
			}
			else aux->col[0] = false;
		}
		else if (aux->tipo != linha) { //se o bloco for normal, é preciso checar o proprio bloco e as junçoes
			if (mouse_sobre(aux->pos_x, aux->pos_y, BLOCO[aux->tipo - 1][0])) {
				bloco_topo = aux;
				aux->col[0] = true;
			}
			else aux->col[0] = false;

			switch (aux->tipo) {
                case bloco_decisao:
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(dec_E), aux->pos_y + coord.pontos_y(dec_E), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[1] = true;
                    }
                    else aux->col[1] = false;

                    if (mouse_sobre(aux->pos_x + coord.pontos_x(dec_S_nao), aux->pos_y + coord.pontos_y(dec_S_nao), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[2] = true;
                    }
                    else aux->col[2] = false;

                    if (mouse_sobre(aux->pos_x + coord.pontos_x(dec_S_sim), aux->pos_y + coord.pontos_y(dec_S_sim), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[3] = true;
                    }
                    else aux->col[3] = false;
                    break;
                case bloco_acao:

                    if (mouse_sobre(aux->pos_x + coord.pontos_x(acao_E), aux->pos_y + coord.pontos_y(acao_E), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[1] = true;
                    }
                    else aux->col[1] = false;

                    if (mouse_sobre(aux->pos_x + coord.pontos_x(acao_S), aux->pos_y + coord.pontos_y(acao_S), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[2] = true;
                    }
                    else aux->col[2] = false;
                    break;
                case bloco_inicio:
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(ini_S), aux->pos_y + coord.pontos_y(ini_S), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[1] = true;
                    }
                    else aux->col[1] = false;
                    break;
                case bloco_fim:
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(fim_E), aux->pos_y + coord.pontos_y(fim_E), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[1] = true;
                    }
                    else aux->col[1] = false;
                    break;

                case bloco_juncao:
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(jun_E_esq), aux->pos_y + coord.pontos_y(jun_E_esq), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[1] = true;
                    }
                    else aux->col[1] = false;
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(jun_E_dir), aux->pos_y + coord.pontos_y(jun_E_dir), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[2] = true;
                    }
                    else aux->col[2] = false;
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(jun_S), aux->pos_y + coord.pontos_y(jun_S), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[3] = true;
                    }
                    else aux->col[3] = false;
                    break;
                case bloco_repeticao:
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(rep_E), aux->pos_y + coord.pontos_y(rep_E), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[1] = true;
                    }
                    else aux->col[1] = false;
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(rep_loop_ini), aux->pos_y + coord.pontos_y(rep_loop_ini), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[2] = true;
                    }
                    else aux->col[2] = false;
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(rep_loop_fim), aux->pos_y + coord.pontos_y(rep_loop_fim), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[3] = true;
                    }
                    else aux->col[3] = false;
                    if (mouse_sobre(aux->pos_x + coord.pontos_x(rep_S), aux->pos_y + coord.pontos_y(rep_S), PONTO[0])) {
                        bloco_topo = aux;
                        aux->col[4] = true;
                    }
                    else aux->col[4] = false;
                    break;
            }
		}
		//como os blocos comuns e os de trava foram tratados anteriormente, os unicos que sobram sao as linhas
		//se a linha que está sendo avaliada é a linha ativa ela não pode ser o bloco do topo,
		//pois o bloco do topo precisa ser o bloco na qual se ligaria
        //além disso, nao é possivel selecionar duas linhas ao mesmo tempo, pois só pode haver uma linha ligada a um ponto,
        //e uma linha não pode ser conectada a outra
		else if (aux != bloco_ativo ) {
            aux->col[0] = false; //zera a variável
            if (na_linha(aux->pos_x, aux->pos_y, aux->lin_pos2_x, aux->lin_pos2_y, mouse_xy_ajustado)) aux->col[0] = true; //se o mouse estiver sobre a linha, seta a variável
            if (ativo) {if (bloco_ativo->tipo == linha) aux->col[0] = false;} //se houver um bloco ativo e esse for uma linha, outra linha não pode ser selecionada, e então a variável é zerada
            if (aux->col[0]) bloco_topo = aux; //se realmente houver a colisão, a linha se torna o bloco ativo
		}
		aux = aux->prox_bloco;
	}
	aux = inicio;
}

bloco* lista_blocos::criar_bloco(int tipo, int x, int y, bool esta_ativo) {
	bloco* xbloco = new bloco(tipo, x, y, esta_ativo);
	xbloco->prox_bloco = inicio;
	inicio = xbloco;
    salvar(arquivo_backup); //cada vez que umn bloco é criado ou deletado, o programa salva o fluxograma atual em um backup
    return xbloco;
}

bloco* lista_blocos::criar_bloco(int tipo, int x, int y) {

	bloco* xbloco = new bloco(tipo, x, y, false);
	xbloco->prox_bloco = inicio;
	inicio = xbloco;
    return xbloco;
}
bloco* lista_blocos::criar_bloco(int ponto, bloco* xbloco) {
	bloco* novo = new bloco(ponto, xbloco);
	novo->prox_bloco = inicio;
	inicio = novo;
	salvar(arquivo_backup);//cada vez que umn bloco é criado ou deletado, o programa salva o fluxograma atual em um backup
	return novo;
}

void lista_blocos::desenhar() { //simplesmente percorre a lista encadeada desenhando todos os blocos
	bloco* aux = inicio;

	while (aux != NULL) {
		aux->desenhar(this);
		aux = aux->prox_bloco;
	}

}

void lista_blocos::rodar() {
	if (!ativo) { //se não ha bloco ativo, o programa ainda nao começou a rodar, entao é preciso encontrar um bloco de inicio
		bloco* aux = inicio;
		while (aux != NULL && !ativo) {//percorre a lista encadeada até encontrar um bloco de inicio ou ate o fim
			if (aux->tipo == bloco_inicio) {//se um bloco de inicio é encontrado, ele é ativado e o loop acaba
				ativo = true;
				bloco_ativo = aux;
				bloco_ativo->ativo = true;
			}
			else aux = aux->prox_bloco;
		}
	}
	else {
		bloco_ativo->ativo = false; //desativa o bloco ativo
		bloco_ativo = bloco_ativo->rodar();
		if (bloco_ativo != NULL) bloco_ativo->ativo = true; //ativa o novo bloco ativo, se houver, ou reativa o antigo, se ele não mudou
		else ativo = false; //se não houver novo bloco ativo, o programa parou de rodar, e é necessario registrar isso na lista de blocos

	}

}

bool lista_blocos::salvar(const char* caminho) {
        //percorre a lista encadeada contando quantos blocos existem
        int n_blocos, i, j; //variáveis que armazenam o tamanho da lista de blocos e indices para loops
        bloco *aux = inicio;
        for(n_blocos = 0; aux != NULL; n_blocos++) aux = aux->prox_bloco;
        if (n_blocos == 0) return false;
        ofstream arquivo (caminho); //cria um arquivo novo
        if (!arquivo.is_open()) return false;
            //se houver blocos para salvar, armazenará o ponteiro para cada bloco em um vetor
            //isso é necessário pois, como não sabemos onde os blocos serão criados quando o fluxograma for carregado
            //salvar os ponteiros antigos no ajudará
            //assim, associamos um numero (o indice do vetor) a cada bloco e seu respectivo endereço
            bloco* ponteiros[n_blocos];
            aux = inicio;
            for (i = 0; i < n_blocos; i++) {
                ponteiros[i] = aux;
                aux = aux->prox_bloco;
            }
            arquivo << n_blocos;
            aux = inicio;
            while (aux != NULL) {
                arquivo << "\n"; //começa uma nova linha para o próximo bloco
                arquivo << aux->tipo << " " << aux->pos_x << " " << aux->pos_y << " " ; // manda para o arquivo as variáveis que todos os blocos tem

                //como todo bloco possui ao menos um ponteiro, é preciso salvar o bloco para qual esse ponteiro aponta não importando qual tipo de bloco é
                if (aux->var_pont[0] == NULL) arquivo << "-1 "; // se o ponteiro é nulo, o arquivo receberá -1
                else {//do contrário, receberá o número do bloco a qual aponta
                    for(j = 0; aux->var_pont[0] != ponteiros[j]; j++);
                    arquivo << j << " ";
                }
                switch (aux->tipo) {
                    case linha :
                        if (aux->var_pont[1] == NULL) arquivo << "-1 "; // se o ponteiro é nulo, o arquivo receberá -1
                        else {//do contrário, receberá o número do bloco a qual aponta
                            for(j = 0; aux->var_pont[1] != ponteiros[j]; j++);
                            arquivo << j << " ";
                        }
                        for (j = 0; j < ints_linha; j++) arquivo << aux->var_int[j] << " "; //salva as variaveis inteiras
                        break;
                    case bloco_decisao :
                        for (i = 1; i < ponts_decisao; i++) { //guarda os  ponteiros que faltam do bloco
                            if (aux->var_pont[i] == NULL) arquivo << "-1 "; // se o ponteiro é nulo, o arquivo receberá -1
                            else {//do contrário, receberá o número do bloco a qual aponta
                                for(j = 0; aux->var_pont[i] != ponteiros[j]; j++);
                                arquivo << j << " ";
                            }
                        }
                        break;
                    case bloco_acao :
                        for (i = 1; i < ponts_acao; i++) { //guarda os  ponteiros que faltam do bloco
                            if (aux->var_pont[i] == NULL) arquivo << "-1 "; // se o ponteiro é nulo, o arquivo receberá -1
                            else {//do contrário, receberá o número do bloco a qual aponta
                                for(j = 0; aux->var_pont[i] != ponteiros[j]; j++);
                                arquivo << j << " ";
                            }
                        }
                        break;
                        /* como esses blocos só tem um ponteiro e nenhuma variavel inteira, não precisam de tratamento específico
                    case bloco_inicio :
                        break;
                    case bloco_fim :
                        break;
                        */
                    case bloco_juncao :
                        for (i = 1; i < ponts_juncao; i++) { //guarda os  ponteiros que faltam do bloco
                            if (aux->var_pont[i] == NULL) arquivo << "-1 "; // se o ponteiro é nulo, o arquivo receberá -1
                            else {//do contrário, receberá o número do bloco a qual aponta
                                for(j = 0; aux->var_pont[i] != ponteiros[j]; j++);
                                arquivo << j << " ";
                            }
                        }
                        break;
                    case bloco_repeticao :
                        for (i = 1; i < ponts_repeticao; i++) { //guarda os  ponteiros que faltam do bloco
                            if (aux->var_pont[i] == NULL) arquivo << "-1 "; // se o ponteiro é nulo, o arquivo receberá -1
                            else {//do contrário, receberá o número do bloco a qual aponta
                                for(j = 0; aux->var_pont[i] != ponteiros[j]; j++);
                                arquivo << j << " ";
                            }
                        }
                        for (j = 0; j < ints_repeticao; j++) arquivo << aux->var_int[j] << " "; //salva as variaveis inteiras
                        break;
                    //os blocos de trava tambem só tem um ponteiro, e por isso só é preciso guardar o valor das variaveis inteiras de cada um
                    case bloco_andar :
                        for (j = 0; j < ints_andar; j++) arquivo << aux->var_int[j] << " "; //salva as variaveis inteiras
                        break;
                    case bloco_virar :
                        for (j = 0; j < ints_virar; j++) arquivo << aux->var_int[j] << " "; //salva as variaveis inteiras
                        break;
                    case bloco_fita :
                        for (j = 0; j < ints_fita; j++) arquivo << aux->var_int[j] << " "; //salva as variaveis inteiras
                        break;
                    case bloco_som :
                        for (j = 0; j < ints_som; j++) arquivo << aux->var_int[j] << " "; //salva as variaveis inteiras
                        break;
                    case bloco_numero :
                        for (j = 0; j < 3; j++) arquivo << aux->var_int[j] << " "; //salva as variaveis inteiras
                        break;
                    case bloco_VF :
                        for (j = 0; j < ints_VF; j++) arquivo << aux->var_int[j] << " "; //salva as variaveis inteiras
                        break;
                }
                aux = aux->prox_bloco;
            }
    arquivo.close(); //fecha o arquivo
    return true;
}

bloco::bloco(int tipo_novo_bloco, int x, int y, bool esta_ativo) {
//aloca espaço para e inicializa as variáveis armazenadas nos vetores, de acordo com as necessidades de cada tipo
//a ordem usada para determinar qual espaço cada colisao deve ocupar no vetor e dada pela seguinte logica: col[0] sempre representa colisao com o bloco em si, e entao seguem-se as juncoes, comecando no canto superior esquerdo do bloco e seguindo em sentido horario
//a ordem usada para var_pont é que comeca com as juncoes, primeiro no canto superior direito, e entao em sentido horario. Nos blocos que podem se ligar aos blocos de trava, esses ocupam as posicoes apos as juncoes, sempre da esquerda para a direita
//os blocos de trava sempre ligam-se apenas a um outro e nao possuem juncoes, logo apenas um espaco em var_pont e col e suficiente para esses. No entanto, por terem propriedades mais variadas, precisam de algumas posicoes em var_int
	switch (tipo_novo_bloco) {
	case bloco_decisao:

		col = new bool[cols_decisao];
		for(int i = 0; i < cols_decisao; i++) col[i] = false;

		var_pont = new bloco*[ponts_decisao];
		for(int i = 0; i < ponts_decisao; i++) var_pont[i] = NULL;

		break;
	case bloco_acao:

		col = new bool[cols_acao];
		for(int i = 0; i < cols_acao; i++) col[i] = false;

		var_pont = new bloco*[ponts_acao];
		for(int i = 0; i < ponts_acao; i++) var_pont[i] = NULL;

		break;
	case bloco_inicio:

		col = new bool[cols_inicio];
		for(int i = 0; i < cols_inicio; i++) col[i] = false;

		var_pont = new bloco*[ponts_inicio];
		for(int i = 0; i < ponts_inicio; i++) var_pont[i] = NULL;

		blocos_inicio = true;

		break;
	case bloco_fim:

		col = new bool[cols_fim];
		for(int i = 0; i < cols_fim; i++) col[i] = false;

		var_pont = new bloco*[ponts_fim];
		for(int i = 0; i < ponts_fim; i++) var_pont[i] = NULL;

		break;
	case bloco_juncao:

		col = new bool[cols_juncao];
		for(int i = 0; i < cols_juncao; i++) col[i] = false;

		var_pont = new bloco*[ponts_juncao];
		for(int i = 0; i < ponts_juncao; i++) var_pont[i] = NULL;

		break;
    case bloco_repeticao :

        col = new bool[cols_repeticao];
        for(int i = 0; i < cols_repeticao; i++) col[i] = false;

		var_pont = new bloco*[ponts_repeticao];
		for(int i = 0; i < ponts_repeticao; i++) var_pont[i] = NULL;

        var_int = new int[ints_repeticao];
		rep_it_total = 2;
		rep_it_faltando = 2;
		break;
	case bloco_andar:

		col = new bool[cols_trava];
		for(int i = 0; i < cols_trava; i++) col[i] = false;

		var_pont = new bloco*[ponts_trava];
        for(int i = 0; i < ponts_trava; i++) var_pont[i] = NULL;

		var_int = new int[ints_andar];
		trava_posicao = -1;
		and_sentido = 1; // determina se o bloco deve mandar o robo ir para a frente (1) ou para tras (-1)
		break;
	case bloco_virar:

		col = new bool[cols_trava];
		for(int i = 0; i < cols_trava; i++) col[i] = false;

		var_pont = new bloco*[ponts_trava];
        for(int i = 0; i < ponts_trava; i++) var_pont[i] = NULL;

		var_int = new int[ints_virar];
		trava_posicao = -1;
		vir_dir = 1; //determina se o robo deve virar para a esquerda (1) ou direita (-1)
		break;
	case bloco_fita:

		col = new bool[cols_trava];
		for(int i = 0; i < cols_trava; i++) col[i] = false;

		var_pont = new bloco*[ponts_trava];
        for(int i = 0; i < ponts_trava; i++) var_pont[i] = NULL;

		var_int = new int[ints_fita];
		trava_posicao = -1;
		fita_sensor = 1; //determina qual sensor está sendo avaliado ( de 1 a 5, com 1 sendo o mais a esquerda e 5 o mais a direita
		fita_cor = 0; //determina se o robo deve retornar positivo se o robo estiver percebendo preto (0), branco (1), ou outras cores (ainda nao foi necessario, e portanto nao implementado
		break;
	case bloco_som:

		col = new bool[cols_trava];
		for(int i = 0; i < cols_trava; i++) col[i] = false;

		var_pont = new bloco*[ponts_trava];
        for(int i = 0; i < ponts_trava; i++) var_pont[i] = NULL;

		var_int = new int[ints_som];
		trava_posicao = -1;
		som_sensor = 1; //determina qual sensor de ultrassom o robo deve testar. varia entre 1 e 3, da esquerda para a direita
		break;
	case bloco_numero:

		col = new bool[cols_trava];
		for(int i = 0; i < cols_trava; i++) col[i] = false;

		var_pont = new bloco*[ponts_trava];
        for(int i = 0; i < ponts_trava; i++) var_pont[i] = NULL;

		var_int = new int[3];
		trava_posicao = -1;
		var_int[1] = 2; //determina qual numero (entre 0 e 9) o bloco representa
		var_int[2] = 2; //determina quantas iterações faltam
		break;
	case bloco_VF:

		col = new bool[cols_trava];
		for(int i = 0; i < cols_trava; i++) col[i] = false;

		var_pont = new bloco*[ponts_trava];
        for(int i = 0; i < ponts_trava; i++) var_pont[i] = NULL;

		var_int = new int[ints_VF];
		trava_posicao = -1;
		VF_estado = 1; //determina se o bloco deve seguir para o bloco abaixo quando a condicao avaliada retorna 1 (1) ou 0 (0)

	}
	//marca o bloco como ativo, diz que ele não deve ser destruido, cria ele no ponto marcado
	tipo = tipo_novo_bloco;
	ativo = esta_ativo;
	marcado_para_destruir = false;
	pos_x = x;
	pos_y = y;

	//faz com que o bloco seja criado com o mouse centrado nele (uma explicação mais detalhada é dada em bloco::mover())
	//se o bloco for um dos normais, faz com que o "offset" para o mouse seja igual a metade do comprimento no eixo x e metade da altura no eixo y (usando as medidas do bloco correspondente). do contrário, o offset é calculado usando as dimensoes dos blocos para andar, pois todos os blocos de trava tem as mesmas dimensoes
	if (tipo < primeiro_bloco_trava) {
		dmouse_x = al_get_bitmap_width(BLOCO[tipo - 1][0])/2;
		dmouse_y = al_get_bitmap_height(BLOCO[tipo - 1][0])/2;
	}
	else {
		dmouse_x = al_get_bitmap_width(seta)/2;
		dmouse_y = al_get_bitmap_height(seta)/2;
	}

}

bloco::bloco(int ponto, bloco* xbloco) {
	ativo = true;
	marcado_para_destruir = false;

	//coloca as coordenadas do bloco de acordo com o bloco e juncao na qual esta conectado
	pos_x = xbloco->pos_x + coord.linhas_x(ponto);
	pos_y = xbloco->pos_y + coord.linhas_y(ponto);

	col = new bool[cols_linha];
	col[0] = false;

	var_pont = new bloco*[ponts_linha];
	var_pont[0] = xbloco;
	var_pont[1] = NULL;

	var_int = new int[ints_linha];
	lin_pos2_x = mouse_x;
	lin_pos2_y = mouse_y_ajustado;
	lin_ponto1 = ponto;
	lin_ponto2 = 0;
	tipo = linha;

}

bloco::bloco() {}
int bloco::atualizar(lista_blocos* l) {
	int a_tipo = -1;
	if (l->bloco_ativo != NULL) a_tipo = l->bloco_ativo->tipo;
//tratamento dos blocos que representam linhas
	if (tipo == linha) {
	 // para explicacoes sobre as variaveis que controlam as linhas, ver a sua funcao criadora
	 		// se o bloco estiver ativo, coloca o segundo ponto no mouse. Do contrario, faz o mesmo que o switch anterior para o outro ponto
		if (ativo) {
			lin_pos2_x = mouse_x;
			lin_pos2_y = mouse_y_ajustado;
		}
		else if (var_pont[1]){
            lin_pos2_x = var_pont[1]->pos_x + coord.linhas_x(lin_ponto2);
            lin_pos2_y = var_pont[1]->pos_y + coord.linhas_y(lin_ponto2);
		}
		else {
            al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Houve um erro com uma das linhas do programa. Ela será destruída.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
            marcado_para_destruir = true;
            return 0;
		}
//se o botão direito for pressionado sobre a linha ou enquanto ela está selecionada, ela é destruida
		if (mouse_clicar[mouse_dir] && (col[0] || ativo)) {
			marcado_para_destruir = true;
			ativo = false;
			if (l->bloco_ativo != NULL) l->bloco_ativo = NULL;
		}
//se o botão esquerdo for pressionado sobre uma linha que já existe, torna-a ativa
		if (mouse_clicar[mouse_esq] && col[0] && bloco_topo == this) {
			ativo = true;
			l->ativo = true;

//quando a linha for ativada, será desconectada do bloco cujo ponto de junção estiver mais próximo do ponteiro do mouse
			if (pow(pos_x - mouse_x, 2) + pow(pos_y - mouse_y_ajustado, 2) >= pow(lin_pos2_x - mouse_x, 2) + pow(lin_pos2_y - mouse_y_ajustado, 2)) {
				//Limpa o bloco do qual a linha e desligada, para que ele nao ache que a linha continua la
				if (var_pont[1]->var_pont[0] == this) var_pont[1]->var_pont[0] = NULL;
				if (var_pont[1]->var_pont[1] == this) var_pont[1]->var_pont[1] = NULL;
				if (var_pont[1]->var_pont[2] == this) var_pont[1]->var_pont[2] = NULL;
				if (var_pont[1]->var_pont[3] == this) var_pont[1]->var_pont[2] = NULL;
				var_pont[1] = NULL; //limpa a variavel da linha, para que ela saiba que nao esta ligada a nada
				lin_ponto2 = 0;
			}
			else {
//se o mouse estiver mais próximo da segunda posicao (var_pont[1]), faz com que a primeira e a segunda troquem de lugar, entao limpa a segunda
				if (var_pont[0]->var_pont[0] == this) var_pont[0]->var_pont[0] = NULL;
				if (var_pont[0]->var_pont[1] == this) var_pont[0]->var_pont[1] = NULL;
				if (var_pont[0]->var_pont[2] == this) var_pont[0]->var_pont[2] = NULL;
				if (var_pont[0]->var_pont[3] == this) var_pont[0]->var_pont[2] = NULL;
				var_pont[0] = var_pont[1];
				lin_ponto1  = lin_ponto2;
				var_pont[1] = NULL;
				lin_ponto2 = 0;
			}

		}
		//atualiza as coordenadas do primeiro ponto de acordo com as coordenadas da juncao do bloco a qual esta ligado
        pos_x = var_pont[0]->pos_x + coord.linhas_x(lin_ponto1);
        pos_y = var_pont[0]->pos_y + coord.linhas_y(lin_ponto1);


	}
	//Tratamento para blocos normais
	else if (tipo < primeiro_bloco_trava) {

		if (ativo) {
			if (mouse_segurar[mouse_esq]) mover();
			else if (!mouse_segurar[mouse_esq] && !col[0]) {
				ativo = false;
				l->bloco_ativo = NULL;
			}
			else if (mouse_clicar[mouse_dir]) {
                marcado_para_destruir = true;
                ativo = false;
				l->bloco_ativo = NULL;
			}
		}
		//se o mouse estiver sobre o bloco
		else if (col[0] && bloco_topo == this) {
			if (mouse_clicar[mouse_esq]) {
				ativo = true;
				dmouse_x = mouse_x - pos_x;
				dmouse_y = mouse_y_ajustado - pos_y;
			}
			else if (mouse_clicar[mouse_meio]) ativo = true;
			else if (mouse_clicar[mouse_dir]) marcado_para_destruir = true;

		}
		//parte da funcao que verifica se uma linha deve ser criada ou presa em uma das juncoes do bloco ativo
		switch (tipo) {
		case bloco_decisao:
            if (mouse_clicar[mouse_esq] && bloco_topo == this) {
                if (col[1]) {
                    if (a_tipo == linha) prender(l, dec_E); // se o bloco ativo for uma linha, a liga ao ponto
                    else if (!l->ativo && var_pont[0] == NULL) var_pont[0] = l->criar_bloco(dec_E, this);
                    //se nao houver bloco ativo e o ponto sobre o qual o mouse esta nao tem nada ligado nele, cria uma nova linha
                }
                else if (col[2]) {
                    if (a_tipo == linha) prender(l, dec_S_nao);
                    else if (!l->ativo && var_pont[1] == NULL) var_pont[1] = l->criar_bloco(dec_S_nao, this);
                }
                else if (col[3]) {
                    if (a_tipo == linha) prender(l, dec_S_sim);
                    else if (!l->ativo && var_pont[2] == NULL) var_pont[2] = l->criar_bloco(dec_S_sim, this);
                }
            }
            break;

		case bloco_acao:
            if (mouse_clicar[mouse_esq] && bloco_topo == this){

                if (col[1]) {
                    if (a_tipo == linha) prender(l, acao_E);
                    else if (!l->ativo && var_pont[0] == NULL) var_pont[0] = l->criar_bloco(acao_E, this);
                }

                if (col[2]) {
                    if (a_tipo == linha) prender(l, acao_S);
                    else if (!l->ativo && var_pont[1] == NULL) var_pont[1] = l->criar_bloco(acao_S, this);
                }
            }
			break;

		case bloco_inicio:

			if (col[1] && mouse_clicar[mouse_esq] && bloco_topo == this) {
				if (a_tipo == linha) prender(l, ini_S);
				else if (!l->ativo && var_pont[0] == NULL) var_pont[0] = l->criar_bloco(ini_S, this);
			}

			break;
		case bloco_fim:

			if (col[1] && mouse_clicar[mouse_esq] && bloco_topo == this) {
				if (a_tipo == linha) prender(l, fim_E);
				else if (!l->ativo && var_pont[0] == NULL) var_pont[0] = l->criar_bloco(fim_E, this);
			}

			break;
		case bloco_juncao:
            if (mouse_clicar[mouse_esq] && bloco_topo == this) {
                if (col[1]) {
                    if (a_tipo == linha) prender(l, jun_E_esq);
                    else if (!l->ativo && var_pont[0] == NULL) var_pont[0] = l->criar_bloco(jun_E_esq, this);
                }

                if (col[2]) {
                    if (a_tipo == linha) prender(l, jun_E_dir);
                    else if (!l->ativo && var_pont[1] == NULL) var_pont[1] = l->criar_bloco(jun_E_dir, this);
                }

                if (col[3]) {
                    if (a_tipo == linha) prender(l, jun_S);
                    else if (!l->ativo && var_pont[2] == NULL) var_pont[2] = l->criar_bloco(jun_S, this);
                }
            }
			break;
		case bloco_repeticao:

            if (bloco_topo == this && ativo && caractere_pressionado[0] != '\0') {
                if (caractere_pressionado[1] == 'n' || caractere_pressionado[1] == 'N') rep_it_total = caractere_pressionado[0] - 48;
                else if (caractere_pressionado[0] == '8') rep_it_total++;
                else if (caractere_pressionado[0] == '2') rep_it_total--;
            }

            if (rep_it_total < 2) rep_it_total = 2;
            else if (rep_it_total > 9) rep_it_total = 9;
            rep_it_faltando = rep_it_total;
            if (mouse_clicar[mouse_esq] && bloco_topo == this) {
                if (col[1]) {
                    if (a_tipo == linha) prender(l, rep_E);
                    else if (!l->ativo && var_pont[0] == NULL) var_pont[0] = l->criar_bloco(rep_E, this);
                }

                if (col[2]) {
                    if (a_tipo == linha) prender(l, rep_loop_ini);
                    else if (!l->ativo && var_pont[1] == NULL) var_pont[1] = l->criar_bloco(rep_loop_ini, this);
                }

                if (col[3]) {
                    if (a_tipo == linha) prender(l, rep_loop_fim);
                    else if (!l->ativo && var_pont[2] == NULL) var_pont[2] = l->criar_bloco(rep_loop_fim, this);
                }

                if (col[4]) {
                    if (a_tipo == linha) prender(l, rep_S);
                    else if (!l->ativo && var_pont[3] == NULL) var_pont[3] = l->criar_bloco(rep_S, this);
                }

            }
			break;
		}
	}
	// tratamento dos blocos de trava
	else {
		if (ativo) {
            if (!col[0] && !mouse_segurar[mouse_esq]) {
                ativo = false;
                l->ativo = false;
                l->bloco_ativo = NULL;
            }
			if (mouse_segurar[mouse_esq]) mover();
			if (mouse_soltar[mouse_esq]) {
			//ao soltar um bloco de trava podem acontecer duas coisas: ele trava em um bloco ou simplesmente fica na tela normalmente
			//para que ele trave, e preciso verificar se ha algum bloco do tipo que ele aceita logo abaixo dele
				bloco* aux = l->inicio;
				bool col = false;
				bool travar = false;
				//se o bloco se prende ao bloco de acao
				if (tipo < primeiro_bloco_trava_logico) {
					do { //o programa percorre a lista procurando blocos de acao
						if (aux->tipo == bloco_acao) col = colisao(bloco_xy, seta, aux->pos_x, aux->pos_y, BLOCO[1][0]); //se os encontra, verifica se ha colisao entre ele e o bloco de trava
						if (!col) aux = aux->prox_bloco; //se nao ha, continua procurando
					} while (aux != NULL && !col);
					if (col) {
						if (aux->var_pont[2] == NULL) { //se ha colisao e nao existe bloco algum que ja esteja preso ao bloco de acao, prende o bloco ativo e então o desativa
							aux->var_pont[2] = this;
							var_pont[0] = aux;
							trava_posicao = 0;
							pos_x = aux->pos_x + qua_acao_x;
							pos_y = aux->pos_y + qua_acao_y;
						}
					}
				}
                // se o bloco se prende ao bloco de decisao
				else {
					do {
						if (aux->tipo == bloco_decisao) col = colisao(bloco_xy, seta, aux->pos_x, aux->pos_y, BLOCO[0][0]);
						if (!col) aux = aux->prox_bloco;
					} while (aux != NULL && !col);

					if (col) {
                        //como no bloco de acao existem 2 lugares onde blocos podem travar, ele precisa verificar em qual dos dois deve tentar se ligar
						//alem disso, como nem toda combinacao de blocos e valida, e preciso verificar tambem se os blocos sao compativeis, se o outro espaco ja estiver preenchido
						if (pos_x < aux->pos_x + al_get_bitmap_width(BLOCO[0][0])/2) {
							if (aux->var_pont[3] == NULL && tipo < bloco_VF) {
								if (aux->var_pont[4] == NULL) travar = true;
								else if (aux->var_pont[4]->tipo == bloco_numero && aux->var_pont[4]->tipo - tipo < 1) travar = false;
								else if (aux->var_pont[4]->tipo == bloco_VF && aux->var_pont[4]->tipo - tipo < 2) travar = false;

								else travar = true;

							}
							if (travar) {
								aux->var_pont[3] = this;
								var_pont[0] = aux;
								trava_posicao = 1;
								pos_x = aux->pos_x + qua_log1_x;
								pos_y = aux->pos_y + qua_log_y;
							}
						}
						else {
							if (aux->var_pont[4] == NULL) {
								if (tipo >= bloco_numero) travar = true;
								if (aux->var_pont[3] != NULL) {if (aux->var_pont[3]->tipo == bloco_numero) travar = false;}
							}
							if (travar) {
								aux->var_pont[4] = this;
								var_pont[0] = aux;
								trava_posicao = 2;
								pos_x = aux->pos_x + qua_log2_x;
								pos_y = aux->pos_y + qua_log_y;
							}
						}
					}
				}
				//se nao ocorreu travamento, o bloco simplesmente e solto
			}
		}
		else if (var_pont[0] != NULL) { //se o bloco ja estiver preso atualiza suas coordenadas
			if (trava_posicao == 0) { //se estiver preso ao bloco de acao
				pos_x = var_pont[0]->pos_x + qua_acao_x;
				pos_y = var_pont[0]->pos_y + qua_acao_y;
			}
			else { //se estiver preso ao bloco de decisao
				pos_y = var_pont[0]->pos_y + qua_log_y;
				pos_x = var_pont[0]->pos_x + ((trava_posicao == 1) ? qua_log1_x : qua_log2_x);
			}
		}
		bool clique_duplo = false;


        if (col[0] && bloco_topo == this) {

            if (mouse_clicar[mouse_esq]) {
                //alguns blocos podem ser atualizados usando um clique duplo
                //se o usuário clicar em um bloco de trava, é marcado que é possível haver um clique duplo
                //se houver um clique de novo dentro do espaço de tempo marcado em tempo_maximo_clique_duplo, o clique duplo é verificado
                if (possibilidade_clique_duplo) {
                    clique_duplo = true;
                    possibilidade_clique_duplo = false;
                }
                else {
                    possibilidade_clique_duplo = true;
                    tempo_max_clique_duplo = 0;
                }
                ativo = true;
                dmouse_x = mouse_x - pos_x;
				dmouse_y = mouse_y_ajustado - pos_y;
                if (var_pont[0] != NULL) {
                    var_pont[0]->var_pont[2 + trava_posicao] = NULL; //tira o ponteiro para o bloco de trava do bloco normal
					var_pont[0] = NULL; //tira o ponteiro para o bloco normal do bloco de trava
					trava_posicao = -1; //indica que ele não está preso
                }

            }
            if (mouse_clicar[mouse_dir]) marcado_para_destruir = 1;
            if (mouse_clicar[mouse_meio]) ativo = true;
        }


		if (ativo) switch (tipo) {
		case bloco_andar: //se o botao do meio do mouse for clicado ou a roda girada, troca se o bloco representa ir pra frente ou pra tras
			if ((mouse_clicar[mouse_esq] || mouse_clicar[mouse_meio]) && clique_duplo) and_sentido *= -1; //se houver um clique duplo, inverte o sentido
			if (caractere_pressionado[1] == 'N' || caractere_pressionado[1] == 'S') {
                //se o 8 do numpad ou a seta pra cima forem pressionados, vai para a frente
                if (caractere_pressionado[0] == '8') and_sentido = 1;
                //se o 2 do numpad ou a seta pra baixo forem pressionados, vai para atrás
                else if (caractere_pressionado[0] == '2') and_sentido = -1;
			}
			break;

		case bloco_virar:
			if ((mouse_clicar[mouse_esq] || mouse_clicar[mouse_meio]) && clique_duplo) vir_dir *= -1; //se houver um clique duplo, inverte o sentido da curva
			if (caractere_pressionado[1] == 'N' || caractere_pressionado[1] == 'S') {
                //se o 6 do numpad ou a seta pra direita forem pressionados, vai para a direita
                if (caractere_pressionado[0] == '6') vir_dir = -1;
                //se o 4 do numpad ou a seta pra esquerda forem pressionados, vai para a esquerda
                else if (caractere_pressionado[0] == '4') vir_dir = 1;
			}
			break;

		case bloco_fita:
            //se for apertada um numero do teclado normal, 1 a 5 determinam qual sensor é lido e 0 e 9 a cor lida
            if (caractere_pressionado[1] == 'n') {
                if (caractere_pressionado[0] <= '5' && caractere_pressionado[0] >= '1') fita_sensor = caractere_pressionado[0] - 48;
                else if (caractere_pressionado[0] == '0') fita_cor = 0;
                else if (caractere_pressionado[0] == '9') fita_cor = 1;
            }
            //se for no teclado numérico, imita o padrão da distribuição dos sensores, com 7 e 9 determinadno a cor
            else if (caractere_pressionado[1] == 'N') {
                if (caractere_pressionado[0] == '1')      fita_sensor = 1;
                else if (caractere_pressionado[0] == '4') fita_sensor = 2;
                else if (caractere_pressionado[0] == '5') fita_sensor = 3;
                else if (caractere_pressionado[0] == '6') fita_sensor = 4;
                else if (caractere_pressionado[0] == '3') fita_sensor = 5;
                else if (caractere_pressionado[0] == '7') fita_cor = 0;
                else if (caractere_pressionado[0] == '9') fita_cor = 1;
            }//se for uma das setas, cima e baixo trocam a cor, esquerda troca o sensor escolhido pelo da esquerda do atual, e o da direita pelo a direita do atual
            else if (caractere_pressionado[1] == 'S') {
                if (caractere_pressionado[0] == '8' || caractere_pressionado[0] == '2') fita_cor = (fita_cor == 0 ? 1 : 0);
                else if (caractere_pressionado[0] == '4') fita_sensor = (fita_sensor == 1 ? 5 : (fita_sensor - 1));
                else if (caractere_pressionado[0] == '6') fita_sensor = ((fita_sensor == 5) ? (1) : (fita_sensor + 1));

            }
			break;

		case bloco_som:
			break;

		case bloco_numero: //se o botao do meio for apertado ou o mouse rolado pra cima, incrementa o numero. Se for rolado pra baixo, o diminui
			if (mouse_clicar[mouse_meio] || mouse_rolar == mouse_cima) var_int[1] = ((var_int[1] == 9) ? 0 : var_int[1] + 1);
			if (mouse_rolar == mouse_baixo) var_int[1] = ((var_int[1] == 0) ? 9 : var_int[1] - 1);
			var_int[2] = var_int[1]; // faz com que o numero de iterações a ser feitas sempre seja igual ao número de iterações a fazer quando o fluxograma não estiver rodando
			break;

		case bloco_VF:
			if (clique_duplo /*|| ((caractere_pressionado[0] = '8' || caractere_pressionado[0] == '2') && caractere_pressionado[1] != 'n')*/) VF_estado *= -1;
			break;
		}
	}
	return ativo;
}

//função que desenha os blocos
void bloco::desenhar(lista_blocos *l) {
	int a_tipo = -1; //armazena o tipo do bloco ativo (se não houver, continua em -1)
	if (l->ativo) a_tipo = l->bloco_ativo->tipo;
	switch (tipo) {
	case linha:
		if (ativo && modo == -1) al_draw_line(bloco_xy, lin_pos2_x, lin_pos2_y, branco, 1); //se a linha for ativa, desenha-a em branco
		else if (col[0]) al_draw_line(bloco_xy, lin_pos2_x, lin_pos2_y, al_map_rgb(200, 200, 200), 1); //se o mouse estiver sobre a linha, desenha-a em um cinza escuro
		else  al_draw_line(bloco_xy, lin_pos2_x, lin_pos2_y, preto, 1); //em caso contrário, desenha-a em preto
		break;

	case bloco_decisao:
		if (modo == -1) { //se o programa estiver em modo de desenho

            //desenha primeiro o bloco em si, dependendo se ele está ativo, com o mouse sobre ele (ou, no caso do bloco de decisão, com
            //um bloco de trava compatível sobre ele) ou sem nada
			if (ativo) al_draw_bitmap(BLOCO[0][2], bloco_xy, 0);
			else if (col[0] && (!l->ativo || a_tipo >= bloco_fita)) al_draw_bitmap(BLOCO[0][1], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[0][0], bloco_xy, 0);

			//então desenha as junções de cada bloco, com uma linha branca em volta caso o mouse esteja sobre ela e não haja bloco ativo ou ele seja uma linha
			//(pois em um caso clicar na junção criará uma linha e no outro conectará a linha a essa junção)
			if (col[1] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(dec_E), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(dec_E), 0);

			if (col[2] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(dec_S_nao), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(dec_S_nao), 0);

			if (col[3] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(dec_S_sim), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(dec_S_sim), 0);
		}
		else {//se o programa estiver rodando, desenha o bloco com todas as junções sempre como inativas
			if (ativo) al_draw_bitmap(BLOCO[0][3], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[0][0], bloco_xy, 0);
			al_draw_bitmap(PONTO[0], pos_ponto(dec_E), 0);
			al_draw_bitmap(PONTO[0], pos_ponto(dec_S_nao), 0);
			al_draw_bitmap(PONTO[0], pos_ponto(dec_S_sim), 0);
            if (var_pont[3] != NULL && var_pont[4] == NULL) if (var_pont[3]->tipo == bloco_numero) { //se o bloco estiver no modo de loop (apenas um bloco de número travado na primeira posição e nada na segunda) desenha um segundo bloco de numero na segunda posição, que mostrará quantas vezes o loop foi executado
                if (var_pont[3]->var_int[1] == var_pont[3]->var_int[2]) al_draw_bitmap(NUMERO[var_pont[3]->var_int[2]], pos_x + qua_log2_x, pos_y + qua_log_y, 0); //como a variavel que indica quantas iterações ainda precisam ser feitas é decrementada assim que o comando é mandado para o programa, parece que uma iteração é feita imediatamente. Para evitar isso, apenas quando as duas variáceis são iguais elas são mostradas em seus valores reais, e depois o numero de iterações a fazer é mostrado incrementado por 1
                else al_draw_bitmap(NUMERO[var_pont[3]->var_int[2] + 1], pos_x + qua_log2_x, pos_y + qua_log_y, 0);
		}
		}
		break;

	case bloco_acao:
		if (modo == -1) {

			if (ativo) al_draw_bitmap(BLOCO[1][2], bloco_xy, 0);
			else if (col[0] && (!l->ativo || (a_tipo >= bloco_andar && a_tipo <= bloco_virar))) al_draw_bitmap(BLOCO[1][1], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[1][0], bloco_xy, 0);

			if (col[1] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(acao_E), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(acao_E), 0);

			if (col[2] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(acao_S), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(acao_S), 0);
		}
		else {
			if (ativo) al_draw_bitmap(BLOCO[1][3], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[1][0], bloco_xy, 0);
			al_draw_bitmap(PONTO[0], pos_ponto(acao_E), 0);
			al_draw_bitmap(PONTO[0], pos_ponto(acao_S), 0);
		}
		break;

	case bloco_inicio:
		if (modo == -1) {

			if (ativo) al_draw_bitmap(BLOCO[2][2], bloco_xy, 0);
			else if (bloco_topo == this && col[0]) al_draw_bitmap(BLOCO[2][1], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[2][0], bloco_xy, 0);

			if (col[1] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(ini_S), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(ini_S), 0);

		}
		else {
			if (ativo) al_draw_bitmap(BLOCO[2][3], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[2][0], bloco_xy, 0);
			al_draw_bitmap(PONTO[0], pos_ponto(ini_S), 0);
		}
		break;

	case bloco_fim:
		if (modo == -1) {

			if (ativo) al_draw_bitmap(BLOCO[3][2], bloco_xy, 0);
			else if (bloco_topo == this && col[0]) al_draw_bitmap(BLOCO[3][1], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[3][0], bloco_xy, 0);

			if (col[1] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(fim_E), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(fim_E), 0);
		}
		else {
			if (ativo) al_draw_bitmap(BLOCO[3][3], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[3][0], bloco_xy, 0);
			al_draw_bitmap(PONTO[0], pos_ponto(fim_E), 0);
		}

		break;

	case bloco_juncao:
		if (modo == -1) {

			if (ativo) al_draw_bitmap(BLOCO[4][2], bloco_xy, 0);
			else if (bloco_topo == this && col[0]) al_draw_bitmap(BLOCO[4][1], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[4][0], bloco_xy, 0);

			if (col[1] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(jun_E_esq), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(jun_E_esq), 0);

			if (col[2] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(jun_E_dir), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(jun_E_dir), 0);

			if (col[3] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(jun_S), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(jun_S), 0);
		}
		else {
			if (ativo) al_draw_bitmap(BLOCO[4][3], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[4][0], bloco_xy, 0);
			al_draw_bitmap(PONTO[0], pos_ponto(jun_E_esq), 0);
			al_draw_bitmap(PONTO[0], pos_ponto(jun_E_dir), 0);
			al_draw_bitmap(PONTO[0], pos_ponto(fim_E), 0);
		}
		break;
    case bloco_repeticao:
		if (modo == -1) {
			if (ativo) al_draw_bitmap(BLOCO[5][2], bloco_xy, 0);
			else if (bloco_topo == this && col[0]) al_draw_bitmap(BLOCO[5][1], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[5][0], bloco_xy, 0);

			if (col[1] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(rep_E), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(rep_E), 0);

			if (col[2] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(rep_loop_ini), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(rep_loop_ini), 0);

			if (col[3] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(rep_loop_fim), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(rep_loop_fim), 0);

            if (col[4] && (!l->ativo || a_tipo == linha)) al_draw_bitmap(PONTO[1], pos_ponto(rep_S), 0);
			else al_draw_bitmap(PONTO[0], pos_ponto(rep_S), 0);
		}
		else {
			if (ativo) al_draw_bitmap(BLOCO[5][3], bloco_xy, 0);
			else al_draw_bitmap(BLOCO[5][0], bloco_xy, 0);
			al_draw_bitmap(PONTO[0], pos_ponto(rep_E), 0);
			al_draw_bitmap(PONTO[0], pos_ponto(rep_loop_ini), 0);
			al_draw_bitmap(PONTO[0], pos_ponto(rep_loop_fim), 0);
			al_draw_bitmap(PONTO[0], pos_ponto(rep_S), 0);
            if (rep_it_faltando == rep_it_total) al_draw_bitmap(NUMERO[rep_it_total], pos_x + qua_rep_x, pos_y + qua_rep2_y, 0); //como a variavel que indica quantas iterações ainda precisam ser feitas é decrementada assim que o comando é mandado para o programa, parece que uma iteração é feita imediatamente. Para evitar isso, apenas quando as duas variáceis são iguais elas são mostradas em seus valores reais, e depois o numero de iterações a fazer é mostrado incrementado por 1
            else al_draw_bitmap(NUMERO[rep_it_faltando + 1], pos_x + qua_rep_x, pos_y + qua_rep2_y, 0);
		}
		al_draw_bitmap(NUMERO[rep_it_total], pos_x + qua_rep_x, pos_y + qua_rep1_y, 0); //desenha o número de iterações totais
		break;
	case bloco_andar:
        //desenha o bloco invertido ou não, de acordo com a direção que ele representa
		if (and_sentido == 1) al_draw_bitmap(seta, bloco_xy, 0);
		else al_draw_bitmap(seta, bloco_xy, ALLEGRO_FLIP_VERTICAL);
		if (modo == desenhando) { //desenha uma moldura branca no bloco se ele estiver atio e preta se o mouse estiver sobre ele
			if (ativo) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, branco, 3);
			else if (bloco_topo == this) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, preto, 3);
		}
		else { //se o fluxograma estiver rodando e esse bloco se tornar ativo, desenha uma moldura dourada nele (o que atualmente nunca acontece para nenhum dos blocos de trava)
			if (ativo) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, al_map_rgb(165, 179, 4), 3);
		}
		break;

	case bloco_virar:
		if (vir_dir == 1) al_draw_bitmap(voltita, bloco_xy, 0);
		else al_draw_bitmap(voltita, bloco_xy, ALLEGRO_FLIP_HORIZONTAL);
		if (modo == -1) {
			if (ativo) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(voltita) - 1, pos_y + al_get_bitmap_height(voltita) - 1, branco, 1);
			else if (bloco_topo == this) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(voltita) - 1, pos_y + al_get_bitmap_height(voltita) - 1, preto, 3);
		}
		else {
			if (ativo) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(voltita) - 1, pos_y + al_get_bitmap_height(voltita) - 1, al_map_rgb(165, 179, 4), 3);
		}
		break;

	case bloco_fita: {
		al_draw_bitmap(fita, bloco_xy, 0); //desenha o bloco
		//como o sensor que está sendo testado é representado visualmente por um retângulo
		//da cor a ser testada sobre o sensor correspondente no desenho, é preciso determinar as coordenadas desse ponto
		int snsr_x = 0, snsr_y = 0;
		switch (fita_sensor) {
		case 1:
			snsr_x = pos_x + snsr1_x;
			snsr_y = pos_y + snsr15_y;
			break;
		case 2:
			snsr_x = pos_x + snsr2_x;
			snsr_y = pos_y + snsr24_y;
			break;
		case 3:
			snsr_x = pos_x + snsr3_x;
			snsr_y = pos_y + snsr3_y;
			break;
		case 4:
			snsr_x = pos_x + snsr4_x;
			snsr_y = pos_y + snsr24_y;
			break;
		case 5:
			snsr_x = pos_x + snsr5_x;
			snsr_y = pos_y + snsr15_y;
			break;
		}
        //desenha o retângulo de acordo com a cor (em um switch para facilitar caso no futuro hajam mais cores)
		switch (fita_cor) {
		case 0 :
            al_draw_filled_rectangle(snsr_x, snsr_y, snsr_x + 3, snsr_y + 3, preto);
            break;
		case 1 :
            al_draw_filled_rectangle(snsr_x, snsr_y, snsr_x + 3, snsr_y + 3, branco);
            break;
		}
		if (modo == -1) {
			if (ativo) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, branco, 3); //se o bloco estiver ativo, desenha um retângulo branco em volta dele
			else if (bloco_topo == this) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, preto, 3); //se o mouse estiver sobre ele, desenha um retângulo preto
		}
		break; }

	case bloco_som: {
		al_draw_bitmap(ultrassom, bloco_xy, 0);
		if (modo == -1) {
			if (ativo) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, branco, 3);
			else if (bloco_topo == this) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, preto, 3);
		}
		break;

	case bloco_numero:
		if (modo == -1) { //se o fluxograma estiver sendo editado, mostra o número de iterações que seram feitas
            al_draw_bitmap(NUMERO[var_int[1]], bloco_xy, 0);
			if (ativo) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, branco, 3);
			else if (bloco_topo == this) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, preto, 3);
		}
		else if (var_int[0] == 2) {//se estiver rodando e travado na posição da direita de um bloco de deciso, mostra o número de iterações a fazer
            if (var_int[1] == var_int[2]) al_draw_bitmap(NUMERO[var_int[2]], bloco_xy, 0); //como a variavel que indica quantas iterações ainda precisam ser feitas é decrementada assim que o comando é mandado para o programa, parece que uma iteração é feita imediatamente. Para evitar isso, apenas quando as duas variáceis são iguais elas são mostradas em seus valores reais, e depois o numero de iterações a fazer é mostrado incrementado por 1
            else al_draw_bitmap(NUMERO[var_int[2] + 1], bloco_xy, 0);
		}
		else al_draw_bitmap(NUMERO[var_int[1]], bloco_xy, 0); //se estiver rodando e travado na posição da esquerda mostra o numero total de iterações

		break;

	case bloco_VF:
		if (VF_estado == 1) al_draw_bitmap(logico_v, bloco_xy, 0);
		else al_draw_bitmap(logico_f, bloco_xy, 0);
		if (modo == -1) {
			if (ativo) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, branco, 3);
			else if (bloco_topo == this) al_draw_rectangle(bloco_xy, pos_x + al_get_bitmap_width(seta) - 1, pos_y + al_get_bitmap_height(seta) - 1, preto, 3);
		}
		break; }
	}
}

void bloco::destruir(lista_blocos* l) {
    if (l->bloco_ativo == this) l->bloco_ativo = NULL;
    switch (tipo) {
        case linha :
            //se algum bloco tiver um ponteiro para essa linha, o tira, para evitar que a linha seja acessada ou deletada de novo
            if (var_pont[0] != NULL) {
                if (var_pont[0]->var_pont[0] == this) var_pont[0]->var_pont[0] = NULL;
                if (var_pont[0]->var_pont[1] == this) var_pont[0]->var_pont[1] = NULL;
                if (var_pont[0]->var_pont[2] == this) var_pont[0]->var_pont[2] = NULL;
                if (var_pont[0]->var_pont[3] == this) var_pont[0]->var_pont[3] = NULL;
            }

            if (var_pont[1]!= NULL) {
                if (var_pont[1]->var_pont[0] == this) var_pont[1]->var_pont[0] = NULL;
                if (var_pont[1]->var_pont[1] == this) var_pont[1]->var_pont[1] = NULL;
                if (var_pont[1]->var_pont[2] == this) var_pont[1]->var_pont[2] = NULL;
                if (var_pont[1]->var_pont[3] == this) var_pont[1]->var_pont[3] = NULL;
            }
            delete var_int;
            break;
        case bloco_decisao :
            //se existe alguma linha ligada ao bloco sendo deletado, marca ela para ser deletada tambem,
            //além de tirar dela o ponteiro para o bloco deletado, para impedir que a função tente tirar o
            //ponteiro para a linha do bloco que já foi deletado
            for (int i = 0; i < ponts_decisao - 2; i++) {
                if (var_pont[i] != NULL) {
                    var_pont[i]->marcado_para_destruir = true;
                    if (var_pont[i]->var_pont[0] == this) var_pont[i]->var_pont[0] = NULL;
                    if (var_pont[i]->var_pont[1] == this) var_pont[i]->var_pont[1] = NULL;
                }
            }
        	//se há um bloco de trava conectado, garante que ele não tera mais um ponteiro para o
        	//bloco destruido e que ele continuara sendo imprimido na ultima posicao que ocupou,
        	//além de marcar que ele não está mais conectado
			if (var_pont[3] != NULL) {
				var_pont[3]->var_pont[0] = NULL;
				var_pont[3]->pos_x = pos_x + qua_log1_x;
				var_pont[3]->pos_y = pos_y + qua_log_y;
                var_pont[3]->trava_posicao = -1;
			}
			if (var_pont[4] != NULL) {
				var_pont[4]->var_pont[0] = NULL;
				var_pont[4]->pos_x = pos_x + qua_log2_x;
				var_pont[4]->pos_y = pos_y + qua_log_y;
                var_pont[4]->trava_posicao = -1;
			}

            break;
        case bloco_acao :
             for (int i = 0; i < ponts_acao - 1; i++) {
                if (var_pont[i] != NULL) {
                    var_pont[i]->marcado_para_destruir = true;
                    if (var_pont[i]->var_pont[0] == this) var_pont[i]->var_pont[0] = NULL;
                    if (var_pont[i]->var_pont[1] == this) var_pont[i]->var_pont[1] = NULL;
                }
            }
            if (tipo == bloco_acao && var_pont[2] != NULL) {
                var_pont[2]->var_pont[0] = NULL;
                var_pont[2]->pos_x = pos_x + qua_acao_x;
                var_pont[2]->pos_y = pos_y + qua_acao_y;
            }
            break;
        case bloco_inicio :
        //como o bloco de inicio e o de fim só tem um ponteiro, são tratados identicamente,
        //exceto pelo fato que, para o bloco de inicio, é necessário mostrar que não há mais nenhum bloco de inicio
            blocos_inicio = false;
        //não há break para que o próximo caso seja executado também, já que é a mesma coisa
        case bloco_fim :
            if (var_pont[0] != NULL) {
                var_pont[0]->marcado_para_destruir = true;
                if (var_pont[0]->var_pont[0] == this) var_pont[0]->var_pont[0] = NULL;
                if (var_pont[0]->var_pont[1] == this) var_pont[0]->var_pont[1] = NULL;
            }
            break;
        case bloco_juncao :
            for (int i = 0; i < ponts_juncao; i++) {
                if (var_pont[i] != NULL) {
                    var_pont[i]->marcado_para_destruir = true;
                    if (var_pont[i]->var_pont[0] == this) var_pont[i]->var_pont[0] = NULL;
                    if (var_pont[i]->var_pont[1] == this) var_pont[i]->var_pont[1] = NULL;
                }
            }
            break;
        case bloco_repeticao :
            for (int i = 0; i < ponts_repeticao; i++) {
                if (var_pont[i] != NULL) {
                    var_pont[i]->marcado_para_destruir = true;
                    if (var_pont[i]->var_pont[0] == this) var_pont[i]->var_pont[0] = NULL;
                    if (var_pont[i]->var_pont[1] == this) var_pont[i]->var_pont[1] = NULL;
                }
            }
            delete var_int;
            break;
        case bloco_andar :
        case bloco_virar :
        case bloco_fita :
        case bloco_som :
        case bloco_numero :
        case bloco_VF :
            //se o bloco for de trava e estiver ligado a outro, garante que esse não terá um ponteiro para o bloco destruido
            if (var_pont[0] != NULL) for (int i = 2; i <= 4; i++) if (var_pont[0]->var_pont[i] == this) var_pont[0]->var_pont[i] = NULL;
            delete var_int;
            break;
    }
    delete col; //desaloca a memoria alocada para col
	delete var_pont; //desaloca a memoria alocada para var_pont
	delete this; //desaloca a memoria usada para o bloco
    l->checar_colisoes(mouse_xy_ajustado);
    l->salvar(arquivo_backup);
}


void bloco::mover() {
//dmouse_x e dmouse_y são a distância do ponto superior esquerdo do bloco ao mouse, calculada no momento em que se pressiona o botão do mouse para mover o bloco
//isso e feito para que, sabendo a distância do mouse ao canto, seja possivel mover o bloco de maneira que a distancia seja constante
//impedindo o efeito que faria o bloco "pular" para o canto superior esquerdo quando fosse começar a ser movido
	pos_x = mouse_x - dmouse_x;
	pos_y = mouse_y_ajustado - dmouse_y;
    //impede que mais da metade de um bloco esteja fora dos limites máximos da tela (contando o máximo com a barra de rolagem vertical e com a tela de tamanho ajustável)
    if (tipo < primeiro_bloco_trava && tipo != linha) {
        if (pos_x < -al_get_bitmap_width(BLOCO[tipo - 1][0])/2) pos_x = -al_get_bitmap_width(BLOCO[tipo - 1][0])/2;
        else if (pos_x > dimensoes_tela_x - al_get_bitmap_width(BLOCO[tipo - 1][0])/2) pos_x = dimensoes_tela_x - al_get_bitmap_width(BLOCO[tipo - 1][0])/2;
        if (pos_y < posicao_y - al_get_bitmap_height(BLOCO[tipo - 1][0])/2) pos_y = posicao_y - al_get_bitmap_height(BLOCO[tipo - 1][0])/2;
        else if (pos_y > dimensoes_tela_y + posicao_y - al_get_bitmap_height(BLOCO[tipo - 1][0])/2) pos_y = dimensoes_tela_y + posicao_y - al_get_bitmap_height(BLOCO[tipo - 1][0])/2;
    }
    else {
        if (pos_x < -al_get_bitmap_width(seta)/2) pos_x = -al_get_bitmap_width(seta)/2;
        else if (pos_x > dimensoes_tela_x - al_get_bitmap_width(seta)/2) pos_x = dimensoes_tela_x - al_get_bitmap_width(seta)/2;
        if (pos_y < posicao_y - al_get_bitmap_height(seta)/2) pos_y = posicao_y - al_get_bitmap_height(seta)/2;
        else if (pos_y > dimensoes_tela_y + posicao_y - al_get_bitmap_height(seta)/2) pos_y = dimensoes_tela_y + posicao_y - al_get_bitmap_height(seta)/2;
    }
}

void bloco::prender(lista_blocos* l, int ponto) {

//ponto/100 = tipo de bloco; ((ponto%100)/10) = entrada ou saída; ponto%10 -1 = numero

//como as entradas dos blocos só podem se conectar as saídas e vice-versa, é preciso verificar a onde a linha já está ligada.
//Além disso, uma linha não pode ligar um bloco a ele mesmo
	if (l->bloco_ativo->var_pont[0] != this && (l->bloco_ativo->lin_ponto1%100)/10 != (ponto%100)/10) {
		l->bloco_ativo->var_pont[1] = this; //faz com que a linha tenha um ponteiro para o bloco na qual está conectado
		l->bloco_ativo->lin_ponto2 = ponto; //faz com que a linha saiba em qual ponto está sendo ligado
		var_pont[ponto%10 - 1] =  l->bloco_ativo; //faz com que o bloco tenha um ponteiro para a linha na qual está conectado
		l->bloco_ativo->ativo = 0; //desativa a linha
		l->bloco_ativo = NULL;
	}
	l->salvar(arquivo_backup);
}

bloco* bloco::rodar() {
	bloco* prox_bloco_a_rodar = this; //variavel que armazena o proximo bloco ativo. Se nenhum for determinado e o fim do programa nao foi encontrado, o bloco ativo continua sendo o mesmo
	bool condicao = false, checou_condicao = false; //para os blocos de decisao, essas variaveis armazenam respectivamente se a condicao logica e verdadeira e se ela ja foi avaliada ou nao
	switch (tipo) {
	case bloco_decisao:

		if (var_pont[3] == NULL) {//se nao ha nenhum bloco ligado ao primeiro espaco, nao ha condicao logica a avaliar
			prox_bloco_a_rodar = NULL;
			al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não há condição lógica a ser avaliada.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		}
		else if (var_pont[3]->tipo == bloco_numero) { //se a primeira posicao estiver preenchida por um bloco de numero, o bloco repetira o que estiver dentro dele um certo numero de vezes
            if (var_pont[3]->var_int[2] == -1) var_pont[3]->var_int[2] = var_pont[3]->var_int[1];//se var_int[2] já é -1, quer dizer que o bloco de repetição foi atingido novamente, e portanto a contagem precisa recomeçar
            if (var_pont[3]->var_int[2] < 1) {//se o loop já se completou, continua para o bloco ligado na junção de baixo
                condicao = true;
                var_pont[3]->var_int[2] = -1; //para que o numero de iterações a fazer seja mostrado corretamente (vide bloco :: desenhar)
            }
            else { //do contrário, mostra que mais uma iteração foi completa e retorna para o bloco da esquerda
                condicao = false;
                var_pont[3]->var_int[2]--;
            }
            checou_condicao = true;
		}//como esse código não foi testado e a maioria tem boa chance de mudar no futuro, ele está aqui mais como ideia geral do que fazer
		else if (var_pont[4] != NULL && !bloco_enviado) {

            switch (var_pont[3]->tipo) {
                case bloco_fita :
                    *command_var = var_pont[3]->fita_sensor*10 + var_pont[3]->fita_cor;

                    break;
                case bloco_som :
                    *command_var = 1 + var_pont[3]->som_sensor*2;

                    break;
                default :
                    al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "bloco inválido conectado ao bloco de decisão", NULL, ALLEGRO_MESSAGEBOX_ERROR);
                    break;
            }
            bloco_enviado = true;
		}
		else {
			prox_bloco_a_rodar = NULL;
			al_show_native_message_box(al_get_current_display(), "Fluxprog", "Erro!", "Não há condição lógica a ser avaliada.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		}

		if ((*feedback) != -1 && var_pont[3] != NULL && var_pont[4] != NULL && bloco_enviado) {//verifica se há um novo comando, blocos de trava em ambas posições e o bloco já foi enviado
            if (var_pont[4]->tipo == bloco_VF) {//verifica se o retorno do sensor é igual ao estado do bloco VF
                if (var_pont[4]->VF_estado == (*feedback)) condicao = true;
                else condicao = false;
                (*feedback) = -1; //marca que o feedback foi usado
            }
            else if (var_pont[4]->tipo == bloco_numero) {//se ainda faltam iterações a fazer, as faz, do contrário continua em frente no fluxograma
            if ((*feedback) == 1) {
                if (var_pont[4]->var_int[2] != 0) {
                    var_pont[4]->var_int[2]--;
                    condicao = false;
                }
                else condicao = true;
                checou_condicao = true;
                }
            }
		}
		if (checou_condicao) {
			if (condicao) { //se a condição for verdadeira, o programa continua para o bloco conectado a junção de baixo
            if (var_pont[2] == NULL) prox_bloco_a_rodar = NULL;
            else if (var_pont[2]->var_pont[0] == NULL || var_pont[2]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
            else if (var_pont[2]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[2]->var_pont[1];
            else prox_bloco_a_rodar = var_pont[2]->var_pont[0];
			}
			else {//se for falsa, continua no do lado
				if (var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
				else if (var_pont[1]->var_pont[0] == NULL || var_pont[1]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
				else if (var_pont[1]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[1]->var_pont[1];
				else prox_bloco_a_rodar = var_pont[1]->var_pont[0];
			}
            bloco_enviado = false;

		}
		break;

	case bloco_acao:
		if (var_pont[2] != NULL && !bloco_enviado) { //envia o comando de acordo com o bloco de trava preso e a sua configuração
            if (var_pont[2]->tipo == bloco_andar && var_pont[2]->and_sentido == -1)	*command_var = 2;
            else  if (var_pont[2]->tipo == bloco_andar && var_pont[2]->and_sentido == 1) *command_var = 8;
            else  if (var_pont[2]->tipo == bloco_virar && var_pont[2]->vir_dir == -1) *command_var = 6;
            else  if (var_pont[2]->tipo == bloco_virar && var_pont[2]->vir_dir == 1) *command_var = 4;
            bloco_enviado = true; //garante que o bloco não será enviado de novo
            if (modelo) for (int i = 0; i < 3; i++) { //guarda a posição inicial do bloco para saber se ele já andou o suficiente
                posicao_angular_inicial[i] = (posicao_angular.first)[i];
                posicao_plana_inical[i] = (posicao_plana.first)[i];
            }
		}
		else if (var_pont[2] == NULL) { //se não houver bloco de trava, o bloco de ação não faz nada e simplesmente passa para o próximo
            if (var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
            else if (var_pont[1]->var_pont[0] == NULL || var_pont[1]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
            else if (var_pont[1]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[1]->var_pont[1];
            else prox_bloco_a_rodar = var_pont[1]->var_pont[0];
            bloco_enviado = false;
        }
		else if (!modelo) {
            if (*(feedback) == 1) {//se não há um bloco de trava travado ou  o outro programa indicou que a ação acabou, decide qual será o próximo bloco
                if (*(feedback) == 1 )*(feedback) = -1; //marca que o novo feedback já foi usado, se houver
                //tratamento idêntico ao do bloco de inicio.
                if (var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
                else if (var_pont[1]->var_pont[0] == NULL || var_pont[1]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
                else if (var_pont[1]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[1]->var_pont[1];
                else prox_bloco_a_rodar = var_pont[1]->var_pont[0];
                bloco_enviado = false;
            }
		}
		else { //se o comando tiver sido enviado, verifica se ele já foi completado
            bool terminar_comando = false;
            if (var_pont[2]->tipo == bloco_andar && var_pont[2]->and_sentido == -1)	{ //como andar pra trás não foi implementado, ignora esse comando
                terminar_comando = true;
            }
            else  if (var_pont[2]->tipo == bloco_andar && var_pont[2]->and_sentido == 1) { //calcula se o robô andou o suficiente para ir um quadrado
                if(sqrt(pow(DIS(posicao_plana_inical[0], posicao_plana.first[0]), 2) + pow(DIS(posicao_plana_inical[1], posicao_plana.first[1]), 2)) >= 0.485) terminar_comando = true;
            }
            else  if (var_pont[2]->tipo == bloco_virar) {
                if (DIS(posicao_angular.first[2], posicao_angular_inicial[2]) >= M_PI_2) terminar_comando = true;//calcula se o robô já andou o suficente para virar 90°
            }
            if (terminar_comando) {//se sim, vai para o próximo bloco, se houver
                if (var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
                else if (var_pont[1]->var_pont[0] == NULL || var_pont[1]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
                else if (var_pont[1]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[1]->var_pont[1];
                else prox_bloco_a_rodar = var_pont[1]->var_pont[0];
                bloco_enviado = false;
            }
		}
		break;

	case bloco_inicio:
		if (var_pont[0] == NULL) prox_bloco_a_rodar = NULL; //se nao há nenhuma linha ligada a junção, para de rodar
		else if (var_pont[0]->var_pont[0] == NULL || var_pont[0]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL; //se a linha ligada não está ligada a outro bloco, para de rodar
		else if (var_pont[0]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[0]->var_pont[1]; //se o lado da linha ligado ao bloco for um, o proximo bloco é o que está ligada no proximo bloco é o outro
		else prox_bloco_a_rodar = var_pont[0]->var_pont[0];
		break;

	case bloco_fim:
		*command_var = -1; //diz ao outro programa que esse parou de rodar
		prox_bloco_a_rodar = NULL;
		break;

	case bloco_juncao://tratamento idêntico ao do bloco de inicio.
		if (var_pont[2] == NULL) prox_bloco_a_rodar = NULL;
		else if (var_pont[2]->var_pont[0] == NULL || var_pont[2]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL;
		else if (var_pont[2]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[2]->var_pont[1];
		else prox_bloco_a_rodar = var_pont[2]->var_pont[0];
		break;
	case bloco_repeticao :
        if (rep_it_faltando == -1) rep_it_faltando = rep_it_total;//se o numero de iteraçoes que faltam é -1, quer dizer que o bloco de repetição foi atingido novamente, e portanto a contagem precisa recomeçar
        if (rep_it_faltando == 0) {//se o loop já se completou, continua para o bloco ligado na junção de baixo
            rep_it_faltando  = -1; //para que o numero de iterações a fazer seja mostrado corretamente (vide bloco :: desenhar)
            if (var_pont[3] == NULL) prox_bloco_a_rodar = NULL; //se nao há nenhuma linha ligada a junção, para de rodar
            else if (var_pont[3]->var_pont[0] == NULL || var_pont[3]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL; //se a linha ligada não está ligada a outro bloco, para de rodar
            else if (var_pont[3]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[3]->var_pont[1]; //se o lado da linha ligado ao bloco for um, o proximo bloco é o que está ligada no proximo bloco é o outro
            else prox_bloco_a_rodar = var_pont[3]->var_pont[0];
        }
        else { //do contrário, mostra que mais uma iteração foi completa e retorna para o bloco ligado ao canto superior esquerdo
            rep_it_faltando--;
            if (var_pont[1] == NULL) prox_bloco_a_rodar = NULL; //se nao há nenhuma linha ligada a junção, para de rodar
            else if (var_pont[1]->var_pont[0] == NULL || var_pont[1]->var_pont[1] == NULL) prox_bloco_a_rodar = NULL; //se a linha ligada não está ligada a outro bloco, para de rodar
            else if (var_pont[1]->var_pont[0] == this) prox_bloco_a_rodar = var_pont[1]->var_pont[1]; //se o lado da linha ligado ao bloco for um, o proximo bloco é o que está ligada no proximo bloco é o outro
            else prox_bloco_a_rodar = var_pont[1]->var_pont[0];
        }
    }
	return prox_bloco_a_rodar;
}

bool colisao(int x1, int y1, ALLEGRO_BITMAP* obj1, int x2, int y2, ALLEGRO_BITMAP* obj2) {
//(x1,y1) e (x2,y2) sao as coordenadas onde os bitmaps sao desenhados, e obj1 e obj2 sao os bitmaps em si

// essa funcao foi praticamente copiada do video https://www.youtube.com/watch?v=okepAyR60bs&index=15&list=PLVg7a6fF-Gaj6DyuqygwqHqGhCu2w7Iub, apenas sendo adaptada para o allegro 5


	int i, j;
	int colisao = false;

	if (!((x1 > x2 + al_get_bitmap_width(obj2)) || (y1 > y2 + al_get_bitmap_height(obj2)) || (x2 > x1 + al_get_bitmap_width(obj1)) || (y2 > y1 + al_get_bitmap_height(obj1)))) {
		int cima = MAX(y1, y2);
		int baixo = MIN(y1 + al_get_bitmap_height(obj1), y2 + al_get_bitmap_height(obj2));
		int esquerda = MAX(x1, x2);
		int direita = MIN(x1 + al_get_bitmap_width(obj1), x2 + al_get_bitmap_width(obj2));
		//para testar se um pixel e transparente ou nao no allegro 5 temos um processo um pouco mais complexo. primeiro é necessario "travar" os bitmaps envolvidos, o que permite com que sejam avaliados mais rapidamente
		al_lock_bitmap(obj1, al_get_bitmap_format(obj1), ALLEGRO_LOCK_READONLY);
		al_lock_bitmap(obj2, al_get_bitmap_format(obj2), ALLEGRO_LOCK_READONLY);
		//então cria-se um objeto que contem uma cor do allegro para cada um dos bitmaps (ou seja duas)
		ALLEGRO_COLOR pixel1;
		ALLEGRO_COLOR pixel2;
		for (i = cima; i < baixo && !colisao; i++) {
			for (j = esquerda; j < direita && !colisao; j++) {
				//por fim, armazena-se a cor de cada pixel que precisa ser avaliado no objeto
				pixel1 = al_get_pixel(obj1, j - x1, i - y1);
				pixel2 = al_get_pixel(obj2, j - x2, i - y2);
				//finalmente, avalia-se se o campo transparencia dos objetos e diferente de 0, o que indicaria que eles sao transparentes ou nao
				if (pixel1.a != 0 && pixel1.a != 0) return true;
			}
		}
	}
	return false;
//esse processo nao e necessariamente o mais otimizado, mas a dificuldade de encontro de materiais sobre o assunto no allegro 5 fez com que tenha sido o utilizado
}

bool mouse_sobre(int x, int y, ALLEGRO_BITMAP* obj) {
//essa funcao e uma simplificacao de colisao() para ver se o mouse esta sobre algum bitmap
	if (mouse_x > x && mouse_x < x + al_get_bitmap_width(obj) && mouse_y_ajustado > y && mouse_y_ajustado < y + al_get_bitmap_height(obj)) {
		ALLEGRO_COLOR pixel;
		al_lock_bitmap(obj, al_get_bitmap_format(obj), ALLEGRO_LOCK_READONLY);
		pixel = al_get_pixel(obj, mouse_x - x, mouse_y_ajustado - y);
		if (pixel.a != 0) return true;
	}
	return false;
}


bool na_linha(int x1, int y1, int x2, int y2, int x, int y) {
    //verifica se as coordenadas (x,y) estão dentro do retângulo determinado por (x1,y1) e (x2,y2), com uma tolerancia definida por tolerancia_linha
    if (x <= MAX(x1,x2) + tolerancia_linha && x >= MIN(x1,x2) - tolerancia_linha && y <= MAX(y1, y2) + tolerancia_linha && y >= MIN(y1,y2) - tolerancia_linha) {
        //se a linha for horizontal ou vertical, estar dentro do retângulo já é suficiente para dizer que o mouse está sobre a linha
        if (x1 == x2 || y1 == y2) return true;

        //no caso da linha estar inclinada, podemos considerar que os pontos (x,y), (x1,y1) e (x2,y2) determinam um triângulo.
        //A distância do ponto (x,y) a linha seria, portanto, a altura do triângulo relativa ao lado determinado por (x1,y1) e (x2,y2)
        //sabendo que a = b*h/2 e que a = det(A)/2, sendo que
        //      | x  y  1 |
        // A =  | x1 y1 1 |
        //      | x2 y2 1 |
        // podemos dizer que b*h/2 = det(A)/2 -> b*h = det(A) -> h = det(A)/b.
        //como b é a distância entre (x1,y1) e (x2,y2), a expressão acima pode ser simplificada para
        // h = |x*(y1-y2) + x1*(y2-y) + x2*(y-y1)| / sqrt( (x1 - x2)² + (y1 - y2)²)
        // e se h for menor que tolerancia_linha, pode-se dizer que (x,y) está sobre a linha determinada por (x1,y1) e (x2,y2)
        else return abs(x*(y1-y2) + x1*(y2-y) + x2*(y-y1))/(sqrt( pow(x1 - x2, 2) + pow(y1 - y2, 2))) <= tolerancia_linha;
    }
    else return false;
}


void scprintf(int n, float x, float y){
	char buffer[20];
	sprintf(buffer, "%i", n);
	al_draw_text(fonte, preto, x, y, ALLEGRO_ALIGN_LEFT, buffer);
}

void scprintf(bloco* p, float x, float y){
	char buffer[20];
	sprintf(buffer, "%p", p);
	al_draw_text(fonte, preto, x, y, ALLEGRO_ALIGN_LEFT, buffer);
}

void scprintf(double d, float x, float y){
	char buffer[20];
	sprintf(buffer, "%f", d);
	al_draw_text(fonte, preto, x, y, ALLEGRO_ALIGN_LEFT, buffer);
}

void scprintf(float f, float x, float y){
	char buffer[20];
	sprintf(buffer, "%f", f);
	al_draw_text(fonte, preto, x, y, ALLEGRO_ALIGN_LEFT, buffer);
}

bool botao(float x, float y, float dx, float dy, float offset, bool ativo) { //dx = comprimento, dy = altura
    if (mouse_dentro_ret(x, y, x + dx, y + dy) && !ativo) {//verifica se o mous esta dentro da area determinada, e se sim desenha um retangulo no botão
        al_draw_rectangle(x - offset, y - offset, x + dx + offset, y + dy + offset, preto, 1);
        return true;
    }
    else return false;
}

#define x(X) [X/100 - 1][X%10-1][0]
#define y(X) [X/100 - 1][X%10-1][1]
void cord::inic() {
    //para economizar memória, são criados duas matrizes de 3 dimensões incompletas, ou seja, só há espaço alocado para o que vai ser utilizado

    //aloca espaço para as linhas da matriz (qual bloco)
    lin = new int**[6];

    //aloca espaço para as colunas da primeira linha da matriz (qual junção)
    lin[0] = new int*[3];
    for(int i = 0; i < 3; i++) lin[0][i] = new int[2]; //aloca espaço para as duas coordenadas do offset (x = 0, y = 1)

    //armazena os valores para cada campo
    lin x(dec_E) = al_get_bitmap_width(BLOCO[0][0])/2;
    lin y(dec_E) = 0;
    lin x(dec_S_nao) = al_get_bitmap_width(BLOCO[0][0]);
    lin y(dec_S_nao) = al_get_bitmap_height(BLOCO[0][0])/2;
    lin x(dec_S_sim) = al_get_bitmap_width(BLOCO[0][0])/2;
    lin y(dec_S_sim) = al_get_bitmap_height(BLOCO[0][0]);


    lin[1] = new int*[2];
    for(int i = 0; i < 2; i++) lin[1][i] = new int[2];

    lin x(acao_E) = al_get_bitmap_width(BLOCO[1][0])/2;
    lin y(acao_E) = 0;
    lin x(acao_S) = al_get_bitmap_width(BLOCO[1][0])/2;
    lin y(acao_S) = al_get_bitmap_height(BLOCO[1][0]);


    lin[2] = new int*[1];
    for(int i = 0; i < 1; i++) lin[2][i] = new int[2];

    lin x(ini_S) = al_get_bitmap_width(BLOCO[2][0])/2;
    lin y(ini_S) = al_get_bitmap_height(BLOCO[2][0]);


    lin[3] = new int*[1];
    for(int i = 0; i < 1; i++) lin[3][i] = new int[2];

    lin x(fim_E) = al_get_bitmap_width(BLOCO[3][0])/2;
    lin y(fim_E) = 0;


    lin[4] = new int*[3];
    for(int i = 0; i < 3; i++) lin[4][i] = new int[2];

    lin x(jun_E_esq) = 0;
    lin y(jun_E_esq) = 0;
    lin x(jun_E_dir) = al_get_bitmap_width(BLOCO[4][0]);
    lin y(jun_E_dir) = 0;
    lin x(jun_S) = al_get_bitmap_width(BLOCO[4][0])/2;
    lin y(jun_S) = al_get_bitmap_height(BLOCO[4][0]);

    lin[5] = new int*[4];
    for(int i = 0; i < 4; i++) lin[5][i] = new int[2];

    lin x(rep_E) = al_get_bitmap_width(BLOCO[5][0])/2;
    lin y(rep_E) = 0;
    lin x(rep_loop_ini) = al_get_bitmap_width(BLOCO[5][0]);
    lin y(rep_loop_ini) = 0;
    lin x(rep_loop_fim) = al_get_bitmap_width(BLOCO[5][0]);
    lin y(rep_loop_fim) = al_get_bitmap_height(BLOCO[5][0]);
    lin x(rep_S) = al_get_bitmap_width(BLOCO[5][0])/2;
    lin y(rep_S) = al_get_bitmap_height(BLOCO[5][0]);


    //o mesmo que o bloco anterior, mas para a outra matriz
    pon = new int**[6];

    pon[0] = new int*[3];
    for(int i = 0; i < 3; i++) pon[0][i] = new int[2];

    pon x(dec_E) = (al_get_bitmap_width(BLOCO[0][0])/2 - al_get_bitmap_width(PONTO[0])/2);
    pon y(dec_E) = (-al_get_bitmap_height(PONTO[0])/2);
    pon x(dec_S_nao) = (al_get_bitmap_width(BLOCO[0][0])-al_get_bitmap_width(PONTO[0])/2);
    pon y(dec_S_nao) = (al_get_bitmap_height(BLOCO[0][0])/2 - al_get_bitmap_height(PONTO[0])/2);
    pon x(dec_S_sim) = (al_get_bitmap_width(BLOCO[0][0])/2-al_get_bitmap_width(PONTO[0])/2);
    pon y(dec_S_sim) = (al_get_bitmap_height(BLOCO[0][0])- al_get_bitmap_height(PONTO[0])/2);


    pon[1] = new int*[2];
    for(int i = 0; i < 2; i++) pon[1][i] = new int[2];

    pon x(acao_E) = (al_get_bitmap_width(BLOCO[1][0])/2-al_get_bitmap_width(PONTO[0])/2);
    pon y(acao_E) = (-al_get_bitmap_height(PONTO[0])/2);
    pon x(acao_S) = (al_get_bitmap_width(BLOCO[1][0])/2-al_get_bitmap_width(PONTO[0])/2);
    pon y(acao_S) = (al_get_bitmap_height(BLOCO[1][0])-al_get_bitmap_height(PONTO[0])/2);


    pon[2] = new int*[1];
    for(int i = 0; i < 1; i++) pon[2][i] = new int[2];

    pon x(ini_S) = (al_get_bitmap_width(BLOCO[2][0])/2-al_get_bitmap_width(PONTO[0])/2);
    pon y(ini_S) = (al_get_bitmap_height(BLOCO[2][0])-al_get_bitmap_height(PONTO[0])/2);


    pon[3] = new int*[1];
    for(int i = 0; i < 1; i++) pon[3][i] = new int[2];

    pon x(fim_E) = (al_get_bitmap_width(BLOCO[3][0])/2-al_get_bitmap_width(PONTO[0])/2);
    pon y(fim_E) = (-al_get_bitmap_height(PONTO[0])/2);


    pon[4] = new int*[3];
    for(int i = 0; i < 3; i++) pon[4][i] = new int[2];

    pon x(jun_E_esq) = (-al_get_bitmap_width(PONTO[0])/2);
    pon y(jun_E_esq) = (-al_get_bitmap_height(PONTO[0])/2);
    pon x(jun_E_dir) = (al_get_bitmap_width(BLOCO[4][0])-al_get_bitmap_width(PONTO[0])/2);
    pon y(jun_E_dir) = (-al_get_bitmap_height(PONTO[0])/2);
    pon x(jun_S) = (al_get_bitmap_width(BLOCO[4][0])/2-al_get_bitmap_width(PONTO[0])/2);
    pon y(jun_S) = (al_get_bitmap_height(BLOCO[4][0])-al_get_bitmap_height(PONTO[0])/2);


    pon[5] = new int*[4];
    for(int i = 0; i < 4; i++) pon[5][i] = new int[2];

    pon x(rep_E) = (al_get_bitmap_width(BLOCO[5][0])/2 - al_get_bitmap_width(PONTO[0])/2);
    pon y(rep_E) = (-al_get_bitmap_height(PONTO[0])/2);
    pon x(rep_loop_ini) = (al_get_bitmap_width(BLOCO[5][0]) - al_get_bitmap_width(PONTO[0])/2);
    pon y(rep_loop_ini) = (-al_get_bitmap_height(PONTO[0])/2);
    pon x(rep_loop_fim) = (al_get_bitmap_width(BLOCO[5][0]) - al_get_bitmap_width(PONTO[0])/2);
    pon y(rep_loop_fim) = (al_get_bitmap_height(BLOCO[5][0])-al_get_bitmap_height(PONTO[0])/2);
    pon x(rep_S) = (al_get_bitmap_width(BLOCO[5][0])/2 - al_get_bitmap_width(PONTO[0])/2);
    pon y(rep_S) = (al_get_bitmap_height(BLOCO[5][0])-al_get_bitmap_height(PONTO[0])/2);
}

void cord::destruir() {

    for(int i = 0; i < 3; i++) delete lin[0][i];
    for(int i = 0; i < 2; i++) delete lin[1][i];
    for(int i = 0; i < 1; i++) delete lin[2][i];
    for(int i = 0; i < 1; i++) delete lin[3][i];
    for(int i = 0; i < 3; i++) delete lin[4][i];
    for(int i = 0; i < 4; i++) delete lin[5][i];
    for(int i = 0; i < 6; i++) delete lin[i];
    delete lin;

    for(int i = 0; i < 3; i++) delete pon[0][i];
    for(int i = 0; i < 2; i++) delete pon[1][i];
    for(int i = 0; i < 1; i++) delete pon[2][i];
    for(int i = 0; i < 1; i++) delete pon[3][i];
    for(int i = 0; i < 3; i++) delete pon[4][i];
    for(int i = 0; i < 4; i++) delete pon[5][i];
    for(int i = 0; i < 6; i++) delete pon[i];
    delete pon;

}
