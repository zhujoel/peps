#include "StandardMonteCarloPricer.h"
#include <iostream>

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel *model, PnlRng *rng, double fdStep, int nbSamples)
: IPricer(model, rng, fdStep, nbSamples){

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::simulate(double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev)
{
    for(int j = 0; j < this->nbSamples_; ++j){
        this->model_->asset(this->rng_);
        this->price(prix, price_std_dev);
        this->delta(delta, delta_std_dev);
    }

    prix /= this->nbSamples_;
    price_std_dev /= this->nbSamples_;
    discount_price(0, prix, price_std_dev);
    for(int d = 0 ; d < this->model_->derivative_->size_; ++d){
        LET(delta, d) = GET(delta, d) / this->nbSamples_;
        LET(delta_std_dev, d) = GET(delta_std_dev, d) / this->nbSamples_;
    }
    discount_delta(0, delta, delta_std_dev);
}

void StandardMonteCarloPricer::price(double &prix, double &std_dev)
{
    double price = this->model_->derivative_->payoff();
    prix += price;
    std_dev += price * price;
}

void StandardMonteCarloPricer::delta(PnlVect *delta, PnlVect *std_dev)
{
    double timeStep = this->model_->derivative_->T_/this->model_->derivative_->nbTimeSteps_;
    // for (int d = 0; d < this->model_->derivative_->size_; ++d)
    // {
        this->model_->shiftAsset(this->fdStep_, 0, timeStep);
        
        double payoff_1 = this->model_->derivative_->shifted_payoff();
        this->model_->shiftAsset(-this->fdStep_, 0, timeStep);
        double payoff_2 = this->model_->derivative_->shifted_payoff();
        double diff = payoff_1 - payoff_2;
        LET(delta, 1) += diff;
        LET(std_dev, 1) += diff * diff;
    // }
}


void StandardMonteCarloPricer::discount_price(double t, double &prix, double &std_dev)
{
    double r = this->model_->derivative_->rd_;
    double T = this->model_->derivative_->T_;
    double M = this->nbSamples_;
    std_dev = sqrt(exp(-2*r*(T-t))*(std_dev - prix * prix)/M);
    prix = exp(-r*(T-t))*prix;
}
    
void StandardMonteCarloPricer::discount_delta(double t, PnlVect *delta, PnlVect *std_dev)
{
    double r = this->model_->derivative_->rd_;
    double T = this->model_->derivative_->T_;
    double M = this->nbSamples_;
    for (int d = 0; d < this->model_->derivative_->size_; ++d)
    {   
        double s0 = this->model_->getSpot(d);
        double acc = GET(delta, d) / (2*this->fdStep_*s0);
        LET(std_dev, d) = sqrt(exp(-2*r*(T-t))*(GET(std_dev, d) - acc * acc)/(2*M*this->fdStep_*s0));
        LET(delta, d) = exp(-r*(T-t))*acc;
    }
}
