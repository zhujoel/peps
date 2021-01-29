#pragma once
#include "IMarketData.h"

class SimulatedMarketData: public IMarketData{
    public:
        SimulatedMarketData(DateTimeVector *dates);

        ~SimulatedMarketData();
        // IUnderlying **getMarketdata(int nbUnderlying);
};