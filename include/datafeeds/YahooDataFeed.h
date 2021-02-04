#pragma once

#include "datafeeds/IDataFeed.h"
#include <string>

class YahooDataFeed : public IDataFeed{
    public:
        char* filepath_;

        YahooDataFeed(char* filepath);
        ~YahooDataFeed();
        
        int getNumberValidData();
        void getData(DateTimeVector *dates, PnlVect *path);
};