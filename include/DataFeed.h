#pragma once
#include <string>
#include <map>
#include "DataTime.h"

using namespace std;


class Datafeed{
    
    public:
    DataTime date_;
    map<string, double> listePrix_;
    Datafeed(DataTime date, map<string, double> listePrix);
    
    ~Datafeed();

};
