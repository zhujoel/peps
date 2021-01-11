#include "IModel.h"

IModel::IModel(IDerivative *derivative, PnlVect *rd, PnlMat *sigma, double nbTimeSteps, PnlRng *rng){
    this->derivative_ = derivative;
    this->rd_ = rd;
    this->sigma_ = sigma;
    this->nbTimeSteps_ = nbTimeSteps;
    this->rng_ = rng;
}

IModel::~IModel(){
}