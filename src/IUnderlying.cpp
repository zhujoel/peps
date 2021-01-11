#include "IUnderlying.h"

IUnderlying::IUnderlying(double spot, double zc_spot){
    this->spot_ = spot;
    this->zc_spot_ = zc_spot;
    this->shifted_price_ = pnl_vect_new();
}

IUnderlying::~IUnderlying(){

}