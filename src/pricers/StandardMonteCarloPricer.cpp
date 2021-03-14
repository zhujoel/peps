#include "pricers/StandardMonteCarloPricer.h"
#include "libs/MathLib.h"

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel * const model, IDerivative * const derivative, PnlRng * const rng, double fdStep, int nbSamples)
    : IPricer(model, derivative, rng, fdStep, nbSamples)
{

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::price_and_delta(const PnlVect * const spot, double t, double &prix, double &price_std_dev, PnlVect * const delta, PnlVect * const delta_std_dev)
{
    prix = 0.;
    price_std_dev = 0.;
    pnl_vect_set_zero(delta);
    pnl_vect_set_zero(delta_std_dev);

    // simulations
    double M = this->nbSamples_;
    for(int j = 0; j < M; ++j){
        this->model_->asset(this->path_, t, this->rng_, spot);
        this->add_price(t, prix, price_std_dev);
        this->add_delta(t, delta, delta_std_dev);
    }

    // actualisation
    prix /= M;
    price_std_dev /= M;
    price_std_dev = sqrt(exp(2)*(price_std_dev - prix * prix)/M);

    for(int d = 0 ; d < this->derivative_->size_; ++d){
        LET(delta, d) = GET(delta, d) / M;
        LET(delta_std_dev, d) = GET(delta_std_dev, d) / M;
        
        double St = GET(spot, d);
        double acc = GET(delta, d);
        LET(delta_std_dev, d) = sqrt(exp(2)*(GET(delta_std_dev, d) - acc * acc)/(2*M*this->fdStep_*St));
        LET(delta, d) = acc / (2*this->fdStep_*St);
    }
}

void StandardMonteCarloPricer::price(const PnlVect * const spot, double t, double &prix, double &price_std_dev)
{
    prix = 0.;
    price_std_dev = 0.;

    // simulations
    double M = this->nbSamples_;
    for(int j = 0; j < M; ++j){
        this->model_->asset(this->path_, t, this->rng_, spot);
        this->add_price(t, prix, price_std_dev);
    }

    // actualisation
    prix /= M;
    price_std_dev /= M;
    price_std_dev = sqrt(exp(2)*(price_std_dev - prix * prix)/M);
}

// TODO: voir comment gérer les rd
void StandardMonteCarloPricer::add_price(double t, double &prix, double &std_dev)
{
    double T = this->derivative_->get_annee_payoff();
    double price = this->derivative_->payoff(this->path_);
    double rd = 0.03;
    price = exp(-rd*(T-t))*price;
    
    prix += price;
    std_dev += price * price;
}

void StandardMonteCarloPricer::add_delta(double t, PnlVect * const delta, PnlVect * const std_dev)
{
    double rd = 0.03;
    for (int d = 0; d < this->derivative_->size_; ++d)
    {
        double T = this->derivative_->get_annee_payoff();
        this->model_->shift_asset(this->shift_path_, t, this->path_, d, this->fdStep_);
        double payoff_1 = this->derivative_->payoff(this->shift_path_);
        this->model_->shift_asset(this->shift_path_, t, this->path_, d, -this->fdStep_);
        double payoff_2 = this->derivative_->payoff(this->shift_path_);
        double diff = payoff_1 - payoff_2;
        diff = exp(-rd*(T-t))*diff;

        LET(delta, d) += diff;
        LET(std_dev, d) += diff * diff;
    }
}
