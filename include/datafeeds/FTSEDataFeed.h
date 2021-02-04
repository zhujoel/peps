#pragma once

#include "datafeeds/IDataFeed.h"

class FTSEDataFeed : public IDataFeed{
    public:
        FTSEDataFeed(std::string filepath);
        ~FTSEDataFeed();
        
        int getNumberValidData();
        void getData(DateTimeVector *dates, PnlVect *path);
        void parseAndOutput();
};