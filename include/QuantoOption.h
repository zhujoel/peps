#pragma once

#include "Derivative.h"

// Option quanto sur un actif étranger avec un strike en devise domestique --
// utilisé pour testé préalablement sur un modèle simple
class QuantoOption : public Derivative {

  public:
    double K_; // strike en devise domestique de l'option Quanto
    double rf_; // taux d'intéret étranger -- on considère que c'est un taux constant (TODO: à changer si nécessaire)

     QuantoOption(double T, int nbTimeSteps, double rf, double K);
    ~QuantoOption();
    
    /**
    * Calcul le payoff d'une option quanto dont les actifs ont été convertis dans la devise domestique.
    * @param path[in] contient les actifs étrangers ramenés en devise domestique.
    * Données de la matrice path :
    * 1ère colonne : actif sans risque étranger en devise domestique
    * 2ème colonne : actif risqué étranger en devise domestique
    */
    double payoff(const PnlMat *path) const;
};