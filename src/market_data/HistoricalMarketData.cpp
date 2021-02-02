#include "market_data/HistoricalMarketData.h"

HistoricalMarketData::HistoricalMarketData(char* filepath, char* name, DateTime *startDate, DateTime *endDate) 
    : IMarketData(name, startDate, endDate)
{
    this->filepath_ = filepath;
}

HistoricalMarketData::~HistoricalMarketData(){}

