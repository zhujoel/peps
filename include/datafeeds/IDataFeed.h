#pragma once

#include "libs/DateTimeVector.h"
#include "pnl/pnl_vector.h"
#include <string>

class IDataFeed{
    public:
        std::string filepath_;

        IDataFeed(std::string filepath);
        ~IDataFeed();
        
        virtual int getNumberValidData() = 0;
        virtual void getData(DateTimeVector *dates, PnlVect *path) = 0;
        virtual void parseAndOutput() = 0;
};