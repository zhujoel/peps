#include "IModel.h"

IModel::IModel(IDerivative *derivative, PnlMat *sigma){
    this->derivative_ = derivative;
    this->sigma_ = sigma;
}

IModel::~IModel(){
}