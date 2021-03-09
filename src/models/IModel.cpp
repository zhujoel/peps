#include "models/IModel.h"

IModel::IModel(int size, int nbTimeSteps, InterestRate * const rates){
    this->size_ = size;
    this->rates_ = rates;
    this->nbTimeSteps_ = nbTimeSteps;
    this->volatility_ = pnl_vect_create(this->size_);
}

IModel::~IModel(){
    pnl_vect_free(&this->volatility_);
}