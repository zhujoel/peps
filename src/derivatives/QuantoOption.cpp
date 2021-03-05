#include "derivatives/QuantoOption.h"
#include "pnl/pnl_mathtools.h"

QuantoOption::QuantoOption(double T, int nbTimeSteps, int size, double rf, double K) : IDerivative(T, nbTimeSteps, size){
    this->rf_ = rf;
    this->K_ = K;
}

QuantoOption::~QuantoOption(){

}

// pas besoin d'ajuster dans quanto
void QuantoOption::adjust_sigma(PnlMat *sigma) const {}
void QuantoOption::adjust_past(PnlMat *past) const {}
void QuantoOption::adjust_spot(PnlVect *spot) const {}

double QuantoOption::get_annee_payoff() const {
    return this->T_;
}

double QuantoOption::payoff(const PnlMat *path){
    // double B_T = MGET(path, this->nbTimeSteps_, 0); // valeur finale de l'actif sans risque converti 
    double S_T = MGET(path, this->nbTimeSteps_, 1); // valeur finale de l'actif risqué converti

    return MAX(S_T - this->K_, 0);   
}