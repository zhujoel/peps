#include "derivatives/Ocelia.h"
#include <stdexcept>
#include "libs/DateTimeVector.h"
#include "pnl/pnl_mathtools.h"
#include "libs/Utilities.h"

Ocelia::Ocelia(double T, int size, int nb_sous_jacents, const std::vector<DateTime*> &all_dates) : IDerivative(T, size)
{
    this->annee_payoff_ = 0;    
    this->nb_sous_jacents_ = nb_sous_jacents;
    this->valeurs_n_ans_ = pnl_vect_create_from_zero(this->nb_sous_jacents_);
    this->valeurs_initiales_ = pnl_vect_create_from_zero(this->nb_sous_jacents_);
    this->nouveau_depart_ = pnl_vect_create_from_zero(this->nb_sous_jacents_);
    this->perfs_ = pnl_vect_create_from_zero(this->nb_sous_jacents_);
    
    std::vector<DateTime*> dates_semestrielles;
    parse_dates_file(dates_semestrielles, "../data/dates/dates_semest.csv", 16, '-');
    std::vector<DateTime*> dates_valeurs_n_ans;
    parse_dates_file(dates_valeurs_n_ans, "../data/dates/dates_valeurs_n.csv", 35, '-');

    this->indices_dates_semestrielles_ = pnl_vect_int_new();
    calcul_indices_dates(this->indices_dates_semestrielles_, all_dates, dates_semestrielles);
    this->indices_dates_valeurs_n_ans_ = pnl_vect_int_new();
    calcul_indices_dates(this->indices_dates_valeurs_n_ans_, all_dates, dates_valeurs_n_ans);

    delete_date_vector(dates_semestrielles);
    delete_date_vector(dates_valeurs_n_ans);
}

Ocelia::~Ocelia(){
    pnl_vect_int_free(&this->indices_dates_semestrielles_);
    pnl_vect_int_free(&this->indices_dates_valeurs_n_ans_);
    pnl_vect_free(&this->valeurs_n_ans_);
    pnl_vect_free(&this->valeurs_initiales_);
    pnl_vect_free(&this->nouveau_depart_);
    pnl_vect_free(&this->perfs_);
}

void Ocelia::adjust_sigma(PnlMat *sigma) const {
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < sigma->n; ++j){
            MLET(sigma, i, j) += MGET(sigma, i+4, j);
        }
    }
}

void Ocelia::adjust_past(PnlMat *past) const {
    for(int i = 0; i < past->m; ++i){
        for(int j = 0; j < 3; ++j){
            MLET(past, i, j) = MGET(past, i, j)*MGET(past, i, j+4);
        }
    }
}
void Ocelia::adjust_spot(PnlVect *spot) const {
    for(int j = 0; j < 3; ++j){
        LET(spot, j) = GET(spot, j)*GET(spot, j+4);
    }
}

double Ocelia::get_annee_payoff() const {
    return this->annee_payoff_;
}

double Ocelia::get_foreign_index_market_value(const PnlMat* path, int date_idx, int idx) const {
    if(idx > 3) throw std::invalid_argument("idx must be between 0 and 3!");
    
    double S_T = MGET(path, date_idx, idx);
    if(idx == 3) return S_T;
    // TODO: ajouter un T qui représente le temps écoulé entre 0 et actuellement aussi
    // TODO: exp(0) devrait etre l'actif sans risque étranger à la place
    return S_T/MGET(path, date_idx, idx+this->nb_sous_jacents_)*exp(0);
}

double Ocelia::compute_perf_moyenne_panier(const PnlMat *path) const
{
    double perf_moy_panier = 0.0;
    for(int t = 0; t < this->indices_dates_semestrielles_->size; ++t){
        double somme = 0.0;
        for(int i = 0; i < this->nb_sous_jacents_; ++i){
            double I_i_s = get_foreign_index_market_value(path, GET_INT(this->indices_dates_semestrielles_, t), i);
            double I0 = GET(this->valeurs_initiales_, i);
            somme += I_i_s/ I0 - 1;
        }
        perf_moy_panier += MAX(somme/this->nb_sous_jacents_, 0);
    }

    return perf_moy_panier/this->indices_dates_semestrielles_->size;
}

void Ocelia::compute_valeurs_n_ans(PnlVect *valeurs, const PnlMat *path, int N) const
{
    if(N == 2 || N == 3){
        throw std::invalid_argument("N n'a pas de valeurs pour N=2 ou N=3 !");
    }
    if(N > 3){
        N -= 2;
    }

    pnl_vect_set_zero(valeurs);
    for(int t = 0; t < 5; ++t){
        for(int i = 0; i < this->nb_sous_jacents_; ++i){
            double S_T = get_foreign_index_market_value(path, GET_INT(this->indices_dates_valeurs_n_ans_, t+(N*5)), i);
            LET(valeurs, i) += S_T;
        }
    }
    pnl_vect_div_scalar(valeurs, 5);
    trunc(valeurs, 4);
}

void Ocelia::compute_perfs_n_ans(PnlVect *perfs, const PnlMat *path, int N) const{
    compute_valeurs_n_ans(this->valeurs_n_ans_, path, N);
    for(int i = 0; i < this->nb_sous_jacents_; ++i){
        LET(perfs, i) = GET(this->valeurs_n_ans_, i)/GET(this->nouveau_depart_, i) - 1;
    }
}

void Ocelia::compute_nouveau_depart(const PnlMat *path){
    compute_valeurs_n_ans(this->valeurs_initiales_, path, 0);
    for(int i = 0; i < this->nb_sous_jacents_; ++i){
        LET(this->nouveau_depart_, i) = GET(this->valeurs_initiales_, i);
    }
    compute_perfs_n_ans(this->perfs_, path, 1);

    if(pnl_vect_max(this->perfs_) <= -0.1){
        pnl_vect_mult_scalar(this->nouveau_depart_, 0.9);
    }
}

double Ocelia::compute_flux_n_ans(const PnlMat *path, int N) const{
    if(N >= 4 && N <= 7){
        return 1.24 + (N - 4) * 0.08;
    }
    else if(N == 8){
        compute_perfs_n_ans(this->perfs_, path, 8);
        double perf_moy = compute_perf_moyenne_panier(path);
        if(are_all_positive(this->perfs_)){
            return MAX(1.56, 1 + perf_moy);
        }
        return 1 + perf_moy;
    }
    return 0.;
}

double Ocelia::payoff(const PnlMat *path)
{
    double val_liquidative_initiale = 100.;
    compute_nouveau_depart(path);
    
    for(int n = 4 ; n <= 8; ++n){
        compute_perfs_n_ans(this->perfs_, path, n);
        if(are_all_positive(this->perfs_) || n == 8){
            this->annee_payoff_ = n;
            return val_liquidative_initiale*compute_flux_n_ans(path, n);
        }
    }
    return -1;
}