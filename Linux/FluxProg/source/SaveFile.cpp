#include "SaveFile.hpp"

SaveFile :: SaveFile() {

}
SaveFile :: ~SaveFile() {

}
void SaveFile :: save(Block** _blocks_list_to_print, char* path) {

    out.open(path); // o arquivo que será criado;
    cout<<"caminho salvar "<<path<<endl;
    out<<"---*---STARTED THE PROCESS---*---\n";
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(_blocks_list_to_print[i] != NULL) {
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
                if(_blocks_list_to_print[i]->getPrevious1() != NULL) {
                    out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
                if(_blocks_list_to_print[i]->getNext1() != NULL) {
                    out<<_blocks_list_to_print[i]->getNext1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
                if(_blocks_list_to_print[i]->getNext2() != NULL) {
                    out<<_blocks_list_to_print[i]->getNext2()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
            } else if(_blocks_list_to_print[i]->getType() == ACTION_BLOCK) {
                if(_blocks_list_to_print[i]->getPrevious1() != NULL) {
                    out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
                if(_blocks_list_to_print[i]->getNext1() != NULL) {
                    out<<_blocks_list_to_print[i]->getNext1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
            } else if(_blocks_list_to_print[i]->getType() == START_BLOCK) {
                if(_blocks_list_to_print[i]->getNext1() != NULL) {
                    out<<_blocks_list_to_print[i]->getNext1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
            } else if(_blocks_list_to_print[i]->getType() == END_BLOCK) {
                if(_blocks_list_to_print[i]->getPrevious1() != NULL) {
                    out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
            } else if(_blocks_list_to_print[i]->getType() == MERGE_BLOCK) {
                if(_blocks_list_to_print[i]->getPrevious1() != NULL) {
                    out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
                if(_blocks_list_to_print[i]->getPrevious2() != NULL) {
                    out<<_blocks_list_to_print[i]->getPrevious2()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
                if(_blocks_list_to_print[i]->getNext1() != NULL) {
                    out<<_blocks_list_to_print[i]->getNext1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
            } else if(_blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                if(_blocks_list_to_print[i]->getPrevious1() != NULL) {
                    out<<_blocks_list_to_print[i]->getPrevious1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
                if(_blocks_list_to_print[i]->getPrevious2() != NULL) {
                    out<<_blocks_list_to_print[i]->getPrevious2()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
                if(_blocks_list_to_print[i]->getNext1() != NULL) {
                    out<<_blocks_list_to_print[i]->getNext1()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
                if(_blocks_list_to_print[i]->getNext2() != NULL) {
                    out<<_blocks_list_to_print[i]->getNext2()->getID();
                    out<<"\n";
                } else {
                    out<<"-1"<<endl;
                }
            }
            out<<"---block connections ended---\n";
        }
    }

    out<<"---*---FINISHED THE PROCESS---*---";
    out.close();
}
