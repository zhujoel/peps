#include "IModel.h"

IModel::IModel(IDerivative *derivative, PnlMat *sigma){
    this->derivative_ = derivative;
    this->sigma_ = sigma;
}

IModel::IModel(){

}

IModel::~IModel(){
}


double IModel::getUnderlyingSpot(int d){
    if(d % 2 == 0){
        return this->derivative_->underlyings_[d/2]->zc_spot_;
    }
    return this->derivative_->underlyings_[d/2]->spot_;
}