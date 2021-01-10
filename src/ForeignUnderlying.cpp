#include "ForeignUnderlying.h"

ForeignUnderlying::ForeignUnderlying(double und_spot, double zc_spot) : IUnderlying(und_spot){
    this->mkt_ = new Market(zc_spot);
}

ForeignUnderlying::~ForeignUnderlying(){

}


void ForeignUnderlying::simulate_price(IModel *model, double T, int nbTimeSteps, PnlRng *rng, PnlMat *sigma, double r, int ind){
    this->price_ = pnl_vect_create(nbTimeSteps+1);
    LET(this->price_, 0) = this->spot_;

    model->asset(this->price_, T, nbTimeSteps, rng, sigma, r, ind);

    pnl_vect_print(this->price_);
}
