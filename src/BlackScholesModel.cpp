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
    PnlVect *row = pnl_vect_create(this->derivative_->size_);
    for(int d = 0; d < this->derivative_->size_/2; ++d){
        for (int k = 1; k <= this->derivative_->nbTimeSteps_; ++k)
        {
            pnl_vect_rng_normal(this->G_, this->derivative_->size_, rng); // G Vecteur gaussien
            pnl_mat_mult_vect_inplace(this->B_, this->sigma_, this->G_);

            pnl_mat_get_row(row, this->sigma_, d*2);
            double sigma_d = pnl_vect_norm_two(row);
            LET(this->derivative_->underlyings_[d]->zc_, k) = GET(this->derivative_->underlyings_[d]->zc_, k-1) * exp( (this->derivative_->rd_ - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, d*2));

            pnl_mat_get_row(row, this->sigma_, (d*2)+1);
            sigma_d = pnl_vect_norm_two(row);
            LET(this->derivative_->underlyings_[d]->price_, k) = GET(this->derivative_->underlyings_[d]->price_, k-1) * exp( (this->derivative_->rd_ - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, (d*2)+1));
        }
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


    for(int d = 0; d < this->derivative_->size_/2; ++d){
        if (h>0)
        {
            pnl_vect_clone(this->derivative_->underlyings_[d]->shifted_price_, this->derivative_->underlyings_[d]->price_);
            for (int k = i+1; k <= this->derivative_->nbTimeSteps_; ++k)
            {  
                LET(this->derivative_->underlyings_[d]->shifted_price_, k) = GET(this->derivative_->underlyings_[d]->shifted_price_, k) * (1+h);
            }

        } else {
            for (int k = i+1; k <= this->derivative_->nbTimeSteps_; ++k)
            {  
                LET(this->derivative_->underlyings_[d]->shifted_price_, k) = (GET(this->derivative_->underlyings_[d]->shifted_price_, k) / (1-h)) * (1+h);
            }
        }
    }
}
