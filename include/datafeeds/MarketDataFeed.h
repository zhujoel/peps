#pragma once

#include "datafeeds/IDataFeed.h"

// parse les fichiers de données du prof
class MarketDataFeed : public IDataFeed{
    public:
        MarketDataFeed(PnlVectInt * const relevant_columns, unsigned int date_column, unsigned int nb_columns, const std::string &filepath);
        
        int get_number_valid_data() const;
        void set_data();
};