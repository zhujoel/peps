#pragma once

#include "market_data/IMarketData.h"

class HistoricalMarketData : public IMarketData{
    public:
        PnlMat *derivative_path_;

        HistoricalMarketData(const std::string &name, const DateTime * const startDate, const DateTime * const endDate);
        ~HistoricalMarketData();
        
        void set_data();
        void set_Ocelia_data();
};