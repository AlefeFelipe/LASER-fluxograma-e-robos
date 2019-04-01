#ifndef SAVEFILE_HPP_
#define SAVEFILE_HPP_

#include <fstream>

using namespace std;


class SaveFile {

    ofstream out;

public:

    SaveFile();
    ~SaveFile();
    void save();


};
#endif
