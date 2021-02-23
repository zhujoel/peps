#pragma once

#include "market_data/IMarketData.h"

class HistoricalMarketData : public IMarketData{
    public:
        std::string* name_; // path to the file to parse for the data

        HistoricalMarketData(std::string name, DateTime *startDate, DateTime *endDate);
        ~HistoricalMarketData();

        PnlMat* getData(std::vector<DateTime*> *used_dates);
};