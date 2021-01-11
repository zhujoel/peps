#pragma once

#include "IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        BlackScholesModel(IDerivative *derivative, PnlVect *rd, PnlMat *sigma, double nbTimeSteps, PnlRng *rng);
        ~BlackScholesModel();
        
        void asset(PnlVect *path, int ind);
        void price_all();
};