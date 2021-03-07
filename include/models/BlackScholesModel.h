#pragma once

#include "models/IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        PnlVect *G_; /// Vecteur Gaussien
        PnlVect *B_; // Brownien (produit matriciel L * G);

        BlackScholesModel(int size, int nbTimeSteps, double rd);
        ~BlackScholesModel();
        
        void asset(PnlMat *path, double t, double T, PnlRng *rng, const PnlMat *past, const PnlMat *sigma);
        void shift_asset(PnlMat *shift_path, const PnlMat *path, int d, double h, int startIdx) const;
};