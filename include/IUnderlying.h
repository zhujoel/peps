#pragma once

#include "IModel.h"

/**
 * @brief Underlying of a derivative.
 */
class IUnderlying {

  public:
    /**
     * @brief Price trajectory of the underlying.
     */
    PnlVect *price_;
    
    /**
     * @brief Initial price of the underlying.
     */
    double spot_;


    IUnderlying(double spot);
    ~IUnderlying();
    
    virtual void simulate_price(IModel *model, double T, int nbTimeSteps, PnlRng *rng, PnlMat *sigma, double r, int ind) = 0;
};