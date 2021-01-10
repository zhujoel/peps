#pragma once

#include "IDerivative.h"
#include "DateTime.h"

class Ocelia : public IDerivative{
    public:
        // TODO: peut etre mettre une classe InterestRate qui garde toutes données de taux d'intéret
        // TODO: on considère que c'est constant
        double r_gbp_; // taux d'intéret gbp
        double r_chf_; // taux d'intéret franc suisse
        double r_jpy_; // japonais
        double r_eur_; // euro
        DateTime **dates_constatation_perf_; // dates de constatation de la performance du panier pour le calcul du payoff
        DateTime **dates_valeurs_n_ans; // 1.2 du pdf d'analyse financier 

        // TODO: paramètres peut etre pas tous nécessaires ?
        Ocelia(double T, int nbTimeSteps, int size, double r_gbp, double r_chf, double r_jpy, double r_eur);
        ~Ocelia();
        double payoff(const PnlMat *path) const;
        void fill_dates_perf();
        void fill_dates_valeurs();
        double compute_perf_moyenne_panier(const PnlMat *path);

};