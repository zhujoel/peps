#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

class IModel{
    public:
        int size_; /// nombre d'actifs du modèle
        double rd_; /// taux d'intérêt domestique (domestic rate) // TODO: à changer si on a besoin qu'il ne soit pas constant 
        PnlMat *sigma_;
        PnlVect *volatility_;
        PnlVect *spot_; /// valeurs initiales des sous-jacents

        IModel(int size, double rd, PnlMat *sigma, PnlVect *volatility, PnlVect *spot);
        virtual ~IModel();
        virtual void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) = 0;
        virtual void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep) = 0;
};