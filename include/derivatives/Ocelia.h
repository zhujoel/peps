#pragma once

#include "derivatives/IDerivative.h"
#include "libs/DateTimeVector.h"
#include "pnl/pnl_matrix.h"
#include <vector>

class Ocelia : public IDerivative{
    public:
        int annee_payoff;
        int nb_sous_jacents_;
        PnlVectInt *indices_dates_semestrielles_; // indices des dates de constatation dans le path 
        PnlVectInt *indices_dates_valeurs_n_ans_; // indices dans le path des dates des valeurs n ans
        PnlVect *valeurs_n_ans_; // moyenne de la valeur des indices à l'année n
        PnlVect *valeurs_initiales_; // moyenne des valeurs initiales (année 0);
        PnlVect *nouveau_depart_; // valeurs de l'effet nouveau départ
        PnlVect *perfs_; // perfs à l'année n

        Ocelia(double T, int nbTimeSteps, int size, int nb_sous_jacents, std::vector<DateTime*> all_dates);
        ~Ocelia();

        void adjust_sigma(PnlMat *sigma); // adjust computed sigma en fonction du nombre de produit dans la matrix path et de comment ils sont organisés
        void adjust_past(PnlMat *past);

        double getMaturity();
        double get_foreign_index_market_value(const PnlMat* path, int date_idx, int idx);
        double compute_perf_moyenne_panier(const PnlMat *path);
        void compute_valeurs_n_ans(const PnlMat *path, PnlVect *valeurs, int N); // calcule la valeur moyenne des indices pour l'année n
        void compute_perfs_n_ans(const PnlMat *path, PnlVect *perfs, int N); // compute performance pour un indice à une année
        void compute_nouveau_depart(const PnlMat *path); 
        double compute_flux_n_ans(const PnlMat *path, int N); // calcul du C(N) cf 1.3
        double payoff(const PnlMat *path);
};
