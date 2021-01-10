#pragma once

#include "IDerivative.h"
#include <ctime>

class Ocelia : public IDerivative{
    public:
        // TODO: peut etre mettre une classe InterestRate qui garde toutes données de taux d'intéret
        // TODO: on considère que c'est constant
        double r_gbp_; // taux d'intéret gbp
        double r_chf_; // taux d'intéret franc suisse
        double r_jpy_; // japonais
        double r_eur_; // euro
        tm *dates_constatation_perf; // dates de constatation de la performance du panier pour le calcul du payoff

        // TODO: paramètres peut etre pas tous nécessaires ?
        Ocelia(double T, int nbTimeSteps, int size, double r_gbp, double r_chf, double r_jpy, double r_eur);
        ~Ocelia();
        double payoff(const PnlMat *path) const;
};