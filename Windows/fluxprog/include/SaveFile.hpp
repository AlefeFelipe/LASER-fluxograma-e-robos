#ifndef SAVEFILE_HPP_
#define SAVEFILE_HPP_

#include <fstream>
#include "fluxProgCte.h"
#include "Block.hpp"

using namespace std;


class SaveFile {

    ofstream out;

public:

    SaveFile();
    ~SaveFile();
    void save(Block** _blocks_list_to_print, char* path);


};
#endif
