#include "StandardMonteCarloPricer.h"
#include <iostream>

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples)
: IPricer(model, derivative, rng, fdStep, nbSamples){

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::simulate(double &prix, double &std_dev, PnlVect *delta, PnlVect *delta_std_dev)
{
    for(int j = 0; j < this->nbSamples_; ++j){
        this->model_->asset(this->path_, this->derivative_->T_, this->derivative_->nbTimeSteps_, this->rng_);
        this->price(prix, std_dev);
        this->delta(delta, delta_std_dev);
    }
    discount_price(0, prix, std_dev);
    discount_delta(0, delta, delta_std_dev);
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

void StandardMonteCarloPricer::discount_price(double t, double &prix, double &std_dev)
{
    double r = this->model_->rd_;
    double T = this->derivative_->T_;
    double M = this->nbSamples_;
    double acc = prix /= M;
    double var = std_dev / M - acc * acc;  

    prix = exp(-r*(T-t))*acc;
    std_dev = sqrt(exp(-2*r*(T-t))*var/M);
}
    
void StandardMonteCarloPricer::discount_delta(double t, PnlVect *delta, PnlVect *std_dev)
{
    double r = this->model_->rd_;
    double T = this->derivative_->T_;
    double M = this->nbSamples_;
    for (int d = 0; d < this->derivative_->size_; ++d)
    {   
        double s0 = GET(this->model_->spot_, d);
        double acc = GET(delta, d) / (2*M*this->fdStep_*s0);
        double var = GET(std_dev, d) / M - acc * acc;

        LET(delta, d) = exp(-r*(T-t))*acc;
        LET(std_dev, d) = sqrt(exp(-2*r*(T-t))*var/(2*M*this->fdStep_*s0));
    }
}
