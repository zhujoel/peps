#include "IModel.h"

IModel::IModel(IDerivative *derivative, int size, double rd, PnlMat *sigma, PnlVect *spot){
    this->derivative_ = derivative;
    this->size_ = size;
    this->rd_ = rd;
    this->sigma_ = sigma;
    this->spot_ = spot;
}

IModel::~IModel(){
}