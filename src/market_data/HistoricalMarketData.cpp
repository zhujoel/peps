#include "market_data/HistoricalMarketData.h"
#include "datafeeds/MarketDataFeed.h"
#include "pnl/pnl_matrix.h"
#include <cstring>

HistoricalMarketData::HistoricalMarketData(const std::string &name, const DateTime *startDate, const DateTime *endDate) 
    : IMarketData(name, startDate, endDate)
{
}

void HistoricalMarketData::set_data()
{
    // ajouter un if qui vérifie l'attribut nom et qui parse en fonction
    this->set_Ocelia_data();
}

void HistoricalMarketData::set_Ocelia_data(){
    int nb_files = 2;
    PnlMat *mkt_data = pnl_mat_create(0, 7);
    PnlMat *mkt_rate = pnl_mat_create(0, 4);
    PnlMat *ocelia_real = pnl_mat_create(0, 1);

    std::string files[] = {
        "../data/market_data/Indices.csv",
        "../data/market_data/Taux_de_change.csv",
        "../data/market_data/Interest_rates.csv",
        "../data/market_data/Valeur_Liquidative_Ocelia.csv",
    };
}