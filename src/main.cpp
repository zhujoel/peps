#include <iostream>
#include "datafeeds/YahooDataFeed.h"
#include "pnl/pnl_vector.h"
#include "libs/DateTimeVector.h"


int main(){
    std::string ftse_filepath = "../data/market_data/SSMI.csv";
    YahooDataFeed *ftse = new YahooDataFeed(ftse_filepath);
    PnlVect *pricesFTSE = pnl_vect_new();
    DateTimeVector *datesFTSE = new DateTimeVector(0);
    ftse->getData(datesFTSE, pricesFTSE);

    DateTimeVector *dates_constatations = parseDatesFile("../data/dates/all_dates_constatation.csv", 49, '-');

    DateTimeVector *res = new DateTimeVector(0);
    sameDates(dates_constatations, datesFTSE, res);

    std::cout << res->nbDates_ << std::endl;
}