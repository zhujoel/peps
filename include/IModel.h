#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "IDerivative.h"

class IModel{
    public:
        IDerivative *derivative_; // pointeur sur le produit dérivé qu'on calcule
        PnlMat *sigma_; /// Matrice de volatilité

        IModel(IDerivative *derivative, PnlMat *sigma);
        IModel();
        ~IModel();
        
        virtual void asset(PnlRng *rng) = 0;
        virtual void asset(PnlMat *path, PnlMat *sigma, PnlVect *volatility, double rd, double T, double nbTimeSteps, PnlVect* spot, PnlRng *rng) = 0;

        virtual void shiftAsset(double h, double t, double timestep) = 0;
        double getUnderlyingSpot(int d);
        void compute_sigma();
};

// couverture en tx stochastique :
// un frwd pour un ss-jacent
// un zc domestique pour tous les ss-jct + ss-jacent <-
// actif ss risque étranger + zc domestique