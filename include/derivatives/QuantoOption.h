#pragma once

#include "derivatives/IDerivative.h"

// Option quanto sur un actif étranger avec un strike en devise domestique --
// utilisé pour testé préalablement sur un modèle simple
class QuantoOption : public IDerivative {

  public:
    double K_; // strike en devise domestique de l'option Quanto
    double rf_; // taux d'intéret étranger (foreign rate) -- on considère que c'est un taux constant (TODO: à changer si nécessaire)

     QuantoOption(double T, int nbTimeSteps, int size, double rf, double K);
    ~QuantoOption();
    
    double getMaturity();
    double payoff(const PnlMat *path);
};