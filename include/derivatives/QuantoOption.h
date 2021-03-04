#pragma once

#include "derivatives/IDerivative.h"

// Option quanto sur un actif étranger avec un strike en devise domestique --
// utilisé pour testé préalablement sur un modèle simple
class QuantoOption : public IDerivative {

  public:
    double K_; // strike en devise domestique de l'option Quanto
    double rf_; // taux d'intéret étranger (foreign rate) -- on considère que c'est un taux constant

    QuantoOption(double T, int nbTimeSteps, int size, double rf, double K);
    ~QuantoOption();
    
    void adjust_sigma(PnlMat *sigma) const; // adjust computed sigma en fonction du nombre de produit dans la matrix path et de comment ils sont organisés
    void adjust_past(PnlMat *past) const;
    void adjust_spot(PnlVect *spot) const;
    double get_annee_payoff() const;
    
    double payoff(const PnlMat *path);
};