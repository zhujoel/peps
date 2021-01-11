#include "IPricer.h"

IPricer::IPricer(IModel *model, PnlRng *rng, double fdStep, int nbSamples){
    this->model_ = model;
    this->rng_ = rng;
    this->fdStep_ = fdStep;
    this->nbSamples_ = nbSamples;
}

IPricer::~IPricer(){
}