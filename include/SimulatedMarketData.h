#pragma once
#include "IMarketData.h"

class SimulatedMarketData: public IMarketData{
    public:
        SimulatedMarketData(DateTime ** listeDate, int nombreDate);

        ~SimulatedMarketData();
        IUnderlying ** getMarketdata(int nbUnderlying);
};