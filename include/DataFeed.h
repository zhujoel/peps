#pragma once
#include <string>
#include <map>
#include "DataTime.h"

using namespace std;


class DataFeed{
    
    public:
    DataTime date_;
    map<string, double> listePrix_;
    DataFeed(DataTime date, map<string, double> listePrix);
    
    ~DataFeed();

};
