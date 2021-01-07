#include "QuantoOption.h"
#include "pnl/pnl_mathtools.h"

QuantoOption::QuantoOption(double T, int nbTimeSteps, double rf, double K) : Derivative(T, nbTimeSteps){
    this->rf_ = rf;
    this->K_ = K;
}

QuantoOption::~QuantoOption(){

}

double QuantoOption::payoff(const PnlMat *path) const{
    double B_T = MGET(path, 0, this->nbTimeSteps_); // valeur finale de l'actif sans risque converti 
    double S_T = MGET(path, 1, this->nbTimeSteps_); // valeur finale de l'actif risqué converti

    return MAX(S_T/B_T*exp(this->rf_*this->T_) - this->K_, 0);
}

// TODO: ajouter classe taux d'intéret ?
