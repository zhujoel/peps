#pragma once

#include "IPricer.h"

/**
 * Monte-Carlo standard -- TODO: avec l'idée qu'on puisse aussi implémenter les méthodes + efficaces vues en cours de MonteCarlo 
 */
class StandardMonteCarloPricer : public IPricer{
    public:
        StandardMonteCarloPricer(IModel *model, PnlRng *rng, double fdStep, int nbSamples);
        
        ~StandardMonteCarloPricer();

        void simulate(double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev);

        void price(double &prix, double &std_dev);
        void delta(PnlVect *delta, PnlVect *std_dev);

        void discount_price(double t, double &prix, double &std_dev);
        void discount_delta(double t, PnlVect *delta, PnlVect *std_dev);
};