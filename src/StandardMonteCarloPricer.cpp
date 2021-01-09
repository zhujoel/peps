#include "StandardMonteCarloPricer.h"
#include <iostream>

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples)
: IPricer(model, derivative, rng, fdStep, nbSamples){

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::simulate(double &prix, double &std_dev, PnlVect *delta, PnlVect *delta_std_dev){
    int N = this->derivative_->nbTimeSteps_;
    int D = this->derivative_->size_;
    int M = this->nbSamples_;
    double T = this->derivative_->T_;
    double r = this->model_->rd_;
    double h = this->fdStep_;
    double timeStep = T/N;

    // price computation
    double acc = 0., var = 0.;

    // TODO: est-ce vraiment nécessaire ?
    pnl_vect_set_zero(delta);
    pnl_vect_set_zero(delta_std_dev);

    for(int j = 0; j < M; ++j){
        this->model_->asset(this->path_, T, N, this->rng_);
        this->price(acc, var);
        this->delta(delta, delta_std_dev);
    }

    acc /= M;
    var = var / M - acc * acc;  
    
    prix = exp(-r*T)*acc;
    std_dev = sqrt(exp(-2*r*T)*var/M);


    for (int d = 0; d < D; ++d)
    {   
        double acc = GET(delta, d);
        double var = GET(delta_std_dev, d);
        double s0 = GET(this->model_->spot_, d);
        acc /= M;
        var = var / M - acc * acc; 
        acc /= (2*h*s0);
        LET(delta, d) = exp(-r*T)*acc;
        LET(delta_std_dev, d) = sqrt(exp(-2*r*T)*var/(2*M*h*s0));
    }
}

void StandardMonteCarloPricer::price(double &prix, double &std_dev){
    double price = this->derivative_->payoff(this->path_);
    prix += price;
    std_dev += price * price;
}

void StandardMonteCarloPricer::delta(PnlVect *delta, PnlVect *std_dev)
{
    double timeStep = this->derivative_->T_/this->derivative_->nbTimeSteps_;
    for (int d = 0; d < this->derivative_->size_; ++d)
    {
        this->model_->shiftAsset(this->shift_path_, this->path_, d, this->fdStep_, 0, timeStep);
        double payoff_1 = this->derivative_->payoff(this->shift_path_);  
            
        this->model_->shiftAsset(this->shift_path_, this->path_, d, -this->fdStep_, 0, timeStep);
        double payoff_2 = this->derivative_->payoff(this->shift_path_); 
            
        double diff = payoff_1 - payoff_2;
        LET(delta, d) += diff;
        LET(std_dev, d) += diff * diff;
    }
}
