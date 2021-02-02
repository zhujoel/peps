#pragma once

#include "market_data/IMarketData.h"

class HistoricalMarketData : public IMarketData{
    public:
        char* filepath_; // path to the file to parse for the data

        HistoricalMarketData(char* filepath, char* name, DateTime *startDate, DateTime *endDate);
        ~HistoricalMarketData();

};