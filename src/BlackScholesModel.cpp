#include "BlackScholesModel.h"
#include <iostream>

BlackScholesModel::BlackScholesModel(int size, double rd, PnlMat *sigma, PnlVect *spot) :
IModel(size, rd, sigma, spot)
{
    this->G_ = pnl_vect_create(this->size_); 
    this->B_ = pnl_vect_create(this->size_);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&this->spot_);
    pnl_mat_free(&this->sigma_);
    pnl_vect_free(&this->G_);
    pnl_vect_free(&this->B_);
}

void BlackScholesModel::setTrend(PnlVect *trend){
    this->trend_ = trend;
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
{
    double timestep = T/nbTimeSteps;
    pnl_mat_set_row(path, this->spot_, 0);

    for (int k = 1; k <= nbTimeSteps; ++k)
    {
        pnl_vect_rng_normal(this->G_, this->size_, rng); // G Vecteur gaussien
        pnl_mat_mult_vect_inplace(this->B_, this->sigma_, this->G_);
        for (int d = 0; d < this->size_; ++d)
        {
            PnlVect *row = pnl_vect_create(this->size_);
            pnl_mat_get_row(row, this->sigma_, d);
            double sigma_d = pnl_vect_norm_two(row);
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
