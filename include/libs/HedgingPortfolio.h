#pragma once

#include "libs/InterestRate.h"
#include "pnl/pnl_vector.h"

class HedgingPortfolio{
    public:
        double val_liquidative_initiale_;
        double last_rebalancing_t_;
        double V1_; // Montant investi au taux sans risque pour la traking error
        double V2_; // Montant investi au taux sans risque pour la valeur liquidative
        PnlVect* delta_;
        InterestRate* rates_;

        HedgingPortfolio(double prix, const PnlVect * const delta, const PnlVect * const share_values, InterestRate * const rates, double val_liquidative_initiale);
        ~HedgingPortfolio();

        void rebalancing(double t, const PnlVect * const delta, const PnlVect * const share_values);
        double get_portfolio_value(double t, const PnlVect * const share_values) const;
        double get_tracking_error(double t, double prix, const PnlVect * const share_values) const;
        double get_valeur_liquidative(double t, const PnlVect * const share_values) const;
        double get_FinalPnL(double t, double payoff, const PnlVect * const share_values) const;

};