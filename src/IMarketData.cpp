#include "IMarketData.h"

IMarketData::IMarketData(char* name, DateTime *startDate, DateTime *endDate){
    this->name_ = name;
    this->startDate_ = startDate;
    this->endDate_ = endDate;
}

IMarketData::~IMarketData(){}

