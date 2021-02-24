#pragma once

#include "datafeeds/IDataFeed.h"

class YahooDataFeed : public IDataFeed{
    public:
        YahooDataFeed(std::string filepath);
        
        int getNumberValidData();
        void getData();
};