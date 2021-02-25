#pragma once

#include "datafeeds/IDataFeed.h"

class YahooDataFeed : public IDataFeed{
    public:
        YahooDataFeed(std::string filepath);
        
        int get_number_valid_data();
        void get_data();
};