#pragma once

#include "market_data/IMarketData.h"

class HistoricalMarketData : public IMarketData{
    public:
        HistoricalMarketData(const std::string &name, const DateTime *startDate, const DateTime *endDate);

        void set_data();
        void set_Ocelia_data();
};