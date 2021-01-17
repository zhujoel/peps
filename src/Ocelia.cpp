#include "Ocelia.h"
#include "pnl/pnl_mathtools.h"

// TODO: changer le 0.0 en un r
Ocelia::Ocelia(double T, int nbTimeSteps, int size, IUnderlying **underlyings) : IDerivative(T, nbTimeSteps, size, 0.0, underlyings)
{
    // TODO: mettre des variables au lieu de 16 ou 7*5
    this->dates_semestrielles_ = new DateTime*[16];
    this->dates_valeurs_n_ans_ = new DateTime*[7*5];
    this->indices_dates_constatation_ = pnl_vect_int_create(16);
    this->valeurs_n_ans_ = pnl_vect_create(4);
    this->valeurs_initiales_ = pnl_vect_create(4);
    this->perfs_ = pnl_vect_create(4);
}

Ocelia::~Ocelia(){

}

void Ocelia::fill_dates_perf(){
    fill_dates_from_file(this->dates_semestrielles_, "../data/dates_semest", 16);
}

void Ocelia::fill_dates_valeurs(){   
    fill_dates_from_file(this->dates_valeurs_n_ans_, "../data/dates_valeurs_n", 35);
}

// TODO: à virer d'ocelia et mettre qq part d'autre
void Ocelia::calcul_indices_dates(DateTime **all_dates, DateTime **dates, PnlVectInt *indices)
{
    int cnt = 0;
    for(int i = 0; i < this->nbTimeSteps_+1; ++i){
        // TODO: à tester (peut y avoir +1 ou -1 dans les indices)
        if(all_dates[i] == dates[cnt]){
            LET_INT(indices, cnt++) = i;

            if(cnt == indices->size) break;
        }
    }
}

// TODO: à debugger
void Ocelia::compute_valeurs_n_ans(PnlVect *valeurs, int N)
{
    // TODO: faut pas qu'il y ait de 2 ou 3
    if(N > 1){
        N -= 2;
    }
    // convention : tous les prix sont converti en domestique
    pnl_vect_set_zero(valeurs);
    for(int t = 0; t < 5; ++t){
        for(int i = 0; i < this->size_; ++i){
            double S_T = GET(this->underlyings_[i]->price_, GET_INT(this->indices_dates_valeurs_n_ans_, t+(N*5)));
            // double B_T = GET(this->underlyings_[i]->zc_, GET_INT(this->indices_dates_valeurs_n_ans_, t+(n*5)));
            // LET(valeurs, i) += (S_T / B_T);
            LET(valeurs, i) += S_T;
        }
    }

    // TODO: remplacer les 5 par des variables
    pnl_vect_div_scalar(valeurs, 5);
}

double Ocelia::compute_perf_moyenne_panier()
{
    double perf_moy_panier = 0.0; // performance moyenne du panier (1.4 pdf)

    for(int t = 0; t < 16; ++t){
        double somme = 0.0;
        for(int i = 0; i < 4; ++i){
            double I_i_s = GET(this->underlyings_[i]->price_, GET_INT(this->indices_dates_constatation_, t));
            double I0 = GET(this->valeurs_initiales_, i);
            somme += I_i_s/ I0 - 1;
        }
        perf_moy_panier += MAX(somme/4, 0);
    }

    return perf_moy_panier/16;
}

void Ocelia::compute_perfs_n_ans(PnlVect *perfs, int N){
    compute_valeurs_n_ans(this->valeurs_n_ans_, N);
    for(int i = 0; i < this->size_; ++i){
        // on prend le price au lieu de price converted car on calcule la performance
        LET(perfs, i) = GET(this->valeurs_n_ans_, i)/GET(this->nouveau_depart_, i) - 1;
    }
}

void Ocelia::init_nouveau_depart(){
    this->nouveau_depart_ = pnl_vect_create(4);

    for(int i = 0; i < this->size_; ++i){
        LET(this->nouveau_depart_, i) = GET(this->valeurs_initiales_, i);
    }
}

void Ocelia::compute_nouveau_depart(){
    compute_valeurs_n_ans(this->valeurs_initiales_, 0);
    compute_valeurs_n_ans(this->valeurs_n_ans_, 1);

    compute_perfs_n_ans(this->perfs_, 1);

    if(pnl_vect_max(this->perfs_) <= 0.9){
        pnl_vect_mult_scalar(this->nouveau_depart_, 0.9);
    }
}

double Ocelia::compute_flux_n_ans(int n){
    if(n >= 4 && n <= 7){
        return 1.24 + (n - 4) * 0.08;
    }
    else if(n == 8){
        // TODO: mettre le perf dans un attribut
        compute_perfs_n_ans(this->perfs_, 8);
        double perf_moy = compute_perf_moyenne_panier();
        if(are_all_perfs_positive(this->perfs_)){
            return MAX(1.56, perf_moy);
        }
        return perf_moy;
    }
    return 0;
}

bool Ocelia::are_all_perfs_positive(PnlVect *perfs){
    return pnl_vect_min(perfs) > 0;
}

double Ocelia::payoff()
{
    // TODO: ajouter en [out] un indice de l'année quand le produit s'exerce
    double val_liquidative_initiale = 100.0;
    // 1. valeurs initiale
    compute_valeurs_n_ans(this->valeurs_initiales_, 0);
    // 2. on met valeur init dans valeur depart
    init_nouveau_depart();
    // 4. calcul nouveauDepart
    compute_nouveau_depart();
    
    for(int n = 4 ; n <= 8; ++n){
        compute_perfs_n_ans(this->perfs_, n);
        if(are_all_perfs_positive(this->perfs_)){
            return val_liquidative_initiale*compute_flux_n_ans(n);
        }
    }

    return -1;
}

double Ocelia::shifted_payoff() const
{
    return 0.;
}