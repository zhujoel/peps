#pragma once

#include "datafeeds/IDataFeed.h"
#include <string>

class YahooDataFeed : public IDataFeed{
    public:
        std::string filepath_;

        YahooDataFeed(std::string filepath);
        ~YahooDataFeed();
        
        int getNumberValidData();
        void getData(DateTimeVector *dates, PnlVect *path);
        void parseAndOutput();

};