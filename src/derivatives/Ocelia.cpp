#include "derivatives/Ocelia.h"
#include "pnl/pnl_mathtools.h"
#include <math.h>
#include <stdexcept>

// TODO: changer le 0.0 en un r?
Ocelia::Ocelia(double T, int nbTimeSteps, int size, int nb_sous_jacents, DateTimeVector *all_dates) : IDerivative(T, nbTimeSteps, size)
{
    this->nb_sous_jacents_ = nb_sous_jacents;
    // TODO: mettre une vérif que le nb de timesteps soit le meme que la taille de all_dates
    this->valeurs_n_ans_ = pnl_vect_create_from_zero(this->nb_sous_jacents_);
    this->valeurs_initiales_ = pnl_vect_create_from_zero(this->nb_sous_jacents_);
    this->nouveau_depart_ = pnl_vect_create_from_zero(this->nb_sous_jacents_);
    this->perfs_ = pnl_vect_create_from_zero(this->nb_sous_jacents_);
    
    DateTimeVector *dates_semestrielles = new DateTimeVector(16);
    dates_semestrielles->parseFile("../data/dates_semest");
    DateTimeVector *dates_valeurs_n_ans = new DateTimeVector(35);
    dates_valeurs_n_ans->parseFile("../data/dates_valeurs_n");

    this->indices_dates_semestrielles_ = pnl_vect_int_create(dates_semestrielles->nbDates_);
    this->indices_dates_valeurs_n_ans_ = pnl_vect_int_create(dates_valeurs_n_ans->nbDates_);
    calcul_indices_dates(all_dates, dates_semestrielles, this->indices_dates_semestrielles_);
    calcul_indices_dates(all_dates, dates_valeurs_n_ans, this->indices_dates_valeurs_n_ans_);

    delete dates_semestrielles;
    delete dates_valeurs_n_ans;
}

Ocelia::~Ocelia(){
    pnl_vect_int_free(&this->indices_dates_semestrielles_);
    pnl_vect_int_free(&this->indices_dates_valeurs_n_ans_);
    pnl_vect_free(&this->valeurs_n_ans_);
    pnl_vect_free(&this->valeurs_initiales_);
    pnl_vect_free(&this->nouveau_depart_);
    pnl_vect_free(&this->perfs_);
}

// TODO: tester cette fonction
double Ocelia::get_foreign_index_market_value(const PnlMat* path, int date_idx, int idx){
    if(idx > 3) throw std::invalid_argument("idx must be between 0 and 3!");
    
    double S_T = MGET(path, date_idx, idx);
    if(idx == 3) return S_T;
    // TODO: ajouter un T qui représente le temps écoulé entre 0 et actuellement aussi
    return S_T/MGET(path, date_idx, idx+this->nb_sous_jacents_)*exp(0); // TODO: exp(0) devrait etre l'actif sans risque étranger à la place
}

double Ocelia::compute_perf_moyenne_panier(const PnlMat *path)
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

void Ocelia::compute_valeurs_n_ans(const PnlMat *path, PnlVect *valeurs, int N)
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

void Ocelia::compute_perfs_n_ans(const PnlMat *path, PnlVect *perfs, int N){
    compute_valeurs_n_ans(path, this->valeurs_n_ans_, N);
    for(int i = 0; i < this->nb_sous_jacents_; ++i){
        LET(perfs, i) = GET(this->valeurs_n_ans_, i)/GET(this->nouveau_depart_, i) - 1;
    }
}

void Ocelia::compute_nouveau_depart(const PnlMat *path){
    compute_valeurs_n_ans(path, this->valeurs_initiales_, 0);
    for(int i = 0; i < this->nb_sous_jacents_; ++i){
        LET(this->nouveau_depart_, i) = GET(this->valeurs_initiales_, i);
    }
    compute_perfs_n_ans(path, this->perfs_, 1);

    if(pnl_vect_max(this->perfs_) <= -0.1){
        pnl_vect_mult_scalar(this->nouveau_depart_, 0.9);
    }
}

double Ocelia::compute_flux_n_ans(const PnlMat *path, int N){
    if(N >= 4 && N <= 7){
        return 1.24 + (N - 4) * 0.08;
    }
    else if(N == 8){
        compute_perfs_n_ans(path, this->perfs_, 8);
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
    // TODO: ajouter en [out] un indice de l'année pour indiquer quand le produit s'exerce
    double val_liquidative_initiale = 100.0;
    compute_nouveau_depart(path);
    
    for(int n = 4 ; n <= 8; ++n){
        compute_perfs_n_ans(path, this->perfs_, n);
        if(are_all_positive(this->perfs_)){
            return val_liquidative_initiale*compute_flux_n_ans(path, n);
        }
    }

    return -1;
}

void trunc(PnlVect* vect, int n){
    int ten = pow(10, n);
    for(int i = 0; i < vect->size; ++i){
        LET(vect, i) = roundl(GET(vect, i)*ten)/ten;
    }
}

bool are_all_positive(PnlVect *vect){
    return pnl_vect_min(vect) >= 0;
}