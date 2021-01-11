#include "BlackScholesModel.h"
#include <iostream>

BlackScholesModel::BlackScholesModel(IDerivative *derivative, PnlVect *rd, PnlMat *sigma, double nbTimeSteps, PnlRng *rng) 
    : IModel(derivative, rd, sigma, nbTimeSteps, rng)
{
}

BlackScholesModel::~BlackScholesModel()
{
}

void BlackScholesModel::asset(PnlVect *path, int ind)
{
    // TODO: enlever le ind et mettre sigma en vecteur à la place
    // TODO: mettre taux r en attribut ?
    double timestep = this->derivative_->T_/this->nbTimeSteps_;

    double size = 2;
    PnlVect *G = pnl_vect_create(size); 
    PnlVect *B = pnl_vect_create(size);    
    PnlVect *row = pnl_vect_create(size);

    for (int k = 1; k <= this->nbTimeSteps_; ++k)
    {
        pnl_vect_rng_normal(G, size, this->rng_);
        pnl_mat_mult_vect_inplace(B, this->sigma_, G);
        pnl_mat_get_row(row, this->sigma_, ind);
        double sigma_d = pnl_vect_norm_two(row);

        LET(path, k) = GET(path, k-1) * exp( (GET(this->rd_, k) - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(B, ind));
    }
}

void BlackScholesModel::price_all(){

    for(int i  = 0; i < this->derivative_->size_; ++i){
        this->derivative_->underlyings_[i]->price_ = pnl_vect_create(this->nbTimeSteps_+1);
        LET(this->derivative_->underlyings_[i]->price_, 0) = this->derivative_->underlyings_[i]->spot_;
        asset(this->derivative_->underlyings_[i]->price_, (i*2));
        this->derivative_->underlyings_[i]->zc_ = pnl_vect_create(this->nbTimeSteps_+1);
        LET(this->derivative_->underlyings_[i]->zc_, 0) = this->derivative_->underlyings_[i]->zc_spot_;
        asset(this->derivative_->underlyings_[i]->zc_, (i*2)+1);
    }

}