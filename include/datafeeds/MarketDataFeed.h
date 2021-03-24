#pragma once

#include "datafeeds/IDataFeed.h"

/**
 * @brief Parseur des fichiers de données de marché de O. TARAMASCO.
 */
class MarketDataFeed : public IDataFeed{
    public:
        /**
         * @brief Construct a new Market Data Feed object
         */
        MarketDataFeed(PnlVectInt * const relevant_columns, unsigned int date_column, unsigned int nb_columns, const std::string &filepath);
        
        int get_number_valid_data() const;
        void set_data();
};