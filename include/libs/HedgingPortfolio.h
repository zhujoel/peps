#pragma once

#include "pnl/pnl_vector.h"

class HedgingPortfolio{
    public:
        double val_liquidative_initiale_;
        double last_rebalancing_t_;
        double rd_; // TODO pointeur vers les taux ??
        double V1_; // Montant investi au taux sans risque pour la traking error
        double V2_; // Montant investi au taux sans risque pour la valeur liquidative
        PnlVect* delta_;

        HedgingPortfolio(double prix, PnlVect* delta, PnlVect* share_values, double rd, double val_liquidative_initiale);
        ~HedgingPortfolio();

        void rebalancing(double t, PnlVect* delta, PnlVect* share_values);
        double get_portfolio_value(double t, PnlVect* share_values);
        double get_tracking_error(double t, double prix, PnlVect* share_values);
        double get_valeur_liquidative(double t, PnlVect* share_values);
        double get_FinalPnL(double t, double payoff, PnlVect* share_values);

};