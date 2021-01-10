#include "Ocelia.h"

// TODO: paramètres peut etre pas tous nécessaires ?
Ocelia::Ocelia(double T, int nbTimeSteps, int size, double r_gbp, double r_chf, double r_jpy, double r_eur) : IDerivative(T, nbTimeSteps, size)
{
    this->r_gbp_ = r_gbp;
    this->r_chf_ = r_chf;
    this->r_jpy_ = r_jpy;
    this->r_eur_ = r_eur;
    this->dates_constatation_perf = new tm[8];
    
    time_t now = time(0);
   // convert now to string form
   char* dt = ctime(&now);
    dates_constatation_perf[0] = *gmtime(&now);
}

Ocelia::~Ocelia(){

}

double Ocelia::payoff(const PnlMat *path) const{
    
    return 0.;
}