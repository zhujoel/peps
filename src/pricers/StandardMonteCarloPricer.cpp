#include "pricers/StandardMonteCarloPricer.h"

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel * const model, IDerivative * const derivative, PnlRng * const rng, double fdStep, int nbSamples)
: IPricer(model, derivative, rng, fdStep, nbSamples){

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::price_and_delta(const PnlMat * const past, double t, const PnlMat * const sigma, double &prix, double &price_std_dev, PnlVect * const delta, PnlVect * const delta_std_dev)
{
    prix = 0.;
    price_std_dev = 0.;
    pnl_vect_set_zero(delta);
    pnl_vect_set_zero(delta_std_dev);

    double M = this->nbSamples_;
    for(int j = 0; j < M; ++j){
        this->model_->asset(this->path_, t, this->derivative_->T_, this->rng_, past, sigma);
        this->add_price(t, prix, price_std_dev);
        this->add_delta(t, past->m, delta, delta_std_dev);
    }

    prix /= M;
    price_std_dev /= M;
    price_std_dev = sqrt(exp(2)*(price_std_dev - prix * prix)/M);

    for(int d = 0 ; d < this->derivative_->size_; ++d){
        LET(delta, d) = GET(delta, d) / M;
        LET(delta_std_dev, d) = GET(delta_std_dev, d) / M;
        
        double St = MGET(past, past->m-1, d);
        double acc = GET(delta, d);
        LET(delta_std_dev, d) = sqrt(exp(2)*(GET(delta_std_dev, d) - acc * acc)/(2*M*this->fdStep_*St));
        LET(delta, d) = acc / (2*this->fdStep_*St);
    }
}

void StandardMonteCarloPricer::price(const PnlMat * const past, double t, const PnlMat * const sigma, double &prix, double &price_std_dev)
{
    prix = 0.;
    price_std_dev = 0.;

    double M = this->nbSamples_;
    for(int j = 0; j < M; ++j){
        this->model_->asset(this->path_, t, this->derivative_->T_, this->rng_, past, sigma);
        this->add_price(t, prix, price_std_dev);
    }

    prix /= M;
    price_std_dev /= M;
    price_std_dev = sqrt(exp(2)*(price_std_dev - prix * prix)/M);
}

void StandardMonteCarloPricer::add_price(double t, double &prix, double &std_dev)
{
    double r = this->model_->rates_->get_domestic_rate();
    double T = this->derivative_->get_annee_payoff();
    double price = this->derivative_->payoff(this->path_);
    price = exp(-r*(T-t))*price;
    
    prix += price;
    std_dev += price * price;
}

void StandardMonteCarloPricer::add_delta(double t, int pastSize, PnlVect * const delta, PnlVect * const std_dev)
{
    double r = this->model_->rates_->get_domestic_rate();
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
