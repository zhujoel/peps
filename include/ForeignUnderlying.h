#pragma once

#include "IUnderlying.h"
#include "Market.h"

/**
 * @brief Underlying from a foreign market.
 */
class ForeignUnderlying : public IUnderlying {

  public:
    /**
     * @brief Market where the underlying resides.
     * 
     */
    Market *mkt_;

    ForeignUnderlying(double und_spot, double zc_spot);
    ~ForeignUnderlying();
    
    void simulate_price(IModel *model, double T, int nbTimeSteps, PnlRng *rng);

};