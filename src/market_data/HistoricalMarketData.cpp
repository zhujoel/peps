#include "market_data/HistoricalMarketData.h"
#include <iostream>
#include "datafeeds/YahooDataFeed.h"
#include "pnl/pnl_matrix.h"
#include <cstring>

HistoricalMarketData::HistoricalMarketData(std::string name, DateTime *startDate, DateTime *endDate) 
    : IMarketData(name, startDate, endDate)
{
}

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

    this->dates_ = fromDateToDate(dataFeeds[0]->dates_, this->startDate_, this->endDate_);
    
    for(int i = 1; i < size; ++i){
        this->dates_ = sameDates(this->dates_, dataFeeds[i]->dates_);
    }

    pnl_mat_resize(this->path_, this->dates_.size(), size);

    for(int i = 0; i < size; ++i){
        PnlVect *prices = getPricesFromDate(dataFeeds[i]->dates_, this->dates_, dataFeeds[i]->prices_);
        pnl_mat_set_col(this->path_, prices, i);
        pnl_vect_free(&prices);
    }

    for(int i = 0; i < size; ++i){
        delete dataFeeds[i];
    }
}