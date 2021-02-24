// #include "market_data/HistoricalMarketData.h"
// #include <iostream>
// #include "datafeeds/YahooDataFeed.h"
// #include "datafeeds/FTSEDataFeed.h"
// #include "pnl/pnl_matrix.h"

// HistoricalMarketData::HistoricalMarketData(std::string name, DateTime *startDate, DateTime *endDate) 
//     : IMarketData(name, startDate, endDate)
// {
// }

// HistoricalMarketData::~HistoricalMarketData(){}

// PnlMat* HistoricalMarketData::getData(std::vector<DateTime*> *used_dates){
//     YahooDataFeed *ftse = new YahooDataFeed("../data/market_data/FTSE80.csv");
//     ftse->getData();
    
//     YahooDataFeed *ssmi = new YahooDataFeed("../data/market_data/SSMI.csv");
//     ssmi->getData();

//     YahooDataFeed *n225 = new YahooDataFeed("../data/market_data/N225.csv");
//     n225->getData();
    
//     YahooDataFeed *n100 = new YahooDataFeed("../data/market_data/N100.csv");
//     n100->getData();

//     YahooDataFeed *chf_eur = new YahooDataFeed("../data/market_data/CHF-EUR.csv");
//     chf_eur->getData();

//     YahooDataFeed *gbp_eur = new YahooDataFeed("../data/market_data/GBP-EUR.csv");
//     gbp_eur->getData();

//     YahooDataFeed *jpy_eur = new YahooDataFeed("../data/market_data/JPY-EUR.csv");
//     jpy_eur->getData();

//     for(int i to 7){
//         new YahooDataFeed();
//         yahoo->getData();
//         sameDates()
//     }

//     DateTimeVector *dateResult = new DateTimeVector(0);
//     sameDates(datesFTSE, datesSSMI, dateResult);
//     sameDates(datesN225, dateResult, dateResult);
//     sameDates(datesN100, dateResult, dateResult);
//     sameDates(datesCHF, dateResult, dateResult);
//     sameDates(datesGBP, dateResult, dateResult);
//     sameDates(datesJPY, dateResult, dateResult);

//     fromDateToDate(dateResult, this->startDate_, this->endDate_, used_dates);

//     PnlVect *pricesFTSEFiltered = pnl_vect_new();
//     PnlVect *pricesSSMIFiltered = pnl_vect_new();
//     PnlVect *pricesN225Filted = pnl_vect_new();
//     PnlVect *pricesN100Filtered = pnl_vect_new();
//     PnlVect *pricesGBPFiltered = pnl_vect_new();
//     PnlVect *pricesCHFFiltered = pnl_vect_new();
//     PnlVect *pricesJPYFiltered = pnl_vect_new();
//     getPricesFromDate(datesFTSE, dateResult, pricesFTSE, pricesFTSEFiltered);
//     getPricesFromDate(datesSSMI, dateResult, pricesSSMI, pricesSSMIFiltered);
//     getPricesFromDate(datesN225, dateResult, pricesN225, pricesN225Filted);
//     getPricesFromDate(datesN100, dateResult, pricesN100, pricesN100Filtered);
//     getPricesFromDate(datesGBP, dateResult, pricesGBP, pricesGBPFiltered);
//     getPricesFromDate(datesCHF, dateResult, pricesCHF, pricesCHFFiltered);
//     getPricesFromDate(datesJPY, dateResult, pricesJPY, pricesJPYFiltered);

//     PnlMat *path = pnl_mat_create(dateResult->nbDates_, 7);

//     pnl_mat_set_col(path, pricesN100Filtered, 0);
//     pnl_mat_set_col(path, pricesN225Filted, 1);
//     pnl_mat_set_col(path, pricesSSMIFiltered, 2);
//     pnl_mat_set_col(path, pricesFTSEFiltered, 3);
//     pnl_mat_set_col(path, pricesGBPFiltered, 4);
//     pnl_mat_set_col(path, pricesCHFFiltered, 5);
//     pnl_mat_set_col(path, pricesJPYFiltered, 6);

//     // pnl_mat_print(path);

//     return path;

// }

