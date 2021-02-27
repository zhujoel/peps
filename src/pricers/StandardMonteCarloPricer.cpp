#include "pricers/StandardMonteCarloPricer.h"

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples)
: IPricer(model, derivative, rng, fdStep, nbSamples){

}

StandardMonteCarloPricer::~StandardMonteCarloPricer(){
    
}

void StandardMonteCarloPricer::simulate(const PnlMat *past, double t, const PnlMat *sigma, double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev)
{

    for(int j = 0; j < this->nbSamples_; ++j){
        this->model_->asset(this->path_, t, this->derivative_->T_, this->derivative_->nbTimeSteps_, this->rng_, past, sigma);
        this->price(prix, price_std_dev);
        this->delta(delta, delta_std_dev);
    }

    prix /= this->nbSamples_;
    price_std_dev /= this->nbSamples_;
    discount_price(0, prix, price_std_dev);
    for(int d = 0 ; d < this->derivative_->size_; ++d){
        LET(delta, d) = GET(delta, d) / this->nbSamples_;
        LET(delta_std_dev, d) = GET(delta_std_dev, d) / this->nbSamples_;
    }
    // discount_delta(0, delta, delta_std_dev);
}

void StandardMonteCarloPricer::price(double &prix, double &std_dev)
{
    double price = this->derivative_->payoff(this->path_);
    prix += price;
    std_dev += price * price;
}

void StandardMonteCarloPricer::delta(PnlVect *delta, PnlVect *std_dev)
{
    double timeStep = this->derivative_->T_/this->derivative_->nbTimeSteps_;
    for (int d = 0; d < this->derivative_->size_; ++d)
    {
        this->model_->shift_asset(this->shift_path_, this->path_, d, this->fdStep_, 0, timeStep);
        double payoff_1 = this->derivative_->payoff(this->shift_path_);
        this->model_->shift_asset(this->shift_path_, this->path_, d, -this->fdStep_, 0, timeStep);
        double payoff_2 = this->derivative_->payoff(this->shift_path_);
        double diff = payoff_1 - payoff_2;
        LET(delta, d) += diff;
        LET(std_dev, d) += diff * diff;
    }
}

// TODO: on peut simuler jusqu'au 13 mai 2016 (au lieu de 28 avril 2016)
void StandardMonteCarloPricer::discount_price(double t, double &prix, double &std_dev)
{
    // TODO: changer le t en : convertir une date (le 15/05/2008) jusqu'à le date de payoff qui devient le t
    // TODO: on a la proportion en faisant par ex: (nb de jours entre 2 dates)/(nb de jours entre toutes les dates)
    double r = this->model_->rd_;
    double T = this->derivative_->get_annee_payoff();
    double M = this->nbSamples_;
    std_dev = sqrt(exp(-2*r*(T-t))*(std_dev - prix * prix)/M);
    prix = exp(-r*(T-t))*prix;
}
    
void StandardMonteCarloPricer::discount_delta(double t, PnlVect *delta, PnlVect *std_dev)
{
    // double r = this->model_->rd_;
    // double T = this->derivative_->get_annee_payoff();
    // double M = this->nbSamples_;
    // for (int d = 0; d < this->derivative_->size_; ++d)
    // {   
    //     double s0 = MGET(this->model_->past_, this->model_->past_->n-1, d); // on récupère le spot
    //     double acc = GET(delta, d) / (2*this->fdStep_*s0);
    //     LET(std_dev, d) = sqrt(exp(-2*r*(T-t))*(GET(std_dev, d) - acc * acc)/(2*M*this->fdStep_*s0));
    //     LET(delta, d) = exp(-r*(T-t))*acc;
    // }
}
