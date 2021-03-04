#include "market_data/HistoricalMarketData.h"
#include "datafeeds/YahooDataFeed.h"
#include "pnl/pnl_matrix.h"
#include <cstring>

HistoricalMarketData::HistoricalMarketData(std::string name, DateTime *startDate, DateTime *endDate) 
    : IMarketData(name, startDate, endDate)
{
}

void HistoricalMarketData::get_data()
{
    // ajouter un if qui vérifie l'attribut nom et qui parse en fonction
    this->get_Ocelia_data();
}

void HistoricalMarketData::get_Ocelia_data(){
    int size = 7;
    std::string files[] = {
        "../data/market_data/N100.csv",
        "../data/market_data/N225.csv",
        "../data/market_data/SSMI.csv",
        "../data/market_data/FTSE80.csv",
        "../data/market_data/GBP-EUR.csv",
        "../data/market_data/JPY-EUR.csv",
        "../data/market_data/CHF-EUR.csv"
    };
    IDataFeed *dataFeeds[size];

    for(int i = 0; i < size; ++i){
        dataFeeds[i] = new YahooDataFeed(files[i]);
        dataFeeds[i]->get_data();
    }


    from_date_to_date(this->dates_, dataFeeds[0]->dates_, this->startDate_, this->endDate_);
    
    for(int i = 1; i < size; ++i){
        std::vector<DateTime*> input(this->dates_);
        std::vector<DateTime*> output;
        same_dates(output, input, dataFeeds[i]->dates_);
        this->dates_ = output;
    }

    pnl_mat_resize(this->path_, this->dates_.size(), size);

    for(int i = 0; i < size; ++i){
        PnlVect *prices = pnl_vect_new();
        get_prices_from_date(prices, dataFeeds[i]->dates_, this->dates_, dataFeeds[i]->prices_);
        pnl_mat_set_col(this->path_, prices, i);
        pnl_vect_free(&prices);
    }

    for(int i = 0; i < size; ++i){
        delete dataFeeds[i];
    }
}