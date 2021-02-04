#include "datafeeds/IDataFeed.h"

IDataFeed::IDataFeed(std::string filepath){
    this->filepath_ = filepath;
}
IDataFeed::~IDataFeed(){}
