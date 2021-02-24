#include "market_data/HistoricalMarketData.h"
#include <iostream>
#include "datafeeds/YahooDataFeed.h"
#include "pnl/pnl_matrix.h"

HistoricalMarketData::HistoricalMarketData(std::string name, DateTime *startDate, DateTime *endDate) 
    : IMarketData(name, startDate, endDate)
{
}

HistoricalMarketData::~HistoricalMarketData(){}

void HistoricalMarketData::getData()
{
    this->getOceliaData();
}

void HistoricalMarketData::getOceliaData(){
    int size = 7;
    std::string files[] = {
        "../data/market_data/FTSE80.csv",
        "../data/market_data/SSMI.csv",
        "../data/market_data/N225.csv",
        "../data/market_data/N100.csv",
        "../data/market_data/CHF-EUR.csv",
        "../data/market_data/GBP-EUR.csv",
        "../data/market_data/JPY-EUR.csv"
    };
    IDataFeed *dataFeeds[size];

    for(int i = 0; i < size; ++i){
        dataFeeds[i] = new YahooDataFeed(files[i]);
        dataFeeds[i]->getData();
    }

    this->dates_ = dataFeeds[0]->dates_;
    this->dates_ = fromDateToDate(this->dates_, this->startDate_, this->endDate_);
    
    for(int i = 1; i < size; ++i){
        this->dates_ = sameDates(this->dates_, dataFeeds[i]->dates_);
    }

    this->path_ = pnl_mat_create(this->dates_.size(), 7);

    for(int i = 0; i < size; ++i){
        PnlVect *prices = getPricesFromDate(dataFeeds[i]->dates_, this->dates_, dataFeeds[i]->prices_);
        pnl_mat_set_col(this->path_, prices, i);
    }
}