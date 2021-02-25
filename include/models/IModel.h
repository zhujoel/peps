#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

class IModel{
    public:
        int size_; /// nombre d'actifs du modèle
        double rd_; /// taux d'intérêt domestique (domestic rate)
        PnlMat *sigma_;
        PnlVect *volatility_;
        PnlMat *past_; /// valeurs de marché passées

        IModel(int size, double rd, PnlMat *past);
        virtual ~IModel();
        virtual void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) = 0;
        virtual void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep) = 0;
};