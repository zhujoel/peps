#include "BlackScholesModel.h"
#include <iostream>

BlackScholesModel::BlackScholesModel(IDerivative *derivative, PnlMat *sigma) : IModel(derivative, sigma)
{
    this->G_ = pnl_vect_create(this->derivative_->size_); 
    this->B_ = pnl_vect_create(this->derivative_->size_);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&this->G_);
    pnl_vect_free(&this->B_);
}

void BlackScholesModel::asset(PnlRng *rng)
{
    double timestep = this->derivative_->T_/this->derivative_->nbTimeSteps_;
    this->derivative_->underlyings_[0]->zc_ = pnl_vect_create(this->derivative_->nbTimeSteps_+1);
    this->derivative_->underlyings_[0]->price_ = pnl_vect_create(this->derivative_->nbTimeSteps_+1);
    LET(this->derivative_->underlyings_[0]->zc_, 0) = this->derivative_->underlyings_[0]->zc_spot_;
    LET(this->derivative_->underlyings_[0]->price_, 0) = this->derivative_->underlyings_[0]->spot_;
    PnlVect *row = pnl_vect_create(this->derivative_->size_);
    for (int k = 1; k <= this->derivative_->nbTimeSteps_; ++k)
    {
        pnl_vect_rng_normal(this->G_, this->derivative_->size_, rng); // G Vecteur gaussien
        pnl_mat_mult_vect_inplace(this->B_, this->sigma_, this->G_);

        pnl_mat_get_row(row, this->sigma_, 0);
        double sigma_d = pnl_vect_norm_two(row);
        LET(this->derivative_->underlyings_[0]->zc_, k) = GET(this->derivative_->underlyings_[0]->zc_, k-1) * exp( (this->derivative_->rd_ - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, 0));

        pnl_mat_get_row(row, this->sigma_, 1);
        sigma_d = pnl_vect_norm_two(row);
        LET(this->derivative_->underlyings_[0]->price_, k) = GET(this->derivative_->underlyings_[0]->price_, k-1) * exp( (this->derivative_->rd_ - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, 1));
    }
    
    pnl_vect_free(&row);
}

void BlackScholesModel::shiftAsset(double h, double t, double timestep)
{
    int i = t/timestep;
    // if (abs( (i+1)*timestep - t)<1E-5)
    // {
    //     i+=1;
    // }
    if (h>0)
    {
        pnl_vect_clone(this->derivative_->underlyings_[0]->shifted_price_, this->derivative_->underlyings_[0]->price_);
        for (int k = i+1; k <= this->derivative_->nbTimeSteps_; ++k)
        {  
            LET(this->derivative_->underlyings_[0]->shifted_price_, k) = GET(this->derivative_->underlyings_[0]->shifted_price_, k) * (1+h);
        }

    } else {
        for (int k = i+1; k <= this->derivative_->nbTimeSteps_; ++k)
        {  
            LET(this->derivative_->underlyings_[0]->shifted_price_, k) = (GET(this->derivative_->underlyings_[0]->shifted_price_, k) / (1-h)) * (1+h);
        }
    }
}

double BlackScholesModel::getSpot(int d){
    if(d % 2 == 0){
        return this->derivative_->underlyings_[d/2]->zc_spot_;
    }
    return this->derivative_->underlyings_[d/2]->spot_;
}
