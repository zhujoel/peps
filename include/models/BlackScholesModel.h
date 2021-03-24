#pragma once

#include "models/IModel.h"
#include "pnl/pnl_vector.h"

/**
 * @brief Modèle Black-Scholes.
 * 
 */
class BlackScholesModel : public IModel{
    public:
        /**
         * @brief Stocke le vecteur Gaussien pour simuler B-S.
         * 
         */
        PnlVect *G_;
        /**
         * @brief Stocke le brownien pour simuler B-S (correspond au produit matriciel L*G).
         * 
         */
        PnlVect *B_;

        /**
         * @brief Construct a new Black Scholes Model object
         * 
         * @param size 
         * @param nbTimeSteps 
         * @param rates 
         * @param computed_ti 
         */
        BlackScholesModel(int size, int nbTimeSteps, InterestRate * const rates, PnlVect * const computed_ti);
        /**
         * @brief Destroy the Black Scholes Model object
         * 
         */
        ~BlackScholesModel();
        
        void asset(PnlMat * const path, double t, PnlRng * const rng, const PnlVect * const spot);
        void shift_asset(PnlMat * const shift_path, double t, const PnlMat * const path, int d, double h) const;
};