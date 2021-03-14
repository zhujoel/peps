#include "libs/Utilities.h"
#include <sstream>
#include <string>
#include <math.h>
#include <iostream>

void split(std::string * const output, const std::string &line, char delimiter){
    std::stringstream string_stream(line);
    int i = 0;
    while(string_stream.good())   // loop will continue if string stream is error free
    {
        getline(string_stream, output[i++], delimiter );
    }
}

void trunc(PnlVect * const vect, int n){
    int ten = pow(10, n);
    for(int i = 0; i < vect->size; ++i){
        LET(vect, i) = roundl(GET(vect, i)*ten)/ten;
    }
}

bool are_all_positive(const PnlVect * const vect){
    return pnl_vect_min(vect) >= 0;
}

bool has_no_index_value_null(const std::string * const array, const PnlVectInt * const indexes){
    for(int i = 0; i < indexes->size; ++i){
        if(array[GET_INT(indexes, i)] == "null"){
            return false;
        }
    }
    return true;
}

int first_index_gte(const PnlVect * const vect, double value){
    for(int i = 0; i < vect->size; ++i){
        if(GET(vect, i) >= value) return i;
    }
    return vect->size;
}

std::ostream& operator<<(std::ostream &stm, const PnlVect * const vect)
{
    int length = vect->size;
    if(length == 0) return stm;
    for (int i = 0; i < length - 1; i++)
    {
        stm << GET(vect, i) << " ";
    }
    stm << GET(vect, length - 1);
    return stm;
}