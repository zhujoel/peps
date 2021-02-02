#include "datafeeds/YahooDataFeed.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

YahooDataFeed::YahooDataFeed(char* filepath){
    this->filepath_ = filepath;
}
YahooDataFeed::~YahooDataFeed(){}

void YahooDataFeed::getData(DateTimeVector *dates, PnlVect *path){
    std::ifstream fin;
    std::string line;
    std::string answer[7];
    fin.open(this->filepath_);
    while(!fin.eof()){
        fin >> line;
        std::stringstream string_stream(line);  // creating string stream object
        int i=0;            // declaring i and assign  to 0
        while(string_stream.good())   // loop will continue if string stream is error free
        {
            std::string a;       
            getline( string_stream, a, ',' );   //calling getline fuction
            answer[i]=a;
            i++;
            
            std::cout << answer[0] << " ";
            if(answer[4] == "") std::cout << "null" << std::endl;
            else std::cout << answer[4] << std::endl;
        }
        // std::cout << line << " ";
    }
}