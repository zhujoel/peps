#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_matrix.h"
#include <string>
#include <vector>

class IDataFeed{
    public:
        std::string filepath_;
        std::vector<DateTime*> dates_;
        PnlMat *prices_;

        PnlVectInt *relevant_columns_;
        unsigned int date_column_;
        unsigned int nb_columns_;

        IDataFeed(PnlVectInt * const relevant_columns, unsigned int date_column, unsigned int nb_columns, const std::string &filepath);
        virtual ~IDataFeed();
        
        virtual int get_number_valid_data() const = 0; // ignores null-value prices
        virtual void set_data() = 0;
};