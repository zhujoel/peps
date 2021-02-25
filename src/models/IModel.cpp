#include "models/IModel.h"

IModel::IModel(int size, double rd, PnlMat *sigma, PnlVect *volatility, PnlMat *past){
    this->size_ = size;
    this->rd_ = rd;
    this->sigma_ = sigma;
    this->volatility_ = volatility;
    this->past_ = past;
}

IModel::~IModel(){
}