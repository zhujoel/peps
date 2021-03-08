#include "market_data/IMarketData.h"

IMarketData::IMarketData(const std::string &name, const DateTime * const startDate, const DateTime * const endDate){
    this->name_ = name;
    this->startDate_ = new DateTime(startDate);
    this->endDate_ = new DateTime(endDate);
    this->path_ = pnl_mat_new();
    this->interest_path_ = pnl_mat_new();
}

IMarketData::~IMarketData(){
    delete this->startDate_;
    delete this->endDate_;
    pnl_mat_free(&this->path_);
    pnl_mat_free(&this->interest_path_);
    delete_date_vector(this->dates_);
}