#pragma once

#include "libs/DateTimeVector.h"
#include "pnl/pnl_vector.h"

class IDataFeed{
    public:
        IDataFeed();
        ~IDataFeed();
        
        virtual int getNumberValidData() = 0;
        virtual void getData(DateTimeVector *dates, PnlVect *path) = 0;
        virtual void parseAndOutput() = 0;
};