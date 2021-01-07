#include "IPricer.h"

IPricer::IPricer(IModel *model, Derivative *derivative, PnlRng *rng, double fdStep, int nbSamples){
    this->model_ = model;
    this->derivative_ = derivative;
    this->rng_ = rng;
    this->fdStep_ = fdStep;
    this->nbSamples_ = nbSamples;
}

IPricer::~IPricer(){
    
}