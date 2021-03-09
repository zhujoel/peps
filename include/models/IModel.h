#pragma once

#include "libs/InterestRate.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

class IModel{
    public:
        int size_; /// nombre d'actifs du modèle
        int nbTimeSteps_;
        InterestRate* rates_;
        PnlVect *volatility_;
        PnlMat *sigma_;

        IModel(int size, int nbTimeSteps, InterestRate * const rates);
        virtual ~IModel();
        virtual void asset(PnlMat * const path, double t, double T, PnlRng * const rng, double rd, int startIdx) = 0;
        virtual void shift_asset(PnlMat * const shift_path, const PnlMat * const path, int d, double h, int startIndex) const = 0;
};