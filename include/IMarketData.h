#pragma once
#include <string>
#include <map>
#include "DateTime.h"
// #include "DataFeed.h"
#include "IUnderlying.h"
using namespace std;

class IMarketData{
    public:
        DateTime ** listeDate_;
        int nombreDate_;

        IMarketData(DateTime ** listeDate, int nombreDate);

        virtual ~IMarketData();
        virtual IUnderlying ** getMarketdata(int nbUnderlying) = 0;

};