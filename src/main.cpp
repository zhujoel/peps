#include <iostream>
#include "pnl/pnl_vector.h"
#include "QuantoOption.h"
#include "BlackScholesModel.h"
#include <ctime>
#include "pnl/pnl_random.h"
#include "StandardMonteCarloPricer.h"
#include "pnl/pnl_finance.h"
#include "Ocelia.h"
#include "DateTime.h"
#include "DateTimeVector.h"
#include "IUnderlying.h"
#include "ForeignUnderlying.h"
#include "IMarketData.h"
#include "SimulatedMarketData.h"
#include "pnl/pnl_mathtools.h"

// TODO: voir comment générer les .dll 
// TODO: Gestion des données
// TODO: ajouter classe taux d'intéret ?
// TODO: ajouter des classes pour séparer les zc et les actifs risqué dans un pf ?
// TODO: calcul du sigma pour généraliser avec des dimensions > 2
// TODO: pricing en tnctions au bons endroits
// TODO: abstraire pour des taux int
// TODO: mettre des const dans les foérets non constants
// TODO: implémenter des MC + opti ?
// TODO: feeder des données ?

void print_path(IUnderlying* und, DateTimeVector *dates){
    for(int i = 0; i < dates->nbDates_; ++i){
        std::cout << (*dates)[i] << " : price: " << GET(und->price_, i) << std::endl;
    }
}

void print_all_paths(IUnderlying** unds, DateTimeVector *dates){
    for(int i = 0; i < dates->nbDates_; ++i){
        std::cout << (*dates)[i] << " : ";
        for(int j = 0; j < 4; ++j){
            std::cout << GET(unds[j]->price_, i) << " | ";
        }
        std::cout << std::endl;
    }
}

// void underlying_test()
// {
//     // ***** DONNEES *****
//     DateTimeVector *all_dates = new DateTimeVector("../data/all_dates", 3288);
//     IMarketData *marketData = new SimulatedMarketData(all_dates);
//     IUnderlying **underlyings = marketData->getMarketdata(4);
//     Ocelia *ocelia = new Ocelia(1, 3288, 4, underlyings, all_dates);
//     PnlMat *sigma = pnl_mat_create(4, 4);
//     BlackScholesModel *blackscholes = new BlackScholesModel(ocelia, sigma);
//     blackscholes->simulateMarket(4);

//     // ***** AFFICHAGE *****
//     print_all_paths(underlyings, all_dates);
//     std::cout << ocelia->payoff() << std::endl;
// }

// void quanto_test(){
// // TEST DE PRICE UNE OPTION QUANTO

//     /** DATA **/
//     double T = 1;
//     double nbTimeSteps = 365;
//     double rf = 0.05;
//     double K = 90.0;
//     double nbProduits = 1;
//     double rd = 0.03; // taux constants pour l'instant
//     double sigma_tx_change = 0.05;
//     double sigma_actif = 0.1;
//     double spot_actif_sans_risque = 1;
//     double spot_actif_risque = 100;
//     double nbSimul = 100;
//     double spot_taux_change_initial = 1.2;
//     double rho = 0.2; // corrélation entre zc et actif risqué étranger
//     double h = 0.01;
//     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
//     pnl_rng_sseed(rng, std::time(NULL));

//     // TODO : ce calcul du sigma ne fonctionnera (enfin il dépend de la taille et du contenu de la matrice du pf de couverture)
//     PnlMat* sigma = pnl_mat_create(nbProduits*2, nbProduits*2); // c'est une matrice de covariance
//     MLET(sigma, 0, 0) = sigma_tx_change * sigma_tx_change;
//     MLET(sigma, 1, 1) = sigma_actif * sigma_actif;
//     MLET(sigma, 0, 1) = sigma_tx_change * sigma_actif * rho;
//     MLET(sigma, 1, 0) = sigma_tx_change * sigma_actif * rho;
//     pnl_mat_chol(sigma);
//     MLET(sigma, 1, 0) += MGET(sigma, 0, 0); // ce calcul dépend de où ce trouve dans la matrice les actifs risqués et non risqués
//     MLET(sigma, 1, 1) += MGET(sigma, 0, 1);

//     PnlVect *spot = pnl_vect_create(2);
//     LET(spot, 0) = spot_actif_sans_risque*spot_taux_change_initial;
//     LET(spot, 1) = spot_actif_risque*spot_taux_change_initial;

//     // prix théorique avec pnl
//     double prix2 = 0.0;
//     double delta2 = 0.0;
//     double sigma_actif_converti = sqrt(sigma_tx_change*sigma_tx_change+sigma_actif*sigma_actif+2*rho*sigma_tx_change*sigma_actif);
//     pnl_cf_call_bs(spot_taux_change_initial*spot_actif_risque, K, T, rd, 0, sigma_actif_converti, &prix2, &delta2);
//     std::cout << "prix théorique : " << prix2 << " delta théorique : " << delta2<< std::endl;

//     // simulation
//     IUnderlying *und = new ForeignUnderlying(GET(spot, 1), GET(spot, 0), nbTimeSteps);
//     IUnderlying **unds = new IUnderlying*[1];
//     unds[0] = und;
    
//     IDerivative *quanto = new QuantoOption(T, nbTimeSteps, nbProduits, rd, rf, K, unds) ;
//     BlackScholesModel *model = new BlackScholesModel(quanto, sigma);
//     IPricer *pricer = new StandardMonteCarloPricer(model, rng, h, nbSimul);

//     double prix = 0.0;
//     double prix_std_dev = 0.0;
//     PnlVect* delta = pnl_vect_create(quanto->size_*2);
//     PnlVect* delta_std_dev = pnl_vect_create(quanto->size_*2);
//     pricer->simulate(prix, prix_std_dev, delta, delta_std_dev);
//     std::cout << "prix simulé : " << prix << " std dev : " << prix_std_dev << std::endl;
//     std::cout << "price est dedans : " << (abs(prix2 - prix) <= 1.96*prix_std_dev) << std::endl;

//     std::cout << "delta simulé : " << std::endl;
//     pnl_vect_print(delta);
//     pnl_vect_print(delta_std_dev);
//     std::cout << "delta est dedans : " << (abs(GET(delta, 1) - delta2) <= 1.96*GET(delta_std_dev, 1)) << std::endl;

//     double delta_zc_sans_risque = spot_actif_risque*delta2 * exp(-rf*0);
//     std::cout << "delta sous-jacent théorique : " << delta_zc_sans_risque << std::endl;

//     // free
//     delete(unds);
//     delete(und);
//     delete(quanto);
//     delete(model);
//     delete(pricer);
//     pnl_mat_free(&sigma);
//     pnl_rng_free(&rng);
//     pnl_vect_free(&spot);
//     pnl_vect_free(&delta);
//     pnl_vect_free(&delta_std_dev);
// }

PnlMat* log_return(PnlMat *market_data){
    PnlMat *log_returns = pnl_mat_create(market_data->m-1, market_data->n);
    for(int i = 0; i < market_data->m-1; ++i){
        for(int j = 0; j < market_data->n; ++j){
            MLET(log_returns, i, j) = log(MGET(market_data, i+1, j) / MGET(market_data, i, j));
        }
    }
    return log_returns;
}

PnlVect* mean(PnlMat *log_returns){
    PnlVect *means = pnl_vect_create(log_returns->n);
    pnl_mat_sum_vect(means, log_returns, 'r');
    pnl_vect_div_scalar(means, log_returns->m);
    return means;
}

double compute_covariance(PnlMat *log_returns, PnlVect *means, int k, int l){
    double sum = 0;
    double mean_k = GET(means, k);
    double mean_l = GET(means, l);
    for(int i = 0; i < log_returns->m; ++i){
        sum += (MGET(log_returns, i, k) - mean_k) * (MGET(log_returns, i, l) - mean_l);
    }

    return sum / (log_returns->m-1);
}

PnlMat* compute_covariance(PnlMat *market_data){
    PnlMat *log_returns = log_return(market_data);
    PnlVect *means = mean(log_returns);
    int n = market_data->n;
    PnlMat *covariances = pnl_mat_create(n, n);

    for(int i = 0; i < n; ++i){
        MLET(covariances, i, i) = compute_covariance(log_returns, means, i, i);
        for(int j = 0; j < i; ++j){
            double covar = compute_covariance(log_returns, means, i, j);
            MLET(covariances, i, j) = covar;
            MLET(covariances, j, i) = covar;
        }
    }

    pnl_mat_mult_scalar(covariances, 250);
    return covariances;
}

void compute_sigma_volatility(PnlMat *covariance, PnlMat *sigma, PnlVect *volatility){
    int size = covariance->n;
    pnl_mat_clone(sigma, covariance);
    pnl_mat_chol(sigma);
    pnl_vect_resize(volatility, size);
    PnlVect *tmp = pnl_vect_create(size);

    for(int i = 0; i < size; ++i){
        pnl_mat_get_row(tmp, sigma, i);
        double sigma_d = pnl_vect_norm_two(tmp);
        LET(volatility, i) = sigma_d;
    }
}

void sigma_vol_test(){
    PnlMat *market_data = pnl_mat_create(3, 2);
    MLET(market_data, 0, 0) = 100;
    MLET(market_data, 1, 0) = 105;
    MLET(market_data, 2, 0) = 110;
    MLET(market_data, 0, 1) = 100;
    MLET(market_data, 1, 1) = 110;
    MLET(market_data, 2, 1) = 100;

    PnlMat *covar = compute_covariance(market_data);

    PnlMat *sigma = pnl_mat_new();
    PnlVect *vol = pnl_vect_new();
    compute_sigma_volatility(covar, sigma, vol);
    pnl_mat_print(sigma);
    pnl_vect_print(vol);
}

void bs_test(){
    double rd = 0.05;
    double T = 1;
    double nbTimeSteps = 365;
    double size = 7;
    PnlMat *path = pnl_mat_create(nbTimeSteps+1, size);
    PnlVect *spot = pnl_vect_create(size);
    LET(spot, 0) = 100;
    LET(spot, 1) = 100;
    LET(spot, 2) = 100;
    LET(spot, 3) = 100;
    LET(spot, 4) = 1;
    LET(spot, 5) = 1;
    LET(spot, 6) = 1;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, std::time(NULL));


    // BlackScholesModel *bs = new BlackScholesModel();
    // bs->asset(path, sigma, volatility, rd, T, nbTimeSteps, spot, rng);

    // pnl_mat_print(path);
}

int main(){
    // quanto_test();
    // underlying_test();
    // bs_test();
    sigma_vol_test();
}