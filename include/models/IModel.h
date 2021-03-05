#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

class IModel{
    public:
        int size_; /// nombre d'actifs du modèle
        double rd_; /// taux d'intérêt domestique (domestic rate)

        IModel(int size, double rd);
        virtual ~IModel();
        virtual void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, const PnlMat *sigma) = 0;
        virtual void shift_asset(PnlMat *shift_path, const PnlMat *path, int d, double h, int startIndex) const = 0;
};