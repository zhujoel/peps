#pragma once

#include "pricers/IPricer.h"

/**
 * @brief Pricer Monte-Carlo Standard.
 * 
 */
class StandardMonteCarloPricer : public IPricer{
    public:
        /**
         * @brief Construct a new Standard Monte Carlo Pricer object
         * 
         * @param model 
         * @param derivative 
         * @param rng 
         * @param fdStep 
         * @param nbSamples 
         */
        StandardMonteCarloPricer(IModel * const model, IDerivative * const derivative, PnlRng * const rng, double fdStep, int nbSamples);
        /**
         * @brief Destroy the Standard Monte Carlo Pricer object
         * 
         */
        ~StandardMonteCarloPricer();

        void price_and_delta(const PnlVect * const spot, double t, double &prix, double &price_std_dev, PnlVect * const delta, PnlVect * const delta_std_dev);
        void price(const PnlVect * const spot, double t, double &prix, double &price_std_dev);
        void add_price(double t, double &prix, double &std_dev);
        void add_delta(double t, PnlVect * const delta, PnlVect * const std_dev);
};