#pragma once

#include "libs/DateTimeVector.h"
#include "pnl/pnl_vector.h"
#include <string>
#include <vector>

class IDataFeed{
    public:
        std::string filepath_;
        std::vector<DateTime*> dates_;
        PnlVect *prices_;

        IDataFeed(std::string filepath);
        virtual ~IDataFeed();
        
        virtual int get_number_valid_data() = 0; // ignores null-value prices
        virtual void get_data() = 0;
};