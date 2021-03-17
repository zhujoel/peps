#include "market_data/HistoricalMarketData.h"
#include "datafeeds/MarketDataFeed.h"
#include "pnl/pnl_matrix.h"
#include <cstring>

HistoricalMarketData::HistoricalMarketData(const std::string &name, const DateTime * const startDate, const DateTime * const endDate) 
    : IMarketData(name, startDate, endDate)
{
    this->derivative_path_ = pnl_mat_new();
}

HistoricalMarketData::~HistoricalMarketData(){
    pnl_mat_free(&this->derivative_path_);
}

void HistoricalMarketData::set_data()
{
    // TODO: ajouter un if qui vérifie l'attribut nom et qui parse en fonction
    this->set_Ocelia_data();
}

void HistoricalMarketData::set_Ocelia_data(){
    int size = 4;

    IDataFeed **dataFeeds = new IDataFeed*[size];

    std::string indices_file = "../data/market_data/Indices.csv";
    unsigned int indices_nb_columns = 5;
    PnlVectInt *indices_relevant_columns = pnl_vect_int_create(4);
    LET_INT(indices_relevant_columns, 0) = 1;
    LET_INT(indices_relevant_columns, 1) = 2;
    LET_INT(indices_relevant_columns, 2) = 3;
    LET_INT(indices_relevant_columns, 3) = 4;
    unsigned int indices_date_column = 0;
    dataFeeds[0] = new MarketDataFeed(indices_relevant_columns, indices_date_column, indices_nb_columns, indices_file);

    std::string tx_change_file = "../data/market_data/Taux_de_change.csv";
    unsigned int tx_change_nb_columns = 4;
    PnlVectInt *tx_change_relevant_columns = pnl_vect_int_create(3);
    LET_INT(tx_change_relevant_columns, 0) = 1;
    LET_INT(tx_change_relevant_columns, 1) = 2;
    LET_INT(tx_change_relevant_columns, 2) = 3;
    unsigned int tx_change_date_column = 0;
    dataFeeds[1] = new MarketDataFeed(tx_change_relevant_columns, tx_change_date_column, tx_change_nb_columns, tx_change_file);

    std::string interest_file = "../data/market_data/Interest_rates.csv";
    unsigned int interest_nb_columns = 5;
    PnlVectInt *interest_relevant_columns = pnl_vect_int_create(4);
    LET_INT(interest_relevant_columns, 0) = 1;
    LET_INT(interest_relevant_columns, 1) = 2;
    LET_INT(interest_relevant_columns, 2) = 3;
    LET_INT(interest_relevant_columns, 3) = 4;
    unsigned int interest_date_column = 0;
    dataFeeds[2] = new MarketDataFeed(interest_relevant_columns, interest_date_column, interest_nb_columns, interest_file);

    std::string ocelia_file = "../data/market_data/Valeur_Liquidative_Ocelia.csv";
    unsigned int ocelia_nb_columns = 2;
    PnlVectInt *ocelia_relevant_columns = pnl_vect_int_create(1);
    LET_INT(ocelia_relevant_columns, 0) = 1;
    unsigned int ocelia_date_column = 0;
    dataFeeds[3] = new MarketDataFeed(ocelia_relevant_columns, ocelia_date_column, ocelia_nb_columns, ocelia_file);

    for(int i = 0; i < size; ++i){
        dataFeeds[i]->set_data();
    }


    from_date_to_date(this->dates_, dataFeeds[0]->dates_, this->startDate_, this->endDate_);
    
    std::vector<DateTime*> tmp_input;
    for(int i = 1; i < size; ++i){
        tmp_input = this->dates_;
        this->dates_ = same_dates(tmp_input, dataFeeds[i]->dates_);
    }
    delete_date_vector(tmp_input);

    PnlMat *indices = pnl_mat_new();
    PnlMat *tx_change = pnl_mat_new();

    get_prices_from_date(indices, dataFeeds[0]->dates_, this->dates_, dataFeeds[0]->prices_);
    get_prices_from_date(tx_change, dataFeeds[1]->dates_, this->dates_, dataFeeds[1]->prices_);
    get_prices_from_date(this->interest_path_, dataFeeds[2]->dates_, this->dates_, dataFeeds[2]->prices_);
    get_prices_from_date(this->derivative_path_, dataFeeds[3]->dates_, this->dates_, dataFeeds[3]->prices_);

    pnl_mat_resize(this->path_, indices->m, indices->n + tx_change->n);
    pnl_mat_set_subblock(this->path_, indices, 0, 0);
    pnl_mat_set_subblock(this->path_, tx_change, 0, indices->n);

    for(int i = 0; i < size; ++i){
        delete dataFeeds[i];
    }
    delete[] dataFeeds;
    pnl_mat_free(&indices);
    pnl_mat_free(&tx_change);
    pnl_vect_int_free(&indices_relevant_columns);
    pnl_vect_int_free(&tx_change_relevant_columns);
    pnl_vect_int_free(&ocelia_relevant_columns);
    pnl_vect_int_free(&interest_relevant_columns);
}