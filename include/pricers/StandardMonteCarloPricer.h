#pragma once

#include "pricers/IPricer.h"

class StandardMonteCarloPricer : public IPricer{
    public:
        StandardMonteCarloPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples);
        ~StandardMonteCarloPricer();

        void price_and_delta(const PnlMat *past, double t, const PnlMat *sigma, double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev);
        void price(const PnlMat *past, double t, const PnlMat *sigma, double &prix, double &price_std_dev);
        void add_price(double t, double &prix, double &std_dev);
        void add_delta(double t, int pastSize, PnlVect *delta, PnlVect *std_dev);
};