#include "StandardMonteCarloPricer.h"
#include <iostream>

StandardMonteCarloPricer::StandardMonteCarloPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples)
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

void StandardMonteCarloPricer::delta(PnlVect *delta, PnlVect *std_dev)
{
    int N = this->derivative_->nbTimeSteps_;
    int D = this->derivative_->size_;
    int M = this->nbSamples_;
    double T = this->derivative_->T_;
    double r = this->model_->rd_;
    double h = this->fdStep_;

    double timeStep = T/N;

    // TODO: est-ce vraiment nécessaire ?
    pnl_vect_set_zero(delta);
    pnl_vect_set_zero(std_dev);

    for (int j = 0; j < M ; ++j)
    {
        // TODO: il faudrait fusionner calcul du price et du delta
        this->model_->asset(this->path_, T, N, this->rng_);
        for (int d = 0; d < D; ++d)
        { 
            this->model_->shiftAsset(this->shift_path_, this->path_, d, h, 0, timeStep);
            double payoff_1 = this->derivative_->payoff(this->shift_path_);  
            
            this->model_->shiftAsset(this->shift_path_, this->path_, d, -h, 0, timeStep);
            double payoff_2 = this->derivative_->payoff(this->shift_path_); 
            
            double diff = payoff_1 - payoff_2;
            // if(d == 0) std::cout << "diff 0 : " << diff << std::endl;
            // if(d == 1) std::cout << "diff 1 : " << diff << std::endl;
            LET(delta, d) += diff;
            LET(std_dev, d) += diff * diff;
        }
    }
    for (int d = 0; d < D; ++d)
    {   
        double acc = GET(delta, d);
        double var = GET(std_dev, d);
        double s0 = GET(this->model_->spot_, d);
        acc /= M;
        var = var / M - acc * acc; 
        acc /= (2*h*s0);
        LET(delta, d) = exp(-r*T)*acc;
        LET(std_dev, d) = sqrt(exp(-2*r*T)*var/(2*M*h*s0));
    }
}
