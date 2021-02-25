#include "models/IModel.h"
#include "libs/MathLib.h"

IModel::IModel(int size, double rd, PnlMat *past){
    this->size_ = size;
    this->rd_ = rd;
    this->past_ = past;
    this->sigma_ = compute_sigma(this->past_, 0, this->past_->m);
    this->volatility_ = compute_volatility(this->sigma_);
}

IModel::~IModel(){
    pnl_mat_free(&this->sigma_);
    pnl_vect_free(&this->volatility_);
}