#include "datafeeds/YahooDataFeed.h"
#include <iostream>
#include <fstream>
#include <string>

YahooDataFeed::YahooDataFeed(char* filepath){
    this->filepath_ = filepath;
}
YahooDataFeed::~YahooDataFeed(){}

void YahooDataFeed::getData(DateTimeVector *dates, PnlVect *path){
    std::ifstream fin;
    std::string line;
    fin.open(this->filepath_);
     while(!fin.eof()){
                fin>>line;
            std::cout<<line<<" ";
            }
}