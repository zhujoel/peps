#include "Ocelia.h"
#include <iostream>

// TODO: paramètres peut etre pas tous nécessaires ?
// Ocelia::Ocelia(double T, int nbTimeSteps, int size, double r_gbp, double r_chf, double r_jpy, double r_eur) : IDerivative(T, nbTimeSteps, size)
// {
//     this->r_gbp_ = r_gbp;
//     this->r_chf_ = r_chf;
//     this->r_jpy_ = r_jpy;
//     this->r_eur_ = r_eur;
//     // TODO: mettre des variables au lieu de 16 ou 7*5
//     this->dates_constatation_perf_ = new DateTime*[16];
//     this->dates_valeurs_n_ans_ = new DateTime*[7*5];
//     this->indices_dates_constatation_ = pnl_vect_create(16);
//     this->valeurs_initiales_ = pnl_vect_create(4);
// }

// Ocelia::~Ocelia(){

// }

// void Ocelia::fill_dates_perf(){
//     // TODO: opti ça?
//     this->dates_constatation_perf_[0] = new DateTime(14, 11, 2008);
//     this->dates_constatation_perf_[1] = new DateTime(15, 5, 2009);
//     this->dates_constatation_perf_[2] = new DateTime(13, 11, 2009);
//     this->dates_constatation_perf_[3] = new DateTime(14, 5, 2010);
//     this->dates_constatation_perf_[4] = new DateTime(15, 11, 2010);
//     this->dates_constatation_perf_[5] = new DateTime(13, 5, 2011);
//     this->dates_constatation_perf_[6] = new DateTime(15, 11, 2011);
//     this->dates_constatation_perf_[7] = new DateTime(15, 5, 2012);
//     this->dates_constatation_perf_[8] = new DateTime(15, 11, 2012);
//     this->dates_constatation_perf_[9] = new DateTime(15, 5, 2013);
//     this->dates_constatation_perf_[10] = new DateTime(15, 11, 2013);
//     this->dates_constatation_perf_[11] = new DateTime(15, 5, 2014);
//     this->dates_constatation_perf_[12] = new DateTime(14, 11, 2014);
//     this->dates_constatation_perf_[13] = new DateTime(15, 5, 2015);
//     this->dates_constatation_perf_[14] = new DateTime(13, 11, 2015);
//     this->dates_constatation_perf_[15] = new DateTime(28, 4, 2016);
// }

// void Ocelia::fill_dates_valeurs(){
//     // TODO: opti ça ?
//     this->dates_valeurs_n_ans_[0] = new DateTime(15, 5, 2008);
//     this->dates_valeurs_n_ans_[1] = new DateTime(16, 5, 2008);
//     this->dates_valeurs_n_ans_[2] = new DateTime(19, 5, 2008);
//     this->dates_valeurs_n_ans_[3] = new DateTime(20, 5, 2008);
//     this->dates_valeurs_n_ans_[4] = new DateTime(21, 5, 2008);
//     this->dates_valeurs_n_ans_[5] = new DateTime(11, 5, 2009);
//     this->dates_valeurs_n_ans_[6] = new DateTime(12, 5, 2009);
//     this->dates_valeurs_n_ans_[7] = new DateTime(13, 5, 2009);
//     this->dates_valeurs_n_ans_[8] = new DateTime(14, 5, 2009);
//     this->dates_valeurs_n_ans_[9] = new DateTime(15, 5, 2009);
//     this->dates_valeurs_n_ans_[10] = new DateTime(23, 4, 2012);
//     this->dates_valeurs_n_ans_[11] = new DateTime(24, 4, 2012);
//     this->dates_valeurs_n_ans_[12] = new DateTime(25, 4, 2012);
//     this->dates_valeurs_n_ans_[13] = new DateTime(26, 4, 2012);
//     this->dates_valeurs_n_ans_[14] = new DateTime(27, 4, 2012);
//     this->dates_valeurs_n_ans_[15] = new DateTime(23, 4, 2013);
//     this->dates_valeurs_n_ans_[16] = new DateTime(24, 4, 2013);
//     this->dates_valeurs_n_ans_[17] = new DateTime(25, 4, 2013);
//     this->dates_valeurs_n_ans_[18] = new DateTime(26, 4, 2013);
//     this->dates_valeurs_n_ans_[19] = new DateTime(30, 4, 2013);
//     this->dates_valeurs_n_ans_[20] = new DateTime(23, 4, 2014);
//     this->dates_valeurs_n_ans_[21] = new DateTime(24, 4, 2014);
//     this->dates_valeurs_n_ans_[22] = new DateTime(25, 4, 2014);
//     this->dates_valeurs_n_ans_[23] = new DateTime(28, 4, 2014);
//     this->dates_valeurs_n_ans_[24] = new DateTime(30, 4, 2014);
//     this->dates_valeurs_n_ans_[25] = new DateTime(23, 4, 2015);
//     this->dates_valeurs_n_ans_[26] = new DateTime(24, 4, 2015);
//     this->dates_valeurs_n_ans_[27] = new DateTime(27, 4, 2015);
//     this->dates_valeurs_n_ans_[28] = new DateTime(28, 4, 2015);
//     this->dates_valeurs_n_ans_[29] = new DateTime(30, 4, 2015);
//     this->dates_valeurs_n_ans_[30] = new DateTime(22, 4, 2016);
//     this->dates_valeurs_n_ans_[31] = new DateTime(25, 4, 2016);
//     this->dates_valeurs_n_ans_[32] = new DateTime(26, 4, 2016);
//     this->dates_valeurs_n_ans_[33] = new DateTime(27, 4, 2016);
//     this->dates_valeurs_n_ans_[34] = new DateTime(28, 4, 2016);
// }

// // TODO: à virer d'ocelia et mettre qq part d'autre
// void Ocelia::calcul_indices_dates(DateTime **all_dates, DateTime **dates, PnlVect *indices)
// {
//     int cnt = 0;
//     for(int i = 0; i < this->nbTimeSteps_+1; ++i){
//         // TODO: à tester (peut y avoir +1 ou -1 dans les indices)
//         if(all_dates[i] == dates[cnt]){
//             LET(indices, cnt++) = i;

//             if(cnt == indices->size) break;
//         }
//     }
// }

// // TODO: à debugger
// double Ocelia::compute_valeurs_initiales(const PnlMat *path){
//     // convention pour le path (toutes les colonnes sont converti en domestique):
//     // 0 : zc gbp
//     // 1 : zc jpy
//     // 2 : zc chf
//     // 3 : ftse100 (gbp)
//     // 4 : nikkei (jpy)
//     // 5 : smi (chf)
//     // 6 : ftse80R (eur)

//     pnl_vect_set_zero(this->valeurs_initiales_);
//     for(int t = 0; t < 5; ++t){
//         for(int i = 0; i < this->valeurs_initiales_->size-1; ++i){
//             double B_T = MGET(path, i, GET(this->indices_dates_valeurs_n_ans_, t));
//             double S_T = MGET(path, i+3, GET(this->indices_dates_valeurs_n_ans_, t));
//             LET(this->valeurs_initiales_, i) += S_T / B_T;
//         }
//         LET(this->valeurs_initiales_, 3) = MGET(path, 6, GET(this->indices_dates_valeurs_n_ans_, t));
//     }

//     // TODO: remplacer les 5 par des variables
//     pnl_vect_div_scalar(this->valeurs_initiales_, 5);

//     return 0;
// }

// double Ocelia::compute_perf_moyenne_panier(const PnlMat *path)
// {
//     double perf_moy_panier = 0.0; // performance moyenne du panier (1.4 pdf)

//     // for(int t = 0; t < 16; ++t){
//     //     double somme = 0.0;
//     //     for(int i = 0; i < this->valeurs_initiales_->size-1; ++i){
//     //         double B_T = MGET(path, i, GET(this->indices_dates_valeurs_n_ans_, t));
//     //         double S_T = MGET(path, i+3, GET(this->indices_dates_valeurs_n_ans_, t));
//     //         somme += (S_T/B_T)*GET(this->valeurs_initiales_, i) /GET(this->compute_valeurs_initiales_, i);
//     //     }
//     //     perf_moy_panier += MAX(somme/4, 0);
//     // }

//     return perf_moy_panier/16;
// }

// double Ocelia::payoff(const PnlMat *path) const
// {

//     return 0.;
// }