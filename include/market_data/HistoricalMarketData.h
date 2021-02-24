#pragma once

#include "market_data/IMarketData.h"

class HistoricalMarketData : public IMarketData{
    public:
        HistoricalMarketData(std::string name, DateTime *startDate, DateTime *endDate);

        void getData();
        void getOceliaData();
};