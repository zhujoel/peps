#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"
#include <string>
#include <vector>

class IDataFeed{
    public:
        std::string filepath_;
        std::vector<DateTime*> dates_;
        PnlVect *prices_;

        IDataFeed(const std::string &filepath);
        virtual ~IDataFeed();
        
        virtual int get_number_valid_data() const = 0; // ignores null-value prices
        virtual void set_data() = 0;
};