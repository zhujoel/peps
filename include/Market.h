#pragma once

/**
 * @brief Represents the financial data from the market of a specific currency.
 */
class Market {

  public:
    /**
     * @brief Price trajectory of the zero-coupon of the market.
     */
    PnlVect *zc_;
    /**
     * @brief Trajectory of the exchange rate of the currency of the market with another currency.
     */
    PnlVect *ex_rate_;    
};