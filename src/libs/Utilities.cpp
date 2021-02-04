#include "libs/Utilities.h"
#include <fstream>
#include <sstream>
#include <string>

void split(std::string line, char delimiter, std::string *output){
    std::stringstream string_stream(line);
    int i = 0;
    while(string_stream.good())   // loop will continue if string stream is error free
    {
        getline(string_stream, output[i++], delimiter );
    }
}