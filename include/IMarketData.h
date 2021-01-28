#pragma once
#include <string>
#include <map>
#include "DateTimeVector.h"
// #include "DataFeed.h"
#include "IUnderlying.h"

class IMarketData{
    public:
        DateTimeVector *dates_;
        IMarketData(DateTimeVector *dates);

        virtual ~IMarketData();
        virtual IUnderlying **getMarketdata(int nbUnderlying) = 0;

};