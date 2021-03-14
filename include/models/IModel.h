#pragma once

#include "libs/InterestRate.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

class IModel{
    public:
        int size_; /// nombre d'actifs du modèle
        int nbTimeSteps_;
        InterestRate* rates_;
        PnlVect *computed_ti_;
        PnlVect *volatility_;
        PnlMat *sigma_;

        IModel(int size, int nbTimeSteps, InterestRate * const rates, PnlVect * const computed_ti);
        virtual ~IModel();
        virtual void asset(PnlMat * const path, double t, PnlRng * const rng, const PnlVect * const spot) = 0;
        virtual void shift_asset(PnlMat * const shift_path, double t, const PnlMat * const path, int d, double h) const = 0;
};