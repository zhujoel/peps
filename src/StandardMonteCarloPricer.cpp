#include "StandardMonteCarloPricer.h"

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel *model, Derivative *derivative, PnlRng *rng, double fdStep, int nbSamples)
: IPricer(model, derivative, rng, fdStep, nbSamples){

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::price(double &prix, double &std_dev){
    
    int N = this->derivative_->nbTimeSteps_;
    int M = this->nbSamples_;
    double T = this->derivative_->T_;
    double r = this->model_->rd_;
    double price = 0;
    double acc = 0., var = 0.;

    for (int j = 0; j < M; ++j)
    {   
        this->model_->asset(this->path_, T, N, this->rng_);
        price = this->derivative_->payoff(this->path_);
        acc += price;
        var += price * price;
    }
    acc /= M;
    var = var / M - acc * acc;  
    
    prix = exp(-r*T)*acc;
    std_dev = sqrt(exp(-2*r*T)*var/M);

}