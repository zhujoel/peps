#include "Ocelia.h"
#include <iostream>

// TODO: paramètres peut etre pas tous nécessaires ?
Ocelia::Ocelia(double T, int nbTimeSteps, int size, double r_gbp, double r_chf, double r_jpy, double r_eur) : IDerivative(T, nbTimeSteps, size)
{
    this->r_gbp_ = r_gbp;
    this->r_chf_ = r_chf;
    this->r_jpy_ = r_jpy;
    this->r_eur_ = r_eur;
    this->dates_constatation_perf_ = new DateTime*[16];
    this->dates_valeurs_n_ans = new DateTime*[7*5];
}

Ocelia::~Ocelia(){

}

void Ocelia::fill_dates_perf(){
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
}

void Ocelia::fill_dates_valeurs(){
    // TODO: opti ça ?
    this->dates_valeurs_n_ans[0] = new DateTime(15, 5, 2008);
    this->dates_valeurs_n_ans[1] = new DateTime(16, 5, 2008);
    this->dates_valeurs_n_ans[2] = new DateTime(19, 5, 2008);
    this->dates_valeurs_n_ans[3] = new DateTime(20, 5, 2008);
    this->dates_valeurs_n_ans[4] = new DateTime(21, 5, 2008);
    this->dates_valeurs_n_ans[5] = new DateTime(11, 5, 2009);
    this->dates_valeurs_n_ans[6] = new DateTime(12, 5, 2009);
    this->dates_valeurs_n_ans[7] = new DateTime(13, 5, 2009);
    this->dates_valeurs_n_ans[8] = new DateTime(14, 5, 2009);
    this->dates_valeurs_n_ans[9] = new DateTime(15, 5, 2009);
    this->dates_valeurs_n_ans[10] = new DateTime(23, 4, 2012);
    this->dates_valeurs_n_ans[11] = new DateTime(24, 4, 2012);
    this->dates_valeurs_n_ans[12] = new DateTime(25, 4, 2012);
    this->dates_valeurs_n_ans[13] = new DateTime(26, 4, 2012);
    this->dates_valeurs_n_ans[14] = new DateTime(27, 4, 2012);
    this->dates_valeurs_n_ans[15] = new DateTime(23, 4, 2013);
    this->dates_valeurs_n_ans[16] = new DateTime(24, 4, 2013);
    this->dates_valeurs_n_ans[17] = new DateTime(25, 4, 2013);
    this->dates_valeurs_n_ans[18] = new DateTime(26, 4, 2013);
    this->dates_valeurs_n_ans[19] = new DateTime(30, 4, 2013);
    this->dates_valeurs_n_ans[20] = new DateTime(23, 4, 2014);
    this->dates_valeurs_n_ans[21] = new DateTime(24, 4, 2014);
    this->dates_valeurs_n_ans[22] = new DateTime(25, 4, 2014);
    this->dates_valeurs_n_ans[23] = new DateTime(28, 4, 2014);
    this->dates_valeurs_n_ans[24] = new DateTime(30, 4, 2014);
    this->dates_valeurs_n_ans[25] = new DateTime(23, 4, 2015);
    this->dates_valeurs_n_ans[26] = new DateTime(24, 4, 2015);
    this->dates_valeurs_n_ans[27] = new DateTime(27, 4, 2015);
    this->dates_valeurs_n_ans[28] = new DateTime(28, 4, 2015);
    this->dates_valeurs_n_ans[29] = new DateTime(30, 4, 2015);
    this->dates_valeurs_n_ans[30] = new DateTime(22, 4, 2016);
    this->dates_valeurs_n_ans[31] = new DateTime(25, 4, 2016);
    this->dates_valeurs_n_ans[32] = new DateTime(26, 4, 2016);
    this->dates_valeurs_n_ans[33] = new DateTime(27, 4, 2016);
    this->dates_valeurs_n_ans[34] = new DateTime(28, 4, 2016);
}

double Ocelia::compute_perf_moyenne_panier(const PnlMat *path){
    double perf_moy_panier = 0.0; // performance moyenne du panier (1.4 pdf)
    return perf_moy_panier;
}

double Ocelia::payoff(const PnlMat *path) const
{

    return 0.;
}