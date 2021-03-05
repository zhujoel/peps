#pragma once

#include "datafeeds/IDataFeed.h"

class YahooDataFeed : public IDataFeed{
    public:
        YahooDataFeed(const std::string &filepath);
        
        int get_number_valid_data() const;
        void set_data();
};