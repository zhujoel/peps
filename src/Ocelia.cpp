#include "Ocelia.h"
#include "pnl/pnl_mathtools.h"
#include <iostream>
#include <fstream>
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
    this->valeurs_initiales_ = pnl_vect_create(4);
}

Ocelia::~Ocelia(){

}

void Ocelia::fill_dates_from_file(string fileName, int nbDates) {
    ifstream inputFileStream(fileName);
    string jour, mois, annee;
    string line;
    for (int i = 0; i < nbDates; i++)
    {
        getline(inputFileStream, line);
        istringstream lineStream(line);
        getline(lineStream, jour, ',');
        getline(lineStream, mois, ',');
        getline(lineStream, annee, ',');
        this->dates_constatation_perf_[i] = new DateTime(stoi(jour), stoi(mois), stoi(annee));
    }
}

void Ocelia::fill_dates_perf(){
    this->fill_dates_from_file("dates_const_semestrielles.txt", 16);
}

void Ocelia::fill_dates_valeurs(){   
    this->fill_dates_from_file("dates_valeurs_N_ans.txt", 35);
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
void Ocelia::compute_valeurs_initiales(){
    // convention : tous les prix sont converti en domestique
    pnl_vect_set_zero(this->valeurs_initiales_);
    for(int t = 0; t < 5; ++t){
        for(int i = 0; i < this->valeurs_initiales_->size; ++i){
            double S_T = GET(this->underlyings_[i]->price_, GET_INT(this->indices_dates_valeurs_n_ans_, t));
            double B_T = GET(this->underlyings_[i]->zc_, GET_INT(this->indices_dates_valeurs_n_ans_, t));
            LET(this->valeurs_initiales_, i) += (S_T / B_T);
        }
    }

    // TODO: remplacer les 5 par des variables
    pnl_vect_div_scalar(this->valeurs_initiales_, 5);
}

double Ocelia::compute_perf_moyenne_panier()
{
    double perf_moy_panier = 0.0; // performance moyenne du panier (1.4 pdf)

    for(int t = 0; t < 16; ++t){
        double somme = 0.0;
        for(int i = 0; i < this->valeurs_initiales_->size; ++i){
            double I_i_s = GET(this->underlyings_[i]->price_, GET_INT(this->indices_dates_constatation_, t));
            double I0 = GET(this->valeurs_initiales_, i);
            somme += I_i_s/ I0 - 1;
        }
        perf_moy_panier += MAX(somme/4, 0);
    }

    return perf_moy_panier/16;
}

double Ocelia::payoff() const
{
    return 0.;
}

double Ocelia::shifted_payoff() const
{
    return 0.;
}