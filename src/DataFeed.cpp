#include "DataFeed.h"

DataFeed::DataFeed(DataTime date, map<string, double> listePrix){
    this->date_ = date;
    this->listePrix_ = listePrix;

}

DataFeed::~DataFeed(){}

