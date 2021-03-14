#pragma once

#include "models/IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        PnlVect *G_; /// Vecteur Gaussien
        PnlVect *B_; // Brownien (produit matriciel L * G);

        BlackScholesModel(int size, int nbTimeSteps, InterestRate * const rates, PnlVect * const computed_ti);
        ~BlackScholesModel();
        
        void asset(PnlMat * const path, double t, PnlRng * const rng, const PnlVect * const spot);
        void shift_asset(PnlMat * const shift_path, double t, const PnlMat * const path, int d, double h) const;
};