#include "models/IModel.h"

IModel::IModel(int size, int nbTimeSteps, InterestRate * const rates, PnlVect * const computed_ti){
    this->size_ = size;
    this->nbTimeSteps_ = nbTimeSteps;
    this->rates_ = rates;
    this->computed_ti_ = computed_ti;
    this->volatility_ = pnl_vect_create(this->size_);
    this->sigma_ = pnl_mat_create(this->size_, this->size_);
}

IModel::~IModel(){
    pnl_vect_free(&this->volatility_);
    pnl_mat_free(&this->sigma_);
}