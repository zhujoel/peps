#include "QuantoOption.h"
#include "pnl/pnl_mathtools.h"
#include <iostream>

QuantoOption::QuantoOption(double T, int nbTimeSteps, int size, double rf, double K, IUnderlying **und) : IDerivative(T, nbTimeSteps, size, und){
    this->rf_ = rf;
    this->K_ = K;
}

QuantoOption::~QuantoOption(){

}

double QuantoOption::payoff() const{
    double B_T = GET(this->underlyings_[0]->zc_, 0); // valeur finale de l'actif sans risque converti 
    double S_T = GET(this->underlyings_[0]->price_, 1); // valeur finale de l'actif risqué converti

    return MAX(S_T - this->K_, 0);   
}