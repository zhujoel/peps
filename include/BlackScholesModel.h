#pragma once

#include "IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        // on l'avait pour l'espace mémoire -- TODO: à enlever si pas nécessaire
        PnlVect *G_; /// Vecteur Gaussien
        PnlVect *B_; // Brownien (produit matriciel L * G);

        BlackScholesModel(IDerivative *derivative, PnlMat *sigma);
        ~BlackScholesModel();
        
        void asset(PnlRng *rng);

        void shiftAsset(double h, double t, double timestep);

        double getSpot(int d);

};