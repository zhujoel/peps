#pragma once

#include "IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        // on l'avait pour l'espace mémoire -- TODO: à enlever si pas nécessaire
        PnlVect *G_; /// Vecteur Gaussien
        PnlVect *B_; // Brownien (produit matriciel L * G);

        BlackScholesModel(int size, double rd, PnlMat *sigma, PnlVect *spot);
        ~BlackScholesModel();
        
        void asset(QuantoOption *derivative, double T, int nbTimeSteps, PnlRng *rng);

        void shiftAsset(QuantoOption *derivative, int d, double h, double t, double timestep);

};