#include "SaveFile.hpp"

SaveFile :: SaveFile() {

}
SaveFile :: ~SaveFile() {

}
void SaveFile :: save() {
    out.open("Nome_do_Arquivo.fluxpg"); // o arquivo que será criado;
    out<<"SAIDA  DE TEXTO";
    out.close(); // nã oesqueça de fechar...
}
