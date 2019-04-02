#include "LoadFile.hpp"

LoadFile :: LoadFile() {

}
LoadFile :: ~LoadFile() {

}
void LoadFile :: load() {
    in.open("Nome_do_Arquivo.fluxpg");
    std::string line;
    getline(in, line); //started the process
    getline(in, line);
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
            getline(in, line);
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
            getline(in, line);
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
            getline(in, line);
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
            getline(in, line);
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
            getline(in, line);
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
            aux->setName("bloco loop");
            getline(in, line);
            aux->setValue(atoi(line.c_str()));
            getline(in, line);
        }
    }
    in.close(); // nã oesqueça de fechar...
}
