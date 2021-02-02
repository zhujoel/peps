#pragma once

#include "libs/DateTimeVector.h"
#include "pnl/pnl_vector.h"

class IDataFeed{
    public:
        IDataFeed();
        ~IDataFeed();

        virtual void getData(DateTimeVector *dates, PnlVect *path) = 0;
};