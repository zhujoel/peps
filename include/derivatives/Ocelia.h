#pragma once

#include "derivatives/IDerivative.h"
#include "libs/DateTimeVector.h"
#include "pnl/pnl_matrix.h"
#include <vector>

class Ocelia : public IDerivative{
    public:
        int annee_payoff_;
        int nb_sous_jacents_;
        PnlVectInt *indices_dates_semestrielles_; // indices des dates de constatation dans le path 
        PnlVectInt *indices_dates_valeurs_n_ans_; // indices dans le path des dates des valeurs n ans
        PnlVect *valeurs_n_ans_; // moyenne de la valeur des indices à l'année n
        PnlVect *valeurs_initiales_; // moyenne des valeurs initiales (année 0);
        PnlVect *nouveau_depart_; // valeurs de l'effet nouveau départ
        PnlVect *perfs_; // perfs à l'année n

        Ocelia(double T, int size, int nb_sous_jacents);
        ~Ocelia();

        void adjust_sigma(PnlMat *sigma) const; // adjust computed sigma en fonction du nombre de produit dans la matrix path et de comment ils sont organisés
        void adjust_past(PnlMat *past) const;
        void adjust_spot(PnlVect *spot) const;
        double get_annee_payoff() const;
        void init_indices(const std::vector<DateTime*> &all_dates, const std::vector<DateTime*> &dates_semestrielles, const std::vector<DateTime*> &dates_valeurs_n_ans);

        double get_foreign_index_market_value(const PnlMat* path, int date_idx, int idx) const;
        double compute_perf_moyenne_panier(const PnlMat *path) const;
        void compute_valeurs_n_ans(PnlVect *valeurs, const PnlMat *path, int N) const; // calcule la valeur moyenne des indices pour l'année n
        void compute_perfs_n_ans(PnlVect *perfs, const PnlMat *path, int N) const; // compute performance pour un indice à une année
        void compute_nouveau_depart(const PnlMat *path); 
        double compute_flux_n_ans(const PnlMat *path, int N) const; // calcul du C(N) cf 1.3
        double payoff(const PnlMat *path);
};
