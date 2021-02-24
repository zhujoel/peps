#include "datafeeds/IDataFeed.h"

IDataFeed::IDataFeed(std::string filepath){
    this->filepath_ = filepath;
    this->prices_ = pnl_vect_new();
}
IDataFeed::~IDataFeed()
{
    delete_date_vector(this->dates_);
    pnl_vect_free(&this->prices_);
}
