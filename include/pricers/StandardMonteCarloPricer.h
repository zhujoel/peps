#pragma once

#include "pricers/IPricer.h"

class StandardMonteCarloPricer : public IPricer{
    public:
        StandardMonteCarloPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples);
        ~StandardMonteCarloPricer();

        void simulate(const PnlMat *past, double t, const PnlMat *sigma, double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev);
        void price(double t, double &prix, double &std_dev);
        void delta(double t, PnlVect *delta, PnlVect *std_dev);

        void discount_price(double t, double &prix, double &std_dev);
        void discount_delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *std_dev);
};