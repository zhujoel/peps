#include "market_data/IMarketData.h"

IMarketData::IMarketData(std::string name, DateTime *startDate, DateTime *endDate){
    this->name_ = name;
    this->startDate_ = startDate;
    this->endDate_ = endDate;
    this->path_ = pnl_mat_new();
}

IMarketData::~IMarketData(){
    delete this->startDate_;
    delete this->endDate_;
    pnl_mat_free(&this->path_);
    delete_date_vector(this->dates_);
}

