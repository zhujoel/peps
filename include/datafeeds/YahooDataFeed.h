#pragma once

#include "datafeeds/IDataFeed.h"

class YahooDataFeed : public IDataFeed{
    public:
        char* filepath_;

        YahooDataFeed(char* filepath);
        ~YahooDataFeed();
        
        void getData(DateTimeVector *dates, PnlVect *path);
};