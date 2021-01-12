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
        DateTime **dates_valeurs_n_ans_; // 1.2 du pdf d'analyse financier
        PnlVectInt *indices_dates_constatation_; // indices des dates de constatation dans le path 
        PnlVectInt *indices_dates_valeurs_n_ans_; // indices dans le path des dates des valeurs n ans
        PnlVect *valeurs_n_ans_; // moyenne de la valeur des indices à l'année n
        PnlVect *valeurs_initiales_; // moyenne des valeurs initiales (année 0);
        // PnlMat *perfs_; // performances à chaque anneé n des indices (cf 1.3 Perf(N, I));
        PnlVect *valeurs_departs_;

        // TODO: paramètres peut etre pas tous nécessaires ?
        Ocelia(double T, int nbTimeSteps, int size, double r_gbp, double r_chf, double r_jpy, double r_eur, IUnderlying **underlyings);
        ~Ocelia();
        double payoff();
        double shifted_payoff() const;
        void fill_dates_perf();
        void fill_dates_valeurs();
        double compute_perf_moyenne_panier();
        void compute_valeurs_n_ans(PnlVect *valeurs, int N); // calcule la valeur moyenne des indices pour l'année n
        void compute_perfs_n_ans(PnlVect *perfs, int N); // compute performance pour un indice à une année
        void compute_valeurs_departs(); 
        void init_valeurs_departs();
        bool are_all_perfs_positive(PnlVect *perfs); // détermine si tous les performances sont positives
        double compute_flux_n_ans(int n); // calcul du C(N) cf 1.3

        // TODO: abstraire le calcul de ces indices dans une autre classe peut etre ?
        /**
         * @brief voir parametres pour indices_dates_constatations et valeurs_n_ans
         * 
         * @param all_dates[in] on suppose que c'est rangé dans l'ordre croissant et dates est inclu dans all_dates
         * @param dates[in] on suppose que c'est rangé dans l'ordre croissant 
         * @param indices[out]
         */
        void calcul_indices_dates(DateTime **all_dates, DateTime **dates, PnlVectInt *indices);

};