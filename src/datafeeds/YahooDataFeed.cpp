#include "datafeeds/YahooDataFeed.h"
#include <iostream>

YahooDataFeed::YahooDataFeed(char* filepath){
    this->filepath_ = filepath;
}
YahooDataFeed::~YahooDataFeed(){}

void YahooDataFeed::getData(DateTimeVector *dates, PnlVect *path){
    std::cout << "hello" << std::endl;
}