#include "datafeeds/IDataFeed.h"

IDataFeed::IDataFeed(std::string filepath){
    this->filepath_ = filepath;
    this->dates_ = new DateTimeVector(0);
    this->prices_ = pnl_vect_new();
}
IDataFeed::~IDataFeed(){}
