#include "BlackScholesModel.h"
#include <iostream>

BlackScholesModel::BlackScholesModel(int size, double rd, PnlMat *sigma, PnlVect *volatility, PnlVect *spot) :
IModel(size, rd, sigma, volatility, spot)
{
    this->G_ = pnl_vect_create(this->size_); 
    this->B_ = pnl_vect_create(this->size_);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&this->G_);
    pnl_vect_free(&this->B_);
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
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
    double timestep = T/nbTimeSteps;
    pnl_mat_set_row(path, this->spot_, 0);

    for (int k = 1; k <= nbTimeSteps; ++k)
    {
        pnl_vect_rng_normal(this->G_, this->size_, rng); // G Vecteur gaussien
        pnl_mat_mult_vect_inplace(this->B_, this->sigma_, this->G_);
        for (int d = 0; d < this->size_; ++d)
        {
            double sigma_d = GET(this->volatility_, d);
            MLET(path, k, d) = MGET(path, k-1, d) * exp( (this->rd_ - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, d));
        }
    }
}

void BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep)
{
    int i = t/timestep;
    // if (abs( (i+1)*timestep - t)<1E-5)
    // {
    //     i+=1;
    // }
    if (h>0)
    {
        pnl_mat_clone(shift_path, path);
        for (int k = i+1; k < path->m; ++k)
        {  
            MLET(shift_path, k, d) = MGET(shift_path, k, d) * (1+h);
        }
    } else {
        for (int k = i+1; k < path->m; ++k)
        {  
            MLET(shift_path, k, d) = (MGET(shift_path, k, d) / (1-h)) * (1+h);
        }
    }
    
}
