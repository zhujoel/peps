#pragma once

#include "libs/InterestRate.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

/**
 * @brief Modèle Black-Scholes.
 * 
 */
class IModel{
    public:
        /**
         * @brief Nombre d'actifs du modèle.
         * 
         */
        int size_;
        /**
         * @brief Nombre total de jours ouvrés entre 0 et la fin du produit. (NE CORRESPOND PAS AU NOMBRE DE PAS SIMULÉS)
         * 
         */
        int nbTimeSteps_;
        /**
         * @brief Taux d'intérêt utilisés.
         * 
         */
        InterestRate* rates_;
        /**
         * @brief Vecteur de tous les t précalculés qui nous intéressent. 
         * 
         */
        PnlVect *computed_ti_;
        /**
         * @brief Volatilité du modèle.
         * 
         */
        PnlVect *volatility_;
        /**
         * @brief Sigma du modèle.
         * 
         */
        PnlMat *sigma_;

        /**
         * @brief Construct a new IModel object
         * 
         * @param size 
         * @param nbTimeSteps 
         * @param rates 
         * @param computed_ti 
         */
        IModel(int size, int nbTimeSteps, InterestRate * const rates, PnlVect * const computed_ti);
        /**
         * @brief Destroy the IModel object
         * 
         */
        virtual ~IModel();
        /**
         * @brief Simule la trajectoire.
         * 
         * @param path[out] Trajectoire simulé.
         * @param t Date t à partir duquelle on simule. Utilise computed_ti. Ne commence donc pas forcément à l'indice 0.
         * @param rng Moteur RNG à utiliser pour simuler les valeurs.
         * @param spot Valeurs par lesquelles on commence la simulation.
         */
        virtual void asset(PnlMat * const path, double t, PnlRng * const rng, const PnlVect * const spot) = 0;
        /**
         * @brief Shift d'une trajectoire du sous-jacent.
         * 
         * @param shift_path[out] Path shifté.
         * @param t Date t à partir duquelle on shift.
         * @param path Path qu'on shift.
         * @param d Indice du sous-jacent à shifter.
         * @param h Pas de différences finies.
         */
        virtual void shift_asset(PnlMat * const shift_path, double t, const PnlMat * const path, int d, double h) const = 0;
};