#include "LoadFile.hpp"

LoadFile :: LoadFile(Block** _blocks_list_to_print) {
    blocks_list_to_print = _blocks_list_to_print;
}
LoadFile :: ~LoadFile() {
}
int LoadFile :: load(char* path) {
    in.open(path);
    std::string line;
    getline(in, line); //started the process
    if(line != "---*---STARTED THE PROCESS---*---") {
        return 0;
    }
    getline(in, line);
    int i = 0; //variável responsavel pelo id da lista
    while(line != "---started the connections---") {
        int type = atoi(line.c_str());
        if(type == CONDITIONAL_BLOCK) {
            ConditionalBlock *aux = new ConditionalBlock();
            getline(in, line);
            aux->setID(atoi(line.c_str()));
            getline(in, line);
            aux->setX(atoi(line.c_str()));
            getline(in, line);
            aux->setY(atoi(line.c_str()));
            getline(in, line);
            aux->setWidth(atoi(line.c_str()));
            getline(in, line);
            aux->setHeight(atoi(line.c_str()));
            aux->setSelected(false);
            aux->setDragging(false);
            aux->setName("bloco condicional");
            getline(in, line);
            aux->setTypeOfSensor(atoi(line.c_str()));
            getline(in, line); //block ended
            getline(in, line); //next type
            blocks_list_to_print[i] = aux;
            i++;
        } else if(type == ACTION_BLOCK) {
            ActionBlock *aux = new ActionBlock();
            getline(in, line);
            aux->setID(atoi(line.c_str()));
            getline(in, line);
            aux->setX(atoi(line.c_str()));
            getline(in, line);
            aux->setY(atoi(line.c_str()));
            getline(in, line);
            aux->setWidth(atoi(line.c_str()));
            getline(in, line);
            aux->setHeight(atoi(line.c_str()));
            aux->setSelected(false);
            aux->setDragging(false);
            aux->setName("bloco ação");
            getline(in, line);
            aux->setFunction(atoi(line.c_str()));
            getline(in, line); //block ended
            getline(in, line); //next type
            blocks_list_to_print[i] = aux;
            i++;
        } else if(type == START_BLOCK) {
            StartBlock *aux = new StartBlock();
            getline(in, line);
            aux->setID(atoi(line.c_str()));
            getline(in, line);
            aux->setX(atoi(line.c_str()));
            getline(in, line);
            aux->setY(atoi(line.c_str()));
            getline(in, line);
            aux->setWidth(atoi(line.c_str()));
            getline(in, line);
            aux->setHeight(atoi(line.c_str()));
            aux->setSelected(false);
            aux->setDragging(false);
            aux->setName("bloco inicio");
            getline(in, line); //block ended
            getline(in, line); //next type
            blocks_list_to_print[i] = aux;
            i++;
        } else if(type == END_BLOCK) {
            EndBlock *aux = new EndBlock();
            getline(in, line);
            aux->setID(atoi(line.c_str()));
            getline(in, line);
            aux->setX(atoi(line.c_str()));
            getline(in, line);
            aux->setY(atoi(line.c_str()));
            getline(in, line);
            aux->setWidth(atoi(line.c_str()));
            getline(in, line);
            aux->setHeight(atoi(line.c_str()));
            aux->setSelected(false);
            aux->setDragging(false);
            aux->setName("bloco fim");
            getline(in, line); //block ended
            getline(in, line); //next type
            blocks_list_to_print[i] = aux;
            i++;
        } else if(type == MERGE_BLOCK) {
            MergeBlock *aux = new MergeBlock();
            getline(in, line);
            aux->setID(atoi(line.c_str()));
            getline(in, line);
            aux->setX(atoi(line.c_str()));
            getline(in, line);
            aux->setY(atoi(line.c_str()));
            getline(in, line);
            aux->setWidth(atoi(line.c_str()));
            getline(in, line);
            aux->setHeight(atoi(line.c_str()));
            aux->setSelected(false);
            aux->setDragging(false);
            aux->setName("bloco junçao");
            getline(in, line); //block ended
            getline(in, line); //next type
            blocks_list_to_print[i] = aux;
            i++;
        } else if(type == LOOP_BLOCK) {
            LoopBlock *aux = new LoopBlock();
            getline(in, line);
            aux->setID(atoi(line.c_str()));
            getline(in, line);
            aux->setX(atoi(line.c_str()));
            getline(in, line);
            aux->setY(atoi(line.c_str()));
            getline(in, line);
            aux->setWidth(atoi(line.c_str()));
            getline(in, line);
            aux->setHeight(atoi(line.c_str()));
            aux->setSelected(false);
            aux->setDragging(false);
            aux->setLimitedLoop(true);
            aux->setName("bloco loop");
            getline(in, line);
            aux->setValue(atoi(line.c_str()));
            getline(in, line); //block ended
            getline(in, line); //next type
            blocks_list_to_print[i] = aux;
            i++;
        }
        if(line == " ") {
            return 0;
        }
    }
    i = 0;
    getline(in, line); //le valor do id do bloco
    while(line != "---*---FINISHED THE PROCESS---*---") {
        for(int j=0; j<valor_maximo_blocos; j++) {
            if(blocks_list_to_print[j] != NULL) {
                if(blocks_list_to_print[j]->getID() == atoi(line.c_str())) { //procura o bloco

                    if(blocks_list_to_print[j]->getType() == CONDITIONAL_BLOCK) {
                        getline(in, line);//le valor do previous1
                        int previous1ID = atoi(line.c_str());
                        getline(in, line);//le valor do next1
                        int next1ID = atoi(line.c_str());
                        getline(in, line);//le valor do next2
                        int next2ID = atoi(line.c_str());
                        for(int k=0; k<valor_maximo_blocos; k++) {
                            if(blocks_list_to_print[k] != NULL) {
                                if(blocks_list_to_print[k]->getID() == previous1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setPrevious1(blocks_list_to_print[k]);
                                }
                                if(blocks_list_to_print[k]->getID() == next1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setNext1(blocks_list_to_print[k]);
                                }
                                if(blocks_list_to_print[k]->getID() == next2ID) {//procura o bloco
                                    blocks_list_to_print[j]->setNext2(blocks_list_to_print[k]);
                                }
                            }
                        }
                        getline(in, line);
                        break;

                    } else if(blocks_list_to_print[j]->getType() == ACTION_BLOCK) {
                        getline(in, line);//le valor do previous1
                        int previous1ID = atoi(line.c_str());
                        getline(in, line);//le valor do next1
                        int next1ID = atoi(line.c_str());
                        for(int k=0; k<valor_maximo_blocos; k++) {
                            if(blocks_list_to_print[k] != NULL) {
                                if(blocks_list_to_print[k]->getID() == previous1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setPrevious1(blocks_list_to_print[k]);
                                }
                                if(blocks_list_to_print[k]->getID() == next1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setNext1(blocks_list_to_print[k]);
                                }
                            }
                        }
                        getline(in, line);
                        break;

                    } else if(blocks_list_to_print[j]->getType() == START_BLOCK) {
                        getline(in, line);//le valor do next1
                        int next1ID = atoi(line.c_str());
                        for(int k=0; k<valor_maximo_blocos; k++) {
                            if(blocks_list_to_print[k] != NULL) {
                                if(blocks_list_to_print[k]->getID() == next1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setNext1(blocks_list_to_print[k]);
                                }
                            }
                        }
                        getline(in, line);
                        break;

                    } else if(blocks_list_to_print[j]->getType() == END_BLOCK) {
                        getline(in, line);//le valor do previous1
                        int previous1ID = atoi(line.c_str());
                        for(int k=0; k<valor_maximo_blocos; k++) {
                            if(blocks_list_to_print[k] != NULL) {
                                if(blocks_list_to_print[k]->getID() == previous1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setPrevious1(blocks_list_to_print[k]);
                                }
                            }
                        }
                        getline(in, line);
                        break;

                    } else if(blocks_list_to_print[j]->getType() == MERGE_BLOCK) {
                        getline(in, line);//le valor do previous1
                        int previous1ID = atoi(line.c_str());
                        getline(in, line);//le valor do next1
                        int previous2ID = atoi(line.c_str());
                        getline(in, line);//le valor do next2
                        int next1ID = atoi(line.c_str());
                        for(int k=0; k<valor_maximo_blocos; k++) {
                            if(blocks_list_to_print[k] != NULL) {
                                if(blocks_list_to_print[k]->getID() == previous1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setPrevious1(blocks_list_to_print[k]);
                                }
                                if(blocks_list_to_print[k]->getID() == previous2ID) {//procura o bloco
                                    blocks_list_to_print[j]->setPrevious2(blocks_list_to_print[k]);
                                }
                                if(blocks_list_to_print[k]->getID() == next1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setNext1(blocks_list_to_print[k]);
                                }
                            }
                        }
                        getline(in, line);
                        break;

                    } else if(blocks_list_to_print[j]->getType() == LOOP_BLOCK) {
                        getline(in, line);//le valor do previous1
                        int previous1ID = atoi(line.c_str());
                        getline(in, line);//le valor do previous2
                        int previous2ID = atoi(line.c_str());
                        getline(in, line);//le valor do next1
                        int next1ID = atoi(line.c_str());
                        getline(in, line);//le valor do next2
                        int next2ID = atoi(line.c_str());
                        for(int k=0; k<valor_maximo_blocos; k++) {
                            if(blocks_list_to_print[k] != NULL) {
                                if(blocks_list_to_print[k]->getID() == previous1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setPrevious1(blocks_list_to_print[k]);
                                }
                                if(blocks_list_to_print[k]->getID() == previous2ID) {//procura o bloco
                                    blocks_list_to_print[j]->setPrevious2(blocks_list_to_print[k]);
                                }
                                if(blocks_list_to_print[k]->getID() == next1ID) {//procura o bloco
                                    blocks_list_to_print[j]->setNext1(blocks_list_to_print[k]);
                                }
                                if(blocks_list_to_print[k]->getID() == next2ID) {//procura o bloco
                                    blocks_list_to_print[j]->setNext2(blocks_list_to_print[k]);
                                }
                            }
                        }
                        getline(in, line);
                        break;
                    }
                }
            }
        }
        getline(in, line);//le o proximo ID
        if(line == " ") {
            return 0;
        }
    }

    in.close(); // nã oesqueça de fechar...
    return 1;
}
