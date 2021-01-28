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

// void bs_test(){
//     double rd = 0.05;
//     double T = 1;
//     double nbTimeSteps = 365;
//     double size = 7;
//     PnlMat *path = pnl_mat_create(nbTimeSteps+1, size);
//     PnlVect *spot = pnl_vect_create(size);
//     LET(spot, 0) = 100;
//     LET(spot, 1) = 100;
//     LET(spot, 2) = 100;
//     LET(spot, 3) = 100;
//     LET(spot, 4) = 1;
//     LET(spot, 5) = 1;
//     LET(spot, 6) = 1;
//     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
//     pnl_rng_sseed(rng, std::time(NULL));


//     BlackScholesModel *bs = new BlackScholesModel();
//     bs->asset(path, sigma, volatility, rd, T, nbTimeSteps, spot, rng);

//     pnl_mat_print(path);
// }

int main(){
    // underlying_test();
    // bs_test();
    // sigma_vol_test();
}