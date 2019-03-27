#include "FluxProg.hpp"

FluxProg :: FluxProg() {

    executing_fluxogram = false;
    current_executing_block = NULL;
    program_connected = false;


    for(int i=0; i<valor_maximo_blocos; i++) {
        blocks_list_to_print[i] = NULL;
    }

    interface = new Interface(blocks_list_to_print);
    //interface = new Interface();
    communication = new Communication();

}

FluxProg :: ~FluxProg() {

    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            delete blocks_list_to_print[i];
        }
    }

    delete interface;

    delete communication;
}

void FluxProg :: start() {

    while(interface->getExecuting()) {

        interface->draw();

        if(interface->getMenuClick() == PLAY) {
            //cout<<"play"<<endl;
            executing_fluxogram = true;
            //teste bloco de inicio
            if(check_if_only_one_startblock_exists() == false) {
                //erro blocos de inicio
                interface->callMessage(1);
                executing_fluxogram = false;
            }
            //teste blocos de fim
            if(check_if_at_least_one_endblock_exist() == false) {
                //erro blocos de fim
                interface->callMessage(2);
                executing_fluxogram = false;
            }
            //teste se todos os blocos estão ligados
            if(check_if_all_the_blocks_have_connections() == false) {
                //erro blocos de fim
                interface->callMessage(3);
                executing_fluxogram = false;
            }
            //teste se todos os blocos estão com as funções setadas
            if(check_if_all_blocks_have_functions_or_sensors() == false) {
                //erro blocos de fim
                interface->callMessage(4);
                executing_fluxogram = false;
            }
            //reseta as variáveis do loops para a execução
            for(int i=0; i<valor_maximo_blocos; i++) {
                if(blocks_list_to_print[i] != NULL) {
                    if(blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                        blocks_list_to_print[i]->reset_loop_variables();
                    }
                }
            }
            reset_fluxogram_execution();
        }
        if(interface->getMenuClick() == PAUSE) {
            //cout<<"pause"<<endl;
            executing_fluxogram = false;
        }
        if(interface->getMenuClick() == STOP) {
            //cout<<"stop"<<endl;
            executing_fluxogram = false;
            reset_fluxogram_execution();
        }
        if(interface->getMenuClick() == SAVE) {
            cout<<"save"<<endl;
        }
        if(interface->getMenuClick() == LOAD) {
            cout<<"load"<<endl;
        }
        if(interface->getMenuClick() == SAVE_AS) {
            cout<<"save_as"<<endl;
        }
        if(interface->getMenuClick() == PHYSICAL_ROBOT) {
            //cout<<"bluetooth"<<endl;
            connect_robot();
            connect();
        }
        if(interface->getMenuClick() == VIRTUAL_ROBOT) {
            //cout<<"vrep"<<endl;
            connect_simulator();
            connect();
        }


        if(interface->getMenuClick() == CONDITIONAL_BLOCK) {
            ConditionalBlock *aux = new ConditionalBlock();
            aux->setWidth(interface->getImageWidth(1));
            aux->setHeight(interface->getImageHeight(1));
            aux->setTypeOfSensor(0);
            aux->setX(interface->getMouseX()-60);
            aux->setY(interface->getMouseY()-35);
            aux->setSelected(true);
            aux->setDragging(true);
            aux->setName("bloco condicional");
            add_block(aux);
        }
        if(interface->getMenuClick() == ACTION_BLOCK) {
            ActionBlock *aux = new ActionBlock();
            aux->setWidth(interface->getImageWidth(2));
            aux->setHeight(interface->getImageHeight(2));
            aux->setFunction(0);
            aux->setX(interface->getMouseX()-45);
            aux->setY(interface->getMouseY()-20);
            aux->setSelected(true);
            aux->setDragging(true);
            aux->setName("bloco de ação");
            add_block(aux);
        }
        if(interface->getMenuClick() == START_BLOCK) {
            StartBlock *aux = new StartBlock();
            aux->setWidth(interface->getImageWidth(3));
            aux->setHeight(interface->getImageHeight(3));
            aux->setName("bloco inicio");
            aux->setX(interface->getMouseX()-40);
            aux->setY(interface->getMouseY()-15);
            aux->setSelected(true);
            aux->setDragging(true);
            add_block(aux);
        }
        if(interface->getMenuClick() == END_BLOCK) {
            EndBlock *aux = new EndBlock();
            aux->setWidth(interface->getImageWidth(4));
            aux->setHeight(interface->getImageHeight(4));
            aux->setX(interface->getMouseX()-40);
            aux->setY(interface->getMouseY()-15);
            aux->setName("bloco de fim");
            aux->setSelected(true);
            aux->setDragging(true);
            add_block(aux);
        }
        if(interface->getMenuClick() == MERGE_BLOCK) {
            MergeBlock *aux = new MergeBlock();
            aux->setWidth(interface->getImageWidth(5));
            aux->setHeight(interface->getImageHeight(5));
            aux->setX(interface->getMouseX()-15);
            aux->setY(interface->getMouseY()-10);
            aux->setName("bloco de junção");
            aux->setSelected(true);
            aux->setDragging(true);
            add_block(aux);
        }
        if(interface->getMenuClick() == LOOP_BLOCK) {
            LoopBlock *aux = new LoopBlock();
            aux->setWidth(interface->getImageWidth(6));
            aux->setHeight(interface->getImageHeight(6));
            aux->setX(interface->getMouseX()-30);
            aux->setY(interface->getMouseY()-40);
            aux->setName("bloco de loop");
            aux->setLimitedLoop(true);
            aux->setNumberOfLoops(2);
            aux->setSelected(true);
            aux->setDragging(true);
            add_block(aux);
        }
        //executa o fluxograma
        if(executing_fluxogram == true) {
            execute();
        }

        for(int i=0; i<valor_maximo_blocos; i++) {
            if(blocks_list_to_print[i] != NULL) {
                if(blocks_list_to_print[i]->getDelete() == true) {
                    remove_block(blocks_list_to_print[i]);
                }
            }
        }

    }

    communication->setCommand(CLOSE_PROGRAM);
}

void FluxProg :: add_block(Block *b) {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] == NULL) {
            blocks_list_to_print[i] = b;
            //cout<< "adicionou na lista" << endl;
            break;
        }
    }

}
void FluxProg :: remove_block(Block *b) {
    //percorre a lista de blocos em busca do bloco a ser excluido
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            //se o bloco tiver relação com algum elimina essa relação
            if(blocks_list_to_print[i]->getNext1() == b) {
                blocks_list_to_print[i]->setNext1(NULL);
                //cout<<"exclui ligação next1"<<endl;
            }
            if(blocks_list_to_print[i]->getNext2() == b) {
                blocks_list_to_print[i]->setNext2(NULL);
                //cout<<"exclui ligação next2"<<endl;
            }
            if(blocks_list_to_print[i]->getPrevious1() == b) {
                blocks_list_to_print[i]->setPrevious1(NULL);
                //cout<<"exclui ligação previous1"<<endl;
            }
            if(blocks_list_to_print[i]->getPrevious2() == b) {
                blocks_list_to_print[i]->setPrevious2(NULL);
                //cout<<"exclui ligação previous2"<<endl;
            }
        }
    }
    //cout<<"removeu ligações"<<endl;
    //percorre a lista de blocos em busca do bloco a ser excluido
    for(int i=0; i<valor_maximo_blocos; i++) {
        //encontra o bloco passado como parametro
        if(blocks_list_to_print[i] == b) {
            blocks_list_to_print[i] = NULL;
            delete b;
            //cout<< "removeu da lista: " << i << endl;
            break;
        }
    }
}
void FluxProg :: execute() {
    if(current_executing_block != NULL) {

        if(program_connected == true) {
            //se for do tipo 8 = condicional
            //tem q fazer leitura de sensores para setar as variáveis de comparação
            if(current_executing_block->getType() == CONDITIONAL_BLOCK) {
                int* black_sensor_reading = communication->getBlackTypeReading();
                int* ultrasonic_sensor_reading = communication->getUltrasonicReading();
                //checa tipo de sensor
                switch(current_executing_block->getTypeOfSensor()) {
                    case 1:
                        //black sensor 1
                        current_executing_block->setParameter1(black_sensor_reading[0]);
                        current_executing_block->setParameter2(1);
                        break;

                    case 2:
                        //black sensor 2
                        current_executing_block->setParameter1(black_sensor_reading[1]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 3:
                        //black sensor 3
                        current_executing_block->setParameter1(black_sensor_reading[2]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 4:
                        //black sensor 4
                        current_executing_block->setParameter1(black_sensor_reading[3]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 5:
                        //black sensor 5
                        current_executing_block->setParameter1(black_sensor_reading[4]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 6:
                        //color sensor 1
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 7:
                        //color sensor 2
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 8:
                        //ultrasonic sensor 1
                        current_executing_block->setParameter1(ultrasonic_sensor_reading[0]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 9:
                        //ultrasonic sensor 2
                        current_executing_block->setParameter1(ultrasonic_sensor_reading[1]);
                        current_executing_block->setParameter2(1);
                        break;
                    case 10:
                        //ultrasonic sensor 3
                        current_executing_block->setParameter1(ultrasonic_sensor_reading[2]);
                        current_executing_block->setParameter2(1);
                        break;
                }
            }
            //se for 1 significa que terminou execução ou está pronto para receber
            if(communication->getFeedback() == READY) {
                //testa se o próximo é não nulo
                if(current_executing_block->getExecutingNext() != NULL) {
                    cout<<"executou bloco: "<<current_executing_block->getName()<<endl;
                    current_executing_block = current_executing_block->getExecutingNext();
                    refresh_executing_block();
                    //bloco de ação
                    if(current_executing_block->getType() == ACTION_BLOCK) {
                        communication->setCommand(current_executing_block->getCommand());
                    }
                } else {
                    cout<<"executou bloco: "<<current_executing_block->getName()<<endl;
                    executing_fluxogram = false;
                }
            }
        } else {
            //se for do tipo 8 = condicional
            //tem q fazer leitura de sensores para setar as variáveis de comparação
            if(current_executing_block->getType() == CONDITIONAL_BLOCK) {
                //checa tipo de sensor
                switch(current_executing_block->getTypeOfSensor()) {
                    case 1:
                        //black sensor 1
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;

                    case 2:
                        //black sensor 2
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 3:
                        //black sensor 3
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 4:
                        //black sensor 4
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 5:
                        //black sensor 5
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 6:
                        //color sensor 1
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 7:
                        //color sensor 2
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 8:
                        //ultrasonic sensor 1
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                    case 9:
                        //ultrasonic sensor 2
                        current_executing_block->setParameter1(0);
                        current_executing_block->setParameter2(1);
                        break;
                    case 10:
                        //ultrasonic sensor 3
                        current_executing_block->setParameter1(1);
                        current_executing_block->setParameter2(1);
                        break;
                }
            }
            if(current_executing_block->getExecutingNext() != NULL) {
                cout<<"executou bloco: "<<current_executing_block->getName()<<endl;
                current_executing_block = current_executing_block->getExecutingNext();
                refresh_executing_block();
            } else {
                cout<<"executou bloco: "<<current_executing_block->getName()<<endl;
                executing_fluxogram = false;
            }
        }

    }
}
bool FluxProg :: check_if_only_one_startblock_exists() {
    int start_test = 0;
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == START_BLOCK) {
                start_test = start_test + 1;
            }
        }
    }
    if(start_test == 1) {
        return true;
    } else {
        return false;
    }
}
bool FluxProg :: check_if_at_least_one_endblock_exist() {
    int end_test = 0;
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == END_BLOCK) {
                end_test = end_test + 1;
            }
        }
    }
    if(end_test >= 1) {
        return true;
    } else {
        return false;
    }
}
bool FluxProg :: check_if_all_the_blocks_have_connections() {
    int connections_test = 1;
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == ACTION_BLOCK) {
                //function
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == END_BLOCK) {
                //end
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == START_BLOCK) {
                //start
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                //loop
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getNext2() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious2() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == CONDITIONAL_BLOCK) {
                //decision
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getNext2() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
            } else if(blocks_list_to_print[i]->getType() == MERGE_BLOCK) {
                //merge
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    connections_test = 0;
                    break;
                }
                if(blocks_list_to_print[i]->getPrevious2() == NULL) {
                    connections_test = 0;
                    break;
                }
            }
        }
    }
    if(connections_test == 1) {
        return true;
    } else {
        return false;
    }
}
bool FluxProg :: check_if_all_blocks_have_functions_or_sensors() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == ACTION_BLOCK) {
                //ação
                if(blocks_list_to_print[i]->getFunction() == 0) {
                    return false;
                }
            } else if(blocks_list_to_print[i]->getType() == CONDITIONAL_BLOCK) {
                //decisão
                if(blocks_list_to_print[i]->getTypeOfSensor() == 0) {
                    return false;
                }
            }

        }
    }
    return true;
}
void FluxProg :: reset_fluxogram_execution() {
    int id_start = 0;
    //procura pelo bloco de inicio
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == START_BLOCK) {
                id_start = i;
                break;
            }
        }
    }
    //seta start como bloco atual
    current_executing_block = blocks_list_to_print[id_start];
}
void FluxProg :: connect_simulator() {
    communication->setIfVirtual(1);
    simulator_connected = true;
}
void FluxProg :: connect_robot() {
    communication->setIfVirtual(0);
    simulator_connected = false;
}
void FluxProg :: connect() {
    int feedback = 10;
    pid_t pid;
    pid = fork();
    switch (pid) {
        case -1 :
            //erro no fork
            feedback = -1;
            break;
        case 0 :
            //processo filho
            execl(".//..//FluxProgBackend//build//fluxprogbackend", "fluxprogbackend", NULL);
            //cout<<"caminho incorreto"<<endl;
            interface->callMessage(5);
            kill(getpid(), SIGTERM);
            break;
        default :
            //processo pai
            feedback = 0;
    }

    if(feedback == 0) {
        sleep(2);
        communication->upadateReadings();
        //cout<<"abriu o programa"<<endl;
        feedback = communication->getFeedback();
        if(feedback == ERROR){
            //não abriu o v-rep ou não tem bluetooth
            //cout<<"não abriu o v-rep ou não tem bluetooth"<<endl;
            if(simulator_connected == true) {
                interface->callMessage(7);
            } else {
                interface->callMessage(6);
            }
        } else if(feedback == READY) {
            if(simulator_connected == true) {
                interface->callMessage(8);
            } else {
                interface->callMessage(9);
            }
            program_connected = true;
        }
        else{
            //cout << "ainda deu problema, rein vr"<<endl;
            if(simulator_connected == true) {
                interface->callMessage(10);
            } else {
                interface->callMessage(11);
            }
        }
    }
}
void FluxProg :: refresh_executing_block() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i] == current_executing_block) {
                blocks_list_to_print[i]->setExecuting(true);
            } else {
                blocks_list_to_print[i]->setExecuting(false);
            }
        }
    }
}

//1. fazer submenu de numeros
//2. setar bloco de loop com numeros
//3. checar ligações entre blocos
