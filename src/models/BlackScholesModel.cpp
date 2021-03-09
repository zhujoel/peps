#include "models/BlackScholesModel.h"
#include "libs/MathLib.h"

BlackScholesModel::BlackScholesModel(int size, int nbTimeSteps, InterestRate * const rates) : IModel(size, nbTimeSteps, rates)
{
    this->G_ = pnl_vect_create(this->size_); 
    this->B_ = pnl_vect_create(this->size_);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&this->G_);
    pnl_vect_free(&this->B_);
}

void BlackScholesModel::asset(PnlMat * const path, double t, double T, PnlRng * const rng, double rd, int startIndex)
{
    // path c'est 4 sous-jacent + 3 zc
    // path, size: 4 + 3 (actif sans risque étrangers en domestique)
    // 0: ss-jct gbp
    // 1: ss-jct jpy
    // 2: ss-jct chf
    // 3: ss-jct euro
    // 4: zc gbp
    // 5: zc jpy
    // 6: zc chf
    // TODO: ya peut etre une erreur sur timestep ?
    double timestep = T/this->nbTimeSteps_;

    for (int k = startIndex; k < path->m; ++k)
    {
        pnl_vect_rng_normal(this->G_, this->size_, rng); // G Vecteur gaussien
        pnl_mat_mult_vect_inplace(this->B_, this->sigma_, this->G_);
        for (int d = 0; d < this->size_; ++d)
        {
            double sigma_d = GET(this->volatility_, d);
            MLET(path, k, d) = MGET(path, k-1, d) * exp( (rd - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, d));
        }
    }
}

void BlackScholesModel::shift_asset(PnlMat * const shift_path, const PnlMat * const path, int d, double h, int startIndex) const
{
    if (h>0)
    {
        pnl_mat_clone(shift_path, path);
        for (int k = startIndex; k < path->m; ++k)
        {  
            MLET(shift_path, k, d) = MGET(shift_path, k, d) * (1+h);
        }
    } else {
        for (int k = startIndex; k < path->m; ++k)
        {  
            MLET(shift_path, k, d) = (MGET(shift_path, k, d) / (1-h)) * (1+h);
        }
    }
}