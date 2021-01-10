#include "ForeignUnderlying.h"

ForeignUnderlying::ForeignUnderlying(double und_spot, double zc_spot) : IUnderlying(und_spot){
    this->mkt_ = new Market(zc_spot);
}

ForeignUnderlying::~ForeignUnderlying(){

}


void ForeignUnderlying::simulate_price(IModel *model, double T, int nbTimeSteps, PnlRng *rng){

}
