#include "models/IModel.h"

IModel::IModel(int size, int nbTimeSteps, InterestRate * const rates){
    this->size_ = size;
    this->rates_ = rates;
    this->nbTimeSteps_ = nbTimeSteps;
    this->volatility_ = pnl_vect_create(this->size_);
    this->sigma_ = pnl_mat_create(this->size_, this->size_);
}

IModel::~IModel(){
    pnl_vect_free(&this->volatility_);
    pnl_mat_free(&this->sigma_);
}