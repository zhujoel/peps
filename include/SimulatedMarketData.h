#pragma once
#include "IMarketData.h"

class SimulatedMarketData: public IMarketData{
    public:
        SimulatedMarketData(DateTime ** listeDate);

        ~SimulatedMarketData();
        IUnderlying ** getMarketdata(int nbUnderlying);
}