#pragma once

#include "libs/InterestRate.h"
#include "pnl/pnl_vector.h"

/**
 * @brief Gère les valeurs calculées d'un portefeuille.
 * 
 */
class HedgingPortfolio{
    public:
        /**
         * @brief Valeur liquidative initiale du portefeuille.
         * 
         */
        double val_liquidative_initiale_;
        /**
         * @brief Date t de dernier rebalancement.
         * 
         */
        double last_rebalancing_t_;
        /**
         * @brief Montant investi au taux sans risque pour la tracking error.
         * 
         */
        double V1_;
        /**
         * @brief Montant investi au taux sans risque pour la valeur liquidative.
         * 
         */
        double V2_; 
        /**
         * @brief Delta calculé.
         * 
         */
        PnlVect* delta_;
        /**
         * @brief Taux d'intérêt utilisé.
         * 
         */
        InterestRate* rates_;

        /**
         * @brief Construct a new Hedging Portfolio object
         * 
         * @throws std::runtime_error Si la marge initiale est négative.
         * @param prix Prix du produit. Détermine la marge initiale du portefeuille à partir du prix et de la valeur liquidative initiale. 
         * @param delta Delta initiale du produit.
         * @param share_values Valeur des sous-jacents. Utilisé pour calculer les V1 et V2 initiales.
         * @param rates 
         * @param val_liquidative_initiale 
         */
        HedgingPortfolio(double prix, const PnlVect * const delta, const PnlVect * const share_values, InterestRate * const rates, double val_liquidative_initiale);
        /**
         * @brief Destroy the Hedging Portfolio object
         * 
         */
        ~HedgingPortfolio();

        /**
         * @brief Rebalance le portefeuille en t.
         * 
         * @param t Date t à laquelle on souhaite calculer.
         * @param delta Delta en t.
         * @param share_values Valeurs des sous-jacents en t.
         */
        void rebalancing(double t, const PnlVect * const delta, const PnlVect * const share_values);
        /**
         * @brief Calcule la valeur du portefeuille en t.
         * 
         * @param t Date t à laquelle on souhaite calculer.
         * @param share_values Valeurs des sous-jacents en t.
         * @return double Valeur du portefeuille en t.
         */
        double get_portfolio_value(double t, const PnlVect * const share_values) const;

        /**
         * @brief Calcule la tracking error du portefeuille en t.
         * 
         * @param t Date t à laquelle on souhaite calculer.
         * @param prix Prix en t.
         * @param share_values Valeurs des sous-jacents en t.
         * @return double Tracking error en t.
         */
        double get_tracking_error(double t, double prix, const PnlVect * const share_values) const;
        /**
         * @brief Calcule la valeur liquidative du portefeuille en t.
         * 
         * @param t Date t à laquelle on souhaite calculer.
         * @param share_values Valeurs des sous-jacents en t.
         * @return double Valeur liquidative du portefeuille en t.
         */
        double get_valeur_liquidative(double t, const PnlVect * const share_values) const;
        /**
         * @brief Calcule le PnL en t.
         * 
         * @param t Date t à laquelle on souhaite calculer.
         * @param payoff Payoff du produit dérivé.
         * @param share_values Valeurs des sous-jacents en t.
         * @return double PnL du portefeuille en t.
         */
        double get_FinalPnL(double t, double payoff, const PnlVect * const share_values) const;

};