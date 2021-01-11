#include "StandardMonteCarloPricer.h"
#include <iostream>

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples)
: IPricer(model, derivative, rng, fdStep, nbSamples){

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::simulate(QuantoOption *option, double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev)
{
    for(int j = 0; j < this->nbSamples_; ++j){
        this->model_->asset(option, this->derivative_->T_, this->derivative_->nbTimeSteps_, this->rng_);
        this->price(prix, price_std_dev);
        this->delta(option, delta, delta_std_dev);
    }

    prix /= this->nbSamples_;
    price_std_dev /= this->nbSamples_;
    discount_price(0, prix, price_std_dev);
    for(int d = 0 ; d < this->derivative_->size_; ++d){
        LET(delta, d) = GET(delta, d) / this->nbSamples_;
        LET(delta_std_dev, d) = GET(delta_std_dev, d) / this->nbSamples_;
    }
    discount_delta(0, delta, delta_std_dev);
}

void StandardMonteCarloPricer::price(double &prix, double &std_dev)
{
    double price = this->derivative_->payoff(this->path_);
    prix += price;
    std_dev += price * price;
}

void StandardMonteCarloPricer::delta(QuantoOption *option, PnlVect *delta, PnlVect *std_dev)
{
    double timeStep = this->derivative_->T_/this->derivative_->nbTimeSteps_;
    // for (int d = 0; d < this->derivative_->size_; ++d)
    // {
        this->model_->shiftAsset(option, 0, this->fdStep_, 0, timeStep);
        
        double payoff_1 = this->derivative_->shifted_payoff(option->underlyings_[0]->shifted_price_);
        this->model_->shiftAsset(option, 0, -this->fdStep_, 0, timeStep);
        double payoff_2 = this->derivative_->shifted_payoff(option->underlyings_[0]->shifted_price_);
        double diff = payoff_1 - payoff_2;
        LET(delta, 1) += diff;
        LET(std_dev, 1) += diff * diff;
    // }
}


void StandardMonteCarloPricer::discount_price(double t, double &prix, double &std_dev)
{
    double r = this->model_->rd_;
    double T = this->derivative_->T_;
    double M = this->nbSamples_;
    std_dev = sqrt(exp(-2*r*(T-t))*(std_dev - prix * prix)/M);
    prix = exp(-r*(T-t))*prix;
}
    
void StandardMonteCarloPricer::discount_delta(double t, PnlVect *delta, PnlVect *std_dev)
{
    double r = this->model_->rd_;
    double T = this->derivative_->T_;
    double M = this->nbSamples_;
    for (int d = 0; d < this->derivative_->size_; ++d)
    {   
        double s0 = GET(this->model_->spot_, d);
        double acc = GET(delta, d) / (2*this->fdStep_*s0);
        LET(std_dev, d) = sqrt(exp(-2*r*(T-t))*(GET(std_dev, d) - acc * acc)/(2*M*this->fdStep_*s0));
        LET(delta, d) = exp(-r*(T-t))*acc;
    }
}
