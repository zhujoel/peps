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
};