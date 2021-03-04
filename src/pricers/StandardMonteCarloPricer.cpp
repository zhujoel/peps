#include "pricers/StandardMonteCarloPricer.h"
#include <iostream>

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples)
: IPricer(model, derivative, rng, fdStep, nbSamples){

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::simulate(const PnlMat *past, double t, const PnlMat *sigma, double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev)
{
    double M = this->nbSamples_;
    for(int j = 0; j < M; ++j){
        this->model_->asset(this->path_, t, this->derivative_->T_, this->derivative_->nbTimeSteps_, this->rng_, past, sigma);
        this->price(t, prix, price_std_dev);
        this->delta(t, past->m, delta, delta_std_dev);
    }

    // price
    prix /= M;
    price_std_dev /= M;
    price_std_dev = sqrt(exp(-2)*(price_std_dev - prix * prix)/M);

    // delta
    for(int d = 0 ; d < this->derivative_->size_; ++d){
        LET(delta, d) = GET(delta, d) / this->nbSamples_;
        LET(delta_std_dev, d) = GET(delta_std_dev, d) / this->nbSamples_;
    }
    
    for (int d = 0; d < this->derivative_->size_; ++d)
    {   
        double s0 = MGET(past, past->m-1, d); // on récupère le spot
        double acc = GET(delta, d) / (2*this->fdStep_*s0);
        LET(delta_std_dev, d) = sqrt(exp(-2)*(GET(delta_std_dev, d) - acc * acc)/(2*M*this->fdStep_*s0));
    }
}

void StandardMonteCarloPricer::price(double t, double &prix, double &std_dev)
{
    double r = this->model_->rd_;
    double T = this->derivative_->get_annee_payoff();
    double price = this->derivative_->payoff(this->path_);
    price = exp(-r*(T-t))*price;
    
    prix += price;
    std_dev += price * price;
}

void StandardMonteCarloPricer::delta(double t, int pastSize, PnlVect *delta, PnlVect *std_dev)
{
    double r = this->model_->rd_;
    for (int d = 0; d < this->derivative_->size_; ++d)
    {
        double T = this->derivative_->get_annee_payoff();
        this->model_->shift_asset(this->shift_path_, this->path_, d, this->fdStep_, pastSize);
        double payoff_1 = this->derivative_->payoff(this->shift_path_);
        this->model_->shift_asset(this->shift_path_, this->path_, d, -this->fdStep_, pastSize);
        double payoff_2 = this->derivative_->payoff(this->shift_path_);
        double diff = payoff_1 - payoff_2;
        diff = exp(-r*(T-t))*diff;

        LET(delta, d) += diff;
        LET(std_dev, d) += diff * diff;
    }
}