#include "datafeeds/IDataFeed.h"

IDataFeed::IDataFeed(std::string filepath){
    this->filepath_ = filepath;
    this->prices_ = pnl_vect_new();
}
IDataFeed::~IDataFeed(){}
