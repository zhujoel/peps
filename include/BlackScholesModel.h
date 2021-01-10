#pragma once

#include "IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        BlackScholesModel();
        ~BlackScholesModel();
        
        void asset(PnlVect *path, double T, int nbTimeSteps, PnlRng *rng, PnlMat *sigma, double spot, double r, int size, int ind);
};