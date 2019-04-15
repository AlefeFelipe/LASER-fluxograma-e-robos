#include "FluxProg.hpp"

FluxProg :: FluxProg() {

    executing_fluxogram = false;
    current_executing_block = NULL;
    program_connected = false;
    paused = false;
    ids = 0;
    already_saved = false;
    path = NULL;

    for(int i=0; i<valor_maximo_blocos; i++) {
        blocks_list_to_print[i] = NULL;
    }
    program_path = getExecutablePath();
    program_path += "/";
    gui = new Interface(blocks_list_to_print, program_path);
    #ifndef _WIN32
        size_t pos = program_path.find(" ", 0);
        while(pos != string::npos)
        {
            program_path.replace(pos, 1, "\\ ");
            pos = program_path.find(" ", pos+2);
        }
    #endif
    communication = new Communication();
    save = new SaveFile();
    load = new LoadFile(blocks_list_to_print);

}
FluxProg :: ~FluxProg() {

    for(int i=0; i<valor_maximo_blocos; i++) {
        delete blocks_list_to_print[i];
    }

    delete gui;

    delete communication;

    delete save;

    delete load;
}

void FluxProg :: start() {

    while(gui->getExecuting()) {

        gui->draw();

        if(gui->getMenuClick() == PLAY) {
            //std::cout<<"play"<<std::endl;
            executing_fluxogram = true;
            //teste bloco de inicio
            if((check_if_only_one_startblock_exists() == false) && (executing_fluxogram == true)) {
                //erro blocos de inicio
                gui->callMessage(1);
                executing_fluxogram = false;
            }
            //teste blocos de fim
            if((check_if_at_least_one_endblock_exist() == false) && (executing_fluxogram == true)) {
                //erro blocos de fim
                gui->callMessage(2);
                executing_fluxogram = false;
            }
            //teste se todos os blocos estão ligados
            if((check_if_all_the_blocks_have_connections() == false) && (executing_fluxogram == true)) {
                //erro blocos de fim
                gui->callMessage(3);
                executing_fluxogram = false;
                int delete_blocks_with_no_connections = gui->callMessage(20);
                if(delete_blocks_with_no_connections == 0) {
                    delete_blocks_with_no_connections_function();
                }
            }
            //teste se todos os blocos estão com as funções setadas
            if((check_if_all_blocks_have_functions_or_sensors() == false) && (executing_fluxogram == true)) {
                //erro blocos de fim
                gui->callMessage(4);
                executing_fluxogram = false;
            }
            if(paused == false) {
                reset_fluxogram_execution();
            } else {
                paused = false;
            }
            //seta o nível de abstração do simulador baseado nas leitura que o programa faz
            communication->setAbstractionLevel(check_abstraction_level());
        }
        if(gui->getMenuClick() == PAUSE) {
            executing_fluxogram = false;
            paused = true;
        }
        if(gui->getMenuClick() == STOP) {
            executing_fluxogram = false;
            paused = false;
            reset_fluxogram_execution();
            reset_executing_block();
        }
        if(gui->getMenuClick() == SAVE) {
            if(already_saved) {
                save->save(blocks_list_to_print, path);
                gui->callMessage(15);
            } else {
                path = gui->save_file_window();
                if(path != NULL) {
                    save->save(blocks_list_to_print, path);
                    already_saved = true;
                    gui->callMessage(15);
                }
            }
        }
        if(gui->getMenuClick() == LOAD) {
            path = gui->open_file_window();
            if(path != NULL) {
                reset_blocks_list();
                gui->reset_scrollbar();
                int t = load->load(path);
                if(t == 1) {
                    already_saved = true;
                    for(int i=0; i<valor_maximo_blocos; i++) {
                        if(blocks_list_to_print[i] != NULL) {
                            if(blocks_list_to_print[i]->getID() >= ids) {
                                ids = blocks_list_to_print[i]->getID()+1;
                            }
                        }
                    }

                } else {
                    gui->callMessage(16);
                }
            }
        }
        if(gui->getMenuClick() == SAVE_AS) {
            path = gui->save_file_window();
            if(path != NULL) {
                save->save(blocks_list_to_print, path);
                already_saved = true;
                gui->callMessage(15);
            }
        }
        if(gui->getMenuClick() == PHYSICAL_ROBOT) {
            if((program_connected == true) && (simulator_connected == false)){
                communication->setCommand(CLOSE_PROGRAM);
                gui->setConnectedRobot(false);
                program_connected = false;
                gui->callMessage(13);

            } else if((program_connected == true) && (simulator_connected == true)){
                communication->setCommand(CLOSE_PROGRAM);
                gui->setConnectedSimulator(false);
                communication->inicialize();
                connect_robot();
                connect();
            } else {
                communication->inicialize();
                connect_robot();
                connect();
            }
        }
        if(gui->getMenuClick() == VIRTUAL_ROBOT) {
            //std::cout<<"vrep"<<std::endl;
            if((program_connected == true) && (simulator_connected == true)){
                communication->setCommand(CLOSE_PROGRAM);
                gui->setConnectedSimulator(false);
                program_connected = false;
                gui->callMessage(12);

            } else if((program_connected == true) && (simulator_connected == false)){
                communication->setCommand(CLOSE_PROGRAM);
                gui->setConnectedRobot(false);
                communication->inicialize();
                connect_simulator();
                connect();
            } else {
                communication->inicialize();
                connect_simulator();
                connect();
            }
        }
        if(gui->getMenuClick() == CONDITIONAL_BLOCK) {
            ConditionalBlock *aux = new ConditionalBlock();
            aux->setWidth(gui->getImageWidth(1));
            aux->setHeight(gui->getImageHeight(1));
            aux->setTypeOfSensor(0);
            aux->setX(gui->getMouseX()-60);
            aux->setY(gui->getMouseY()-35);
            aux->setSelected(true);
            aux->setDragging(true);
            aux->setName("bloco condicional");
            aux->setID(ids);
            ids = ids + 1;
            add_block(aux);
        }
        if(gui->getMenuClick() == ACTION_BLOCK) {
            ActionBlock *aux = new ActionBlock();
            aux->setWidth(gui->getImageWidth(2));
            aux->setHeight(gui->getImageHeight(2));
            aux->setFunction(0);
            aux->setX(gui->getMouseX()-45);
            aux->setY(gui->getMouseY()-20);
            aux->setSelected(true);
            aux->setDragging(true);
            aux->setName("bloco de ação");
            aux->setID(ids);
            ids = ids + 1;
            add_block(aux);
        }
        if(gui->getMenuClick() == START_BLOCK) {
            StartBlock *aux = new StartBlock();
            aux->setWidth(gui->getImageWidth(3));
            aux->setHeight(gui->getImageHeight(3));
            aux->setName("bloco inicio");
            aux->setX(gui->getMouseX()-40);
            aux->setY(gui->getMouseY()-15);
            aux->setSelected(true);
            aux->setDragging(true);
            aux->setID(ids);
            ids = ids + 1;
            add_block(aux);
        }
        if(gui->getMenuClick() == END_BLOCK) {
            EndBlock *aux = new EndBlock();
            aux->setWidth(gui->getImageWidth(4));
            aux->setHeight(gui->getImageHeight(4));
            aux->setX(gui->getMouseX()-40);
            aux->setY(gui->getMouseY()-15);
            aux->setName("bloco de fim");
            aux->setSelected(true);
            aux->setDragging(true);
            aux->setID(ids);
            ids = ids + 1;
            add_block(aux);
        }
        if(gui->getMenuClick() == MERGE_BLOCK) {
            MergeBlock *aux = new MergeBlock();
            aux->setWidth(gui->getImageWidth(5));
            aux->setHeight(gui->getImageHeight(5));
            aux->setX(gui->getMouseX()-15);
            aux->setY(gui->getMouseY()-10);
            aux->setName("bloco de junção");
            aux->setSelected(true);
            aux->setDragging(true);
            aux->setID(ids);
            ids = ids + 1;
            add_block(aux);
        }
        if(gui->getMenuClick() == LOOP_BLOCK) {
            LoopBlock *aux = new LoopBlock();
            aux->setWidth(gui->getImageWidth(6));
            aux->setHeight(gui->getImageHeight(6));
            aux->setX(gui->getMouseX()-30);
            aux->setY(gui->getMouseY()-40);
            aux->setName("bloco de loop");
            aux->setLimitedLoop(true);
            aux->setValue(0);
            aux->setSelected(true);
            aux->setDragging(true);
            aux->setID(ids);
            ids = ids + 1;
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

        gui->setExecutingFluxogram(executing_fluxogram);

    }

    communication->setCommand(CLOSE_PROGRAM);
}

void FluxProg :: add_block(Block *b) {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] == NULL) {
            blocks_list_to_print[i] = b;
            //std::cout<< "adicionou na lista" << std::endl;
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
                //std::cout<<"exclui ligação next1"<<std::endl;
            }
            if(blocks_list_to_print[i]->getNext2() == b) {
                blocks_list_to_print[i]->setNext2(NULL);
                //std::cout<<"exclui ligação next2"<<std::endl;
            }
            if(blocks_list_to_print[i]->getPrevious1() == b) {
                blocks_list_to_print[i]->setPrevious1(NULL);
                //std::cout<<"exclui ligação previous1"<<std::endl;
            }
            if(blocks_list_to_print[i]->getPrevious2() == b) {
                blocks_list_to_print[i]->setPrevious2(NULL);
                //std::cout<<"exclui ligação previous2"<<std::endl;
            }
        }
    }
    //std::cout<<"removeu ligações"<<std::endl;
    //percorre a lista de blocos em busca do bloco a ser excluido
    for(int i=0; i<valor_maximo_blocos; i++) {
        //encontra o bloco passado como parametro
        if(blocks_list_to_print[i] == b) {
            blocks_list_to_print[i] = NULL;
            delete b;
            //std::cout<< "removeu da lista: " << i << std::endl;
            break;
        }
    }
}
void FluxProg :: execute() {
    if(current_executing_block != NULL) {

        if(program_connected == true) {
            if(communication->getFeedback() == CONNECT_ERROR)
            {
                if(simulator_connected)
                {
                   gui->callMessage(17);
                } else {
                   gui->callMessage(18);
                }
                communication->setCommand(CLOSE_PROGRAM);
                program_connected = false;
                reset_executing_block();
                gui->setConnectedRobot(false);
                gui->setConnectedSimulator(false);
            }
            else {
                //se for do tipo 8 = condicional
                //tem q fazer leitura de sensores para setar as variáveis de comparação
                if(current_executing_block->getType() == CONDITIONAL_BLOCK) {
                    communication->upadateReadings();
                    int* black_sensor_reading = communication->getBlackTypeReading();
                    int* ultrasonic_sensor_reading = communication->getUltrasonicReading();
                    int low_limit_ultrasonic = 19;
                    int high_limit_ultrasonic = 21;
                    //checa tipo de sensor
                    switch(current_executing_block->getTypeOfSensor()) {
                        case 1:
                            //black sensor 1
                            current_executing_block->setParameter1(black_sensor_reading[0]);
                            current_executing_block->setParameter2(1);
                            cout<<"sensor: "<<black_sensor_reading[0]<<endl;
                            break;

                        case 2:
                            //black sensor 2
                            current_executing_block->setParameter1(black_sensor_reading[1]);
                            current_executing_block->setParameter2(1);
                            cout<<"sensor: "<<black_sensor_reading[1]<<endl;
                            break;
                        case 3:
                            //black sensor 3
                            current_executing_block->setParameter1(black_sensor_reading[2]);
                            current_executing_block->setParameter2(1);
                            cout<<"sensor: "<<black_sensor_reading[2]<<endl;
                            break;
                        case 4:
                            //black sensor 4
                            current_executing_block->setParameter1(black_sensor_reading[3]);
                            current_executing_block->setParameter2(1);
                            cout<<"sensor: "<<black_sensor_reading[3]<<endl;
                            break;
                        case 5:
                            //black sensor 5
                            current_executing_block->setParameter1(black_sensor_reading[4]);
                            current_executing_block->setParameter2(1);
                            cout<<"sensor: "<<black_sensor_reading[4]<<endl;
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
                            if((ultrasonic_sensor_reading[0] > low_limit_ultrasonic) && (ultrasonic_sensor_reading[0] <= high_limit_ultrasonic)) {
                                current_executing_block->setParameter1(ultrasonic_sensor_reading[0]);
                            } else {
                                current_executing_block->setParameter1(0);
                            }
                            current_executing_block->setParameter2(1);
                            cout<<"sensor: "<<ultrasonic_sensor_reading[0]<<endl;
                            break;
                        case 9:
                            //ultrasonic sensor 2
                            if((ultrasonic_sensor_reading[1] > low_limit_ultrasonic) && (ultrasonic_sensor_reading[1] <= high_limit_ultrasonic)) {
                                current_executing_block->setParameter1(ultrasonic_sensor_reading[1]);
                            } else {
                                current_executing_block->setParameter1(0);
                            }
                            current_executing_block->setParameter2(1);
                            cout<<"sensor lido: "<<ultrasonic_sensor_reading[1]<<endl;
                            break;
                        case 10:
                            //ultrasonic sensor 3
                            if((ultrasonic_sensor_reading[2] > low_limit_ultrasonic) && (ultrasonic_sensor_reading[2] <= high_limit_ultrasonic)) {
                                current_executing_block->setParameter1(ultrasonic_sensor_reading[2]);
                            } else {
                                current_executing_block->setParameter1(0);
                            }
                            current_executing_block->setParameter2(1);
                            cout<<"sensor: "<<ultrasonic_sensor_reading[2]<<endl;
                            break;
                    }
                }
                //se for 1 significa que terminou execução ou está pronto para receber
                if(communication->getFeedback() == READY) {
                    //testa se o próximo é não nulo
                    if(current_executing_block->getExecutingNext() != NULL) {
                        std::cout<<"executou bloco: "<<current_executing_block->getName()<<std::endl;
                        current_executing_block = current_executing_block->getExecutingNext();
                        refresh_executing_block();
                        //bloco de ação
                        if(current_executing_block->getType() == ACTION_BLOCK) {
                            communication->setCommand(current_executing_block->getCommand());
                        }
                    } else {
                        std::cout<<"executou bloco: "<<current_executing_block->getName()<<std::endl;
                        executing_fluxogram = false;
                        //reseta as variáveis execução
                        reset_fluxogram_execution();
                        reset_executing_block();
                    }
                } if(communication->getFeedback() == COLISION) {
                    gui->callMessage(19);
                    executing_fluxogram = false;
                    reset_fluxogram_execution();
                    reset_executing_block();

                }
            }
        } else {
            gui->callMessage(14);
            executing_fluxogram = false;
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
    //reseta as variáveis do loops para a execução
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                blocks_list_to_print[i]->reset_loop_variables();
            }
        }
    }
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
    int feedback = 0, error = 0;
    string address = program_path;
    #ifdef _WIN32
        #ifdef WINDOWS_USER
            address = address + "FluxProgBackend";
        #else
            address = address + "../../../FluxProgBackend/bin/Debug/FluxProgBackend";
        #endif
        STARTUPINFO si;
        PROCESS_INFORMATION p;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&p, sizeof(p));
        char *path_ = (char*)address.c_str();
        if( !CreateProcessA( NULL, path_, NULL, NULL, FALSE, 0, NULL, NULL, &si, &p))
        {
            error = 1;
        }
        Sleep(1000);
    #else
        address = address + "../../../FluxProgBackend/build/bin/FluxProgBackend &";
        error = system(address.c_str());
        sleep(1);
    #endif // _WIN32
    feedback = communication->getFeedback();
    if(feedback != 0 && error == 0) {
        //communication->upadateReadings();
        //std::cout<<"abriu o programa"<<std::endl;
        clock_t t;
        t = clock();
        while(feedback != CONNECT_ERROR && feedback != READY && (((float)(clock() - t))/CLOCKS_PER_SEC) < TIMEOUT)
        {
            feedback = communication->getFeedback();
        }
        //cout << "foram "<< ((float)(clock() - t))/CLOCKS_PER_SEC <<"segundos"<<endl;
        if(feedback == CONNECT_ERROR){
            //não abriu o v-rep ou não tem bluetooth
            //std::cout<<"não abriu o v-rep ou não tem bluetooth"<<std::endl;
            if(simulator_connected == true) {
                gui->callMessage(7);
            } else {
                gui->callMessage(6);
            }
            communication->setCommand(CLOSE_PROGRAM);
            program_connected = false;
            gui->setConnectedRobot(false);
            gui->setConnectedSimulator(false);
        } else if(feedback == READY) {
            if(simulator_connected == true) {
                gui->callMessage(8);
                gui->setConnectedSimulator(true);
                gui->setConnectedRobot(false);
            } else {
                gui->callMessage(9);
                gui->setConnectedSimulator(false);
                gui->setConnectedRobot(true);
            }
            program_connected = true;
        } else{
            if(simulator_connected == true) {
                gui->callMessage(10);
                //communication->inicialize();
            } else {
                gui->callMessage(11);
            }
            communication->setCommand(CLOSE_PROGRAM);
            program_connected = false;
            gui->setConnectedRobot(false);
            gui->setConnectedSimulator(false);
        }
    } else {
        gui->callMessage(5);
        program_connected = false;
        gui->setConnectedRobot(false);
        gui->setConnectedSimulator(false);
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
string FluxProg :: getExecutablePath() {
    char result[ PATH_MAX ];
    string path;
    #ifdef _WIN32
        ssize_t count = GetModuleFileName( NULL, result, MAX_PATH );
    #else
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    #endif // _WIN32
    if (count != -1)
    {
        path = dirname(result);
    }
    #ifdef _WIN32
        size_t pos = path.find("\\", 0);
        while(pos != string::npos)
        {
            path.replace(pos, 1, "/");
            pos = path.find("\\", pos);
        }
    #endif

    return path;
}
void FluxProg :: reset_executing_block() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            blocks_list_to_print[i]->setExecuting(false);
        }
    }
}
int FluxProg :: check_abstraction_level() {
    int level = HIGH_ABSTRACTION;
    for(int i = 0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == CONDITIONAL_BLOCK) {
                if((blocks_list_to_print[i]->getTypeOfSensor() == BLACK_SENSOR_1) || (blocks_list_to_print[i]->getTypeOfSensor() == BLACK_SENSOR_2) || (blocks_list_to_print[i]->getTypeOfSensor() == BLACK_SENSOR_3)) {
                    level = LOW_ABSTRACTION;
                    break;
                } else if((blocks_list_to_print[i]->getTypeOfSensor() == BLACK_SENSOR_4) || (blocks_list_to_print[i]->getTypeOfSensor() == BLACK_SENSOR_5)) {
                    level = LOW_ABSTRACTION;
                    break;
                } else if((blocks_list_to_print[i]->getTypeOfSensor() == ULTRASONIC_SENSOR_1) || (blocks_list_to_print[i]->getTypeOfSensor() == ULTRASONIC_SENSOR_2) || (blocks_list_to_print[i]->getTypeOfSensor() == ULTRASONIC_SENSOR_3)) {
                    level = MID_ABSTRACTION;
                }
            }
        }
    }
    return level;
}
void FluxProg :: reset_blocks_list() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            blocks_list_to_print[i] = NULL;
        }
    }
}
void FluxProg :: delete_blocks_with_no_connections_function() {
    for(int i=0; i<valor_maximo_blocos; i++) {
        if(blocks_list_to_print[i] != NULL) {
            if(blocks_list_to_print[i]->getType() == ACTION_BLOCK) {
                //function
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                } else if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                }
            } else if(blocks_list_to_print[i]->getType() == END_BLOCK) {
                //end
                if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                }
            } else if(blocks_list_to_print[i]->getType() == START_BLOCK) {
                //start
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                }
            } else if(blocks_list_to_print[i]->getType() == LOOP_BLOCK) {
                //loop
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                } else if(blocks_list_to_print[i]->getNext2() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                } else if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                } else if(blocks_list_to_print[i]->getPrevious2() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                }
            } else if(blocks_list_to_print[i]->getType() == CONDITIONAL_BLOCK) {
                //decision
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                } else if(blocks_list_to_print[i]->getNext2() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                } else if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                }
            } else if(blocks_list_to_print[i]->getType() == MERGE_BLOCK) {
                //merge
                if(blocks_list_to_print[i]->getNext1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                } else if(blocks_list_to_print[i]->getPrevious1() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                } else if(blocks_list_to_print[i]->getPrevious2() == NULL) {
                    remove_block(blocks_list_to_print[i]);
                }
            }
        }
    }
}
