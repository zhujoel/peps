#pragma once

#include "models/IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        PnlVect *G_; /// Vecteur Gaussien
        PnlVect *B_; // Brownien (produit matriciel L * G);

        BlackScholesModel(int size, int nbTimeSteps, InterestRate * const rates);
        ~BlackScholesModel();
        
        void asset(PnlMat * const path, double t, double T, PnlRng * const rng, int startIdx, double rd, const PnlMat * const sigma);
        void shift_asset(PnlMat * const shift_path, const PnlMat * const path, int d, double h, int startIdx) const;
};