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
    double zc_spot_;

    /**
     * @brief Trajectory of the exchange rate of the currency of the market with another currency.
     */
    PnlVect *ex_rate_;
    /**
     * @brief Trajectory of the interest rate of the market.
     * 
     */
    PnlVect *r_;
};