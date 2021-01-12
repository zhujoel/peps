#include "Ocelia.h"
#include "pnl/pnl_mathtools.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using  namespace std;

// TODO: paramètres peut etre pas tous nécessaires ?
Ocelia::Ocelia(double T, int nbTimeSteps, int size, double r_gbp, double r_chf, double r_jpy, double r_eur, IUnderlying **underlyings) : IDerivative(T, nbTimeSteps, size, r_eur, underlyings)
{
    this->r_gbp_ = r_gbp;
    this->r_chf_ = r_chf;
    this->r_jpy_ = r_jpy;
    // TODO: mettre des variables au lieu de 16 ou 7*5
    this->dates_constatation_perf_ = new DateTime*[16];
    this->dates_valeurs_n_ans_ = new DateTime*[7*5];
    this->indices_dates_constatation_ = pnl_vect_int_create(16);
    this->valeurs_n_ans_ = pnl_vect_create(4);
    this->valeurs_initiales_ = pnl_vect_create(4);
}

Ocelia::~Ocelia(){

}

void Ocelia::fill_dates_perf(){
    ifstream inputFileStream("dates.txt");
    string jour, mois, annee;
    string line;
    for (int i = 0; i < 16; i++)
    {
        getline(inputFileStream, line);
        istringstream lineStream(line);
        getline(lineStream, jour, ',');
        getline(lineStream, mois, ',');
        getline(lineStream, annee, ',');
        this->dates_constatation_perf_[i] = new DateTime(stoi(jour), stoi(mois), stoi(annee));
    }
    /*
    // TODO: opti ça?
    this->dates_constatation_perf_[0] = new DateTime(14, 11, 2008);
    this->dates_constatation_perf_[1] = new DateTime(15, 5, 2009);
    this->dates_constatation_perf_[2] = new DateTime(13, 11, 2009);
    this->dates_constatation_perf_[3] = new DateTime(14, 5, 2010);
    this->dates_constatation_perf_[4] = new DateTime(15, 11, 2010);
    this->dates_constatation_perf_[5] = new DateTime(13, 5, 2011);
    this->dates_constatation_perf_[6] = new DateTime(15, 11, 2011);
    this->dates_constatation_perf_[7] = new DateTime(15, 5, 2012);
    this->dates_constatation_perf_[8] = new DateTime(15, 11, 2012);
    this->dates_constatation_perf_[9] = new DateTime(15, 5, 2013);
    this->dates_constatation_perf_[10] = new DateTime(15, 11, 2013);
    this->dates_constatation_perf_[11] = new DateTime(15, 5, 2014);
    this->dates_constatation_perf_[12] = new DateTime(14, 11, 2014);
    this->dates_constatation_perf_[13] = new DateTime(15, 5, 2015);
    this->dates_constatation_perf_[14] = new DateTime(13, 11, 2015);
    this->dates_constatation_perf_[15] = new DateTime(28, 4, 2016);
    */
}

void Ocelia::fill_dates_valeurs(){
    ifstream inputFileStream("dates2.txt");
    string jour, mois, annee;
    string line;
    for (int i = 0; i < 35; i++)
    {
        getline(inputFileStream, line);
        istringstream lineStream(line);
        getline(lineStream, jour, ',');
        getline(lineStream, mois, ',');
        getline(lineStream, annee, ',');
        this->dates_constatation_perf_[i] = new DateTime(stoi(jour), stoi(mois), stoi(annee));
    }
    /*
    // TODO: opti ça ?
    this->dates_valeurs_n_ans_[0] = new DateTime(15, 5, 2008);
    this->dates_valeurs_n_ans_[1] = new DateTime(16, 5, 2008);
    this->dates_valeurs_n_ans_[2] = new DateTime(19, 5, 2008);
    this->dates_valeurs_n_ans_[3] = new DateTime(20, 5, 2008);
    this->dates_valeurs_n_ans_[4] = new DateTime(21, 5, 2008);
    this->dates_valeurs_n_ans_[5] = new DateTime(11, 5, 2009);
    this->dates_valeurs_n_ans_[6] = new DateTime(12, 5, 2009);
    this->dates_valeurs_n_ans_[7] = new DateTime(13, 5, 2009);
    this->dates_valeurs_n_ans_[8] = new DateTime(14, 5, 2009);
    this->dates_valeurs_n_ans_[9] = new DateTime(15, 5, 2009);
    this->dates_valeurs_n_ans_[10] = new DateTime(23, 4, 2012);
    this->dates_valeurs_n_ans_[11] = new DateTime(24, 4, 2012);
    this->dates_valeurs_n_ans_[12] = new DateTime(25, 4, 2012);
    this->dates_valeurs_n_ans_[13] = new DateTime(26, 4, 2012);
    this->dates_valeurs_n_ans_[14] = new DateTime(27, 4, 2012);
    this->dates_valeurs_n_ans_[15] = new DateTime(23, 4, 2013);
    this->dates_valeurs_n_ans_[16] = new DateTime(24, 4, 2013);
    this->dates_valeurs_n_ans_[17] = new DateTime(25, 4, 2013);
    this->dates_valeurs_n_ans_[18] = new DateTime(26, 4, 2013);
    this->dates_valeurs_n_ans_[19] = new DateTime(30, 4, 2013);
    this->dates_valeurs_n_ans_[20] = new DateTime(23, 4, 2014);
    this->dates_valeurs_n_ans_[21] = new DateTime(24, 4, 2014);
    this->dates_valeurs_n_ans_[22] = new DateTime(25, 4, 2014);
    this->dates_valeurs_n_ans_[23] = new DateTime(28, 4, 2014);
    this->dates_valeurs_n_ans_[24] = new DateTime(30, 4, 2014);
    this->dates_valeurs_n_ans_[25] = new DateTime(23, 4, 2015);
    this->dates_valeurs_n_ans_[26] = new DateTime(24, 4, 2015);
    this->dates_valeurs_n_ans_[27] = new DateTime(27, 4, 2015);
    this->dates_valeurs_n_ans_[28] = new DateTime(28, 4, 2015);
    this->dates_valeurs_n_ans_[29] = new DateTime(30, 4, 2015);
    this->dates_valeurs_n_ans_[30] = new DateTime(22, 4, 2016);
    this->dates_valeurs_n_ans_[31] = new DateTime(25, 4, 2016);
    this->dates_valeurs_n_ans_[32] = new DateTime(26, 4, 2016);
    this->dates_valeurs_n_ans_[33] = new DateTime(27, 4, 2016);
    this->dates_valeurs_n_ans_[34] = new DateTime(28, 4, 2016);
    */
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
void Ocelia::compute_valeurs_n_ans(PnlVect *valeurs, int n)
{
    // TODO: faut pas qu'il y ait de 2 ou 3
    if(n > 1){
        n -= 2;
    }
    // convention : tous les prix sont converti en domestique
    pnl_vect_set_zero(valeurs);
    for(int t = 0; t < 5; ++t){
        for(int i = 0; i < this->size_; ++i){
            double S_T = GET(this->underlyings_[i]->price_converted_, GET_INT(this->indices_dates_valeurs_n_ans_, t+(n*5)));
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
    pnl_vect_resize(perfs, 4);
    compute_valeurs_n_ans(this->valeurs_initiales_, 0);
    for(int i = 0; i < this->size_; ++i){
        // on prend le price au lieu de price converted car on calcule la performance
        double S_T = GET(this->underlyings_[i]->price_, GET_INT(this->indices_dates_valeurs_n_ans_, t+(n*5)));
        LET(perfs, i) = S_T/GET(this->valeurs_departs_, i) - 1;
    }
}

void Ocelia::init_valeurs_departs(){
    this->valeurs_departs_ = pnl_vect_create(4);

    for(int i = 0; i < this->size_; ++i){
        LET(this->valeurs_departs_, i) = this->valeurs_initiales_;
    }
}

void Ocelia::compute_valeurs_departs(){
    compute_valeurs_n_ans(this->valeurs_initiales_, 0);
    compute_valeurs_n_ans(this->valeurs_n_ans_, 1);

    PnlVect *perf_1_an = pnl_vect_create(4);
    compute_perfs_n_ans(perf_1_an, 1);

    bool flag = true; // devient faux s'il y a une valeur > 0.9
    for(int i = 0; i < this->size_; ++i){
        if(GET(this->perfs, i) > 0.9){
            flag = false;
        }
    }

    if(flag){
        pnl_vect_mult_scalar(this->valeurs_departs_, 0.9);
    }
}

double Ocelia::compute_flux_n_ans(int n){
    if(n >= 4 && n <= 7){
        return 1.24 + (n - 4) * 0.08;
    }
    else if(n == 8){
        // TODO: mettre le perf dans un attribut
        PnlVect *perf_8_ans = pnl_vect_create(4);
        compute_perfs_n_ans(perf_8_ans, 8);
        double perf_moy = compute_perf_moyenne_panier();
        if(are_all_perfs_positive(perf_8_ans){
            return MAX(1.56, perf_moy);
        }
        return perf_moy;
    }
    return 0;
}

bool Ocelia::are_all_perfs_positive(PnlVect *perfs){
    bool flag = true; // devient faux s'il y a une valeur positive
    for(int i = 0; i < this->size_; ++i){
        if(GET(this->perfs, i) < 0){
            flag = false;
        }
    }
    return flag;
}

double Ocelia::payoff() const
{
    // 1. valeurs initiale
    compute_valeurs_n_ans(this->valeurs_initiales_, 0);
    // 2. on met valeur init dans valeur depart
    init_valeurs_departs();
    // 4. calcul nouveauDepart
    compute_valeurs_departs();



    return 0.;
}

double Ocelia::shifted_payoff() const
{
    return 0.;
}