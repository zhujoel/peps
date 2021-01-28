#include "BlackScholesModel.h"
#include <iostream>


BlackScholesModel::BlackScholesModel(IDerivative *derivative, PnlMat *sigma) : IModel(derivative, sigma)
{
    this->G_ = pnl_vect_create(this->derivative_->size_*2); 
    this->B_ = pnl_vect_create(this->derivative_->size_*2);
}

BlackScholesModel::BlackScholesModel(){

    this->G_ = pnl_vect_create(7); 
    this->B_ = pnl_vect_create(7);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&this->G_);
    pnl_vect_free(&this->B_);
}

void BlackScholesModel::asset(PnlRng *rng)
{
    // TODO: works for derivative of size 1, make this work with derivative with d underlyings (instead of [0] everywhere)
    double timestep = this->derivative_->T_/this->derivative_->nbTimeSteps_;
    PnlVect *row = pnl_vect_create(this->derivative_->size_*2);
    for (int k = 1; k < this->derivative_->nbTimeSteps_; ++k)
    {
        // for(int d ; 0 < this->derivative->size )
        pnl_vect_rng_normal(this->G_, this->derivative_->size_*2, rng); // G Vecteur gaussien
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

void BlackScholesModel::asset(PnlMat *path, PnlMat *sigma, PnlVect *volatility, double rd, double T, double nbTimeSteps, PnlVect* spot, PnlRng *rng){
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
    pnl_mat_set_row(path, spot, 0);

    for (int k = 1; k <= nbTimeSteps; ++k)
    {
        pnl_vect_rng_normal(this->G_, 7, rng); // G Vecteur gaussien
        pnl_mat_mult_vect_inplace(this->B_, sigma, this->G_);
        for (int d = 0; d < 7; ++d)
        {
            double sigma_d = GET(volatility, d);
            MLET(path, k, d) = MGET(path, k-1, d) * exp( (rd - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, d));
        }
    }
}

// à taux constant
// todo: récup donneés de marchés
// todo: calculer volatility and sigma
// todo: bs asset
// todo: modifier ocelia / mc pour prendre path
// todo: tester volatility and sigma
// todo: tracking error
// todo: rd en vecteur et calcul intégrale
// todo: pricing en t

void BlackScholesModel::shiftAsset(double h, double t, double timestep)
{
    int i = t/timestep;
    // if (abs( (i+1)*timestep - t)<1E-5)
    // {
    //     i+=1;
    // }


    for(int d = 0; d < this->derivative_->size_; ++d){
        if (h>0)
        {
            pnl_vect_clone(this->derivative_->underlyings_[d]->shifted_price_, this->derivative_->underlyings_[d]->price_);
            for (int k = i+1; k < this->derivative_->nbTimeSteps_; ++k)
            {  
                LET(this->derivative_->underlyings_[d]->shifted_price_, k) = GET(this->derivative_->underlyings_[d]->shifted_price_, k) * (1+h);
            }

        } else {
            for (int k = i+1; k < this->derivative_->nbTimeSteps_; ++k)
            {  
                LET(this->derivative_->underlyings_[d]->shifted_price_, k) = (GET(this->derivative_->underlyings_[d]->shifted_price_, k) / (1-h)) * (1+h);
            }
        }
    }
}

void BlackScholesModel::simulateMarket(int nbUnderlying){
    for(int i=0; i<this->derivative_->nbTimeSteps_; i++){
        for(int j=0; j<nbUnderlying; j++){
            pnl_vect_set(this->derivative_->underlyings_[j]->price_, i, 100);
        }
    }
}

void BlackScholesModel::compute_sigma(){
    
}