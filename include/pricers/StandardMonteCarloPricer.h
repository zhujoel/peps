#pragma once

#include "pricers/IPricer.h"

class StandardMonteCarloPricer : public IPricer{
    public:
        StandardMonteCarloPricer(IModel * const model, IDerivative * const derivative, PnlRng * const rng, double fdStep, int nbSamples);
        ~StandardMonteCarloPricer();

        void price_and_delta(const PnlMat * const past, double t, double &prix, double &price_std_dev, PnlVect * const delta, PnlVect * const delta_std_dev);
        void price(const PnlMat * const past, double t, double &prix, double &price_std_dev);
        void add_price(double t, double &prix, double &std_dev);
        void add_delta(double t, int pastSize, PnlVect * const delta, PnlVect * const std_dev);
};