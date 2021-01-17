#pragma once
#include <string>
#include <map>
#include "DataTime.h"
// #include "DataFeed.h"
#include "IUnderlying.h"
using namespace std;

class IMarketData{
    public:
    DateTime ** listeDate_;

    IMarketData(DateTime ** listeDate);

    virtual ~IMarketData();
    virtual IUnderlying ** getMarketdata();

};