#pragma once

#include "libs/DateTimeVector.h"
#include "pnl/pnl_vector.h"
#include <string>

class IDataFeed{
    public:
        std::string filepath_;
        DateTimeVector *dates_;
        PnlVect *prices_;

        IDataFeed(std::string filepath);
        ~IDataFeed();
        
        virtual int getNumberValidData() = 0;
        virtual void getData() = 0;
        //virtual void parseAndOutput() = 0; TODO: remove perhaps: no need for now
};