#include "derivatives/QuantoOption.h"
#include "pnl/pnl_mathtools.h"

QuantoOption::QuantoOption(double T, int size, double rf, double K) : IDerivative(T, size){
    this->rf_ = rf;
    this->K_ = K;
}

QuantoOption::~QuantoOption(){

}

// pas besoin d'ajuster dans quanto
void QuantoOption::adjust_sigma(PnlMat * const sigma) const {}
void QuantoOption::adjust_past(PnlMat * const past, double timestep) const {}
void QuantoOption::adjust_spot(PnlVect * const spot, double t) const {}

double QuantoOption::get_annee_payoff() const {
    return this->T_;
}

double QuantoOption::payoff(const PnlMat *path){
    double S_T = MGET(path, path->m-1, 1); // valeur finale de l'actif risqué converti
    return MAX(S_T - this->K_, 0);   
}