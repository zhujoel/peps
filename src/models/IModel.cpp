#include "models/IModel.h"

IModel::IModel(int size, int nbTimeSteps, InterestRate* rates){
    this->size_ = size;
    this->rates_ = rates;
    this->nbTimeSteps_ = nbTimeSteps;
}

IModel::~IModel(){
}