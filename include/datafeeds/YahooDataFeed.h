#pragma once

#include "datafeeds/IDataFeed.h"

class YahooDataFeed : public IDataFeed{
    public:
        YahooDataFeed(std::string filepath);
        ~YahooDataFeed();
        
        int getNumberValidData();
        void getData();
};