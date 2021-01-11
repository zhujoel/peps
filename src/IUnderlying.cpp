#include "IUnderlying.h"
#include <iostream>

IUnderlying::IUnderlying(double spot, double zc_spot, int nbTimeSteps){
    this->spot_ = spot;
    this->zc_spot_ = zc_spot;
    this->shifted_price_ = pnl_vect_new();
    this->zc_ = pnl_vect_create(nbTimeSteps+1);
    this->price_ = pnl_vect_create(nbTimeSteps+1);
    LET(this->zc_, 0) = this->zc_spot_;
    LET(this->price_, 0) = this->spot_;
}

IUnderlying::~IUnderlying(){
    pnl_vect_free(&this->price_);
    pnl_vect_free(&this->zc_);
    pnl_vect_free(&this->shifted_price_);
}