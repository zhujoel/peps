#include "datafeeds/IDataFeed.h"
#include "libs/DateTimeVector.h"

IDataFeed::IDataFeed(PnlVectInt *relevant_columns, unsigned int date_column, unsigned int nb_columns, const std::string &filepath){
    this->relevant_columns_ = relevant_columns;
    this->date_column_ = date_column;
    this->nb_columns_ = nb_columns;
    this->filepath_ = filepath;
    this->prices_ = pnl_mat_new();
}

IDataFeed::~IDataFeed()
{
    delete_date_vector(this->dates_);
    pnl_mat_free(&this->prices_);
}