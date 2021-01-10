#pragma once
#include <string>
#include <map>
#include "DataTime.h"
#include "DataFeed.h"
using namespace std;

class IMarketData{
    public:
    DateTime * listeDate_;

    IMarketData(DateTime * listeDate);

    virtual ~IMarketData();
    virtual DataFeed * getMarketdata();

};