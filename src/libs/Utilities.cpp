#include "libs/Utilities.h"
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

void split(std::string *output, const std::string &line, char delimiter){
    std::stringstream string_stream(line);
    int i = 0;
    while(string_stream.good())   // loop will continue if string stream is error free
    {
        getline(string_stream, output[i++], delimiter );
    }
}

void trunc(PnlVect* vect, int n){
    int ten = pow(10, n);
    for(int i = 0; i < vect->size; ++i){
        LET(vect, i) = roundl(GET(vect, i)*ten)/ten;
    }
}

bool are_all_positive(const PnlVect *vect){
    return pnl_vect_min(vect) >= 0;
}