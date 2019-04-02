#include "SaveFile.hpp"

SaveFile :: SaveFile() {

}
SaveFile :: ~SaveFile() {

}
void SaveFile :: save(Block** _blocks_list_to_print) {
    out.open("Nome_do_Arquivo.fluxpg"); // o arquivo que será criado;

    out<<"---*---STARTED THE PROCESS---*---\n";
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(_blocks_list_to_print[i] != NULL) {
            //out<<_blocks_list_to_print[i]->getName();
            //out<<"\n";
            out<<_blocks_list_to_print[i]->getType();
            out<<"\n";
            out<<_blocks_list_to_print[i]->getID();
            out<<"\n";
            out<<_blocks_list_to_print[i]->getX();
            out<<"\n";
            out<<_blocks_list_to_print[i]->getY();
            out<<"\n";
            out<<_blocks_list_to_print[i]->getWidth();
            out<<"\n";
            out<<_blocks_list_to_print[i]->getHeight();
            out<<"\n";
            if(_blocks_list_to_print[i]->getType() == CONDITIONAL_BLOCK) {
                out<<_blocks_list_to_print[i]->getTypeOfSensor();
                out<<"\n";
            } else if(_blocks_list_to_print[i]->getType() == ACTION_BLOCK) {
                out<<_blocks_list_to_print[i]->getFunction();
                out<<"\n";
            } else if(_blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                out<<_blocks_list_to_print[i]->getUnit();
                out<<"\n";
            }
            out<<"---block ended---\n";
        }
    }
    out<<"---started the connections---\n";
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(_blocks_list_to_print[i] != NULL) {

            out<<_blocks_list_to_print[i]->getID();
            out<<"\n";
            if(_blocks_list_to_print[i]->getType() == CONDITIONAL_BLOCK) {
                out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                out<<"\n";
                out<<_blocks_list_to_print[i]->getNext1()->getID();
                out<<"\n";
                out<<_blocks_list_to_print[i]->getNext2()->getID();
                out<<"\n";
            } else if(_blocks_list_to_print[i]->getType() == ACTION_BLOCK) {
                out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                out<<"\n";
                out<<_blocks_list_to_print[i]->getNext1()->getID();
                out<<"\n";
            } else if(_blocks_list_to_print[i]->getType() == START_BLOCK) {
                out<<_blocks_list_to_print[i]->getNext1()->getID();
                out<<"\n";
            } else if(_blocks_list_to_print[i]->getType() == END_BLOCK) {
                out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                out<<"\n";
            } else if(_blocks_list_to_print[i]->getType() == MERGE_BLOCK) {
                out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                out<<"\n";
                out<<_blocks_list_to_print[i]->getPrevious2()->getID();
                out<<"\n";
                out<<_blocks_list_to_print[i]->getNext1()->getID();
                out<<"\n";
            } else if(_blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                out<<"\n";
                out<<_blocks_list_to_print[i]->getPrevious2()->getID();
                out<<"\n";
                out<<_blocks_list_to_print[i]->getNext1()->getID();
                out<<"\n";
                out<<_blocks_list_to_print[i]->getNext2()->getID();
                out<<"\n";
            }
            out<<"---block connections ended---\n";
        }
    }

    out<<"---*---FINISHED THE PROCESS---*---";
    out.close(); // nã oesqueça de fechar...
}
