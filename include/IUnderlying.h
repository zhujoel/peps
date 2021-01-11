#pragma once

#include "pnl/pnl_vector.h"

class IUnderlying {

  public:
    PnlVect *price_;
    PnlVect *shifted_price_;
    double spot_;

    PnlVect *zc_;
    double zc_spot_;
    
    PnlVect *ex_rate_;
    PnlVect *rf_;

    IUnderlying(double spot, double zc_spot, int nbTimeSteps);
    ~IUnderlying();
};