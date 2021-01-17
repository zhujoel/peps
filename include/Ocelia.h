#pragma once

#include "IDerivative.h"
#include "DateTimeVector.h"
#include <string>

class Ocelia : public IDerivative{
    public:
        // TODO: data:
        // 1 vecteur date avec toutes les dates du début 15 mai 2008 à 28 avril 2016
        // 4 underlyings: 4 sous-jacent (euro, gbp, jpy, chf): prix et zc + spot

        DateTimeVector *dates_semestrielles_; // dates de constatation semestrielles de la performance du panier pour le calcul du payoff
        DateTimeVector *dates_valeurs_n_ans_; // 1.2 du pdf d'analyse financier
        PnlVectInt *indices_dates_semestrielles_; // indices des dates de constatation dans le path 
        PnlVectInt *indices_dates_valeurs_n_ans_; // indices dans le path des dates des valeurs n ans
        PnlVect *valeurs_n_ans_; // moyenne de la valeur des indices à l'année n
        PnlVect *valeurs_initiales_; // moyenne des valeurs initiales (année 0);
        PnlVect *nouveau_depart_; // valeurs de l'effet nouveau départ
        PnlVect *perfs_;

        Ocelia(double T, int nbTimeSteps, int size, IUnderlying **underlyings, DateTimeVector *dates_semestrielles, DateTimeVector *date_valeurs_n_ans);
        ~Ocelia();
        double payoff();
        double shifted_payoff() const;
        double compute_perf_moyenne_panier();
        void compute_valeurs_n_ans(PnlVect *valeurs, int N); // calcule la valeur moyenne des indices pour l'année n
        void compute_perfs_n_ans(PnlVect *perfs, int N); // compute performance pour un indice à une année
        void compute_nouveau_depart(); 
        void init_nouveau_depart();
        bool are_all_perfs_positive(PnlVect *perfs); // détermine si tous les performances sont positives
        double compute_flux_n_ans(int n); // calcul du C(N) cf 1.3

};