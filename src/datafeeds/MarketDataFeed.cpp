#include "datafeeds/MarketDataFeed.h"
#include "libs/Utilities.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>

MarketDataFeed::MarketDataFeed(PnlVectInt *relevant_columns, unsigned int date_column, unsigned int nb_columns, const std::string &filepath)
    : IDataFeed(relevant_columns, date_column, nb_columns, filepath) {}

int MarketDataFeed::get_number_valid_data() const
{
    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[this->nb_columns_];
    int count = 0;

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(parsedLine, line, ',');
        if(has_no_index_value_null(parsedLine, this->relevant_columns_)) ++count;
    }

    dataFile.close();
    return count;
}

void MarketDataFeed::set_data()
{
    int nbDates = this->get_number_valid_data();
    pnl_mat_resize(this->prices_, nbDates, this->relevant_columns_->size);

    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[this->nb_columns_];
    int count = 0;

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(parsedLine, line, ',');
        if(has_no_index_value_null(parsedLine, this->relevant_columns_)){
            this->dates_.push_back(parse_date_string(parsedLine[this->date_column_], '-'));
            for(int i = 0; i < this->relevant_columns_->size; ++i){
                MLET(this->prices_, count, i) = std::atof(parsedLine[GET_INT(this->relevant_columns_, i)].c_str()); // c_str(): needs a char* to use atof
            }
            count++;
        }
    }

    dataFile.close();
}