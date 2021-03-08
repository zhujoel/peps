#include "pricers/IPricer.h"

IPricer::IPricer(IModel * const model, IDerivative * const derivative, PnlRng * const rng, double fdStep, int nbSamples){
    this->model_ = model;
    this->derivative_ = derivative;
    this->rng_ = rng;
    this->fdStep_ = fdStep;
    this->nbSamples_ = nbSamples;
    this->path_ = pnl_mat_create(model->nbTimeSteps_+1, derivative->size_);
    this->shift_path_ = pnl_mat_create(model->nbTimeSteps_+1, derivative->size_);
}

IPricer::~IPricer(){
    pnl_mat_free(&this->path_);
    pnl_mat_free(&this->shift_path_);
}