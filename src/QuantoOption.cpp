#include "QuantoOption.h"
#include "pnl/pnl_mathtools.h"
#include <iostream>

QuantoOption::QuantoOption(double T, int nbTimeSteps, int size, double rf, double K) : IDerivative(T, nbTimeSteps, size){
    this->rf_ = rf;
    this->K_ = K;
}

QuantoOption::QuantoOption(double T, int nbTimeSteps, int size, double rf, double K, IUnderlying **underlyings) : IDerivative(T, nbTimeSteps, size){
    this->underlyings_ = underlyings;
    this->rf_ = rf;
    this->K_ = K;
}

QuantoOption::~QuantoOption(){

}

double QuantoOption::payoff2(const PnlMat *path) const{
    double B_T = GET(this->underlyings_[0]->zc_, this->nbTimeSteps_); // valeur finale de l'actif sans risque converti 
    double S_T = GET(this->underlyings_[0]->price_, this->nbTimeSteps_); // valeur finale de l'actif sans risque converti 

    return MAX(S_T - this->K_, 0);   
}

double QuantoOption::shifted_payoff(const PnlVect *path) const{
    double S_T = GET(path, this->nbTimeSteps_); // valeur finale de l'actif sans risque converti 
    return MAX(S_T - this->K_, 0);
}