#pragma once

#include "models/IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        PnlVect *G_; /// Vecteur Gaussien
        PnlVect *B_; // Brownien (produit matriciel L * G);

        BlackScholesModel(int size, double rd, PnlMat *past);
        ~BlackScholesModel();
        
        void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);
        void shift_asset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);
};