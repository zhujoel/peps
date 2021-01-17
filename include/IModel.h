#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "IDerivative.h"

class IModel{
    public:
        IDerivative *derivative_; // pointeur sur le produit dérivé qu'on calcule
        PnlMat *sigma_; /// Matrice de volatilité


        IModel(IDerivative *derivative, PnlMat *sigma);
        ~IModel();
        
        virtual void asset(PnlRng *rng) = 0;
        virtual void shiftAsset(double h, double t, double timestep) = 0;
        double getUnderlyingSpot(int d);
        void compute_sigma();
};