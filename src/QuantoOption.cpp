#include "QuantoOption.h"
#include "pnl/pnl_mathtools.h"
#include <iostream>

QuantoOption::QuantoOption(double T, int nbTimeSteps, int size, double rf, double K) : IDerivative(T, nbTimeSteps, size){
    this->rf_ = rf;
    this->K_ = K;
}

QuantoOption::~QuantoOption(){

}

double QuantoOption::payoff(const PnlMat *path) const{
    double B_T = MGET(path, this->nbTimeSteps_, 0); // valeur finale de l'actif sans risque converti 
    double S_T = MGET(path, this->nbTimeSteps_, 1); // valeur finale de l'actif risqué converti

    return MAX(S_T - this->K_, 0);
    
}

// TODO: ajouter classe taux d'intéret ?
// TODO: ajouter des classes pour séparer les zc et les actifs risqué dans un pf ?