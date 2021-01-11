#pragma once

#include "pnl/pnl_vector.h"

/**
 * @brief Underlying of a derivative.
 */
class IUnderlying {

  public:
    PnlVect *price_;
    double spot_;
    PnlVect *zc_;
    double zc_spot_;
    PnlVect *ex_rate_;
    PnlVect *rf_;

    IUnderlying(double spot, double zc_spot);
    ~IUnderlying();
};