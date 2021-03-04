#include "libs/MathLib.h"
#include "pnl/pnl_mathtools.h"
#include <iostream>

void log_returns(const PnlMat *path, PnlMat *log_returns, int start, int end){ // start and end included
    int nbDates = 1+end-start; // +1 to include index end
    pnl_mat_resize(log_returns, nbDates-1, path->n);
    for(int i = 0; i < nbDates-1; ++i){
        for(int j = 0; j < path->n; ++j){
            MLET(log_returns, i, j) = log(MGET(path, i+start+1, j) / MGET(path, i+start, j));
        }
    }
}

void means(const PnlMat *path, PnlVect *means){ // TODO faire que la moyenne sur start et end inclus
    pnl_vect_resize(means, path->n);
    pnl_mat_sum_vect(means, path, 'r');
    pnl_vect_div_scalar(means, path->m);
}

double compute_covariance(const PnlMat *log_returns, const PnlVect *means, int k, int l){
    double sum = 0;
    double mean_k = GET(means, k);
    double mean_l = GET(means, l);
    for(int i = 0; i < log_returns->m; ++i){
        sum += (MGET(log_returns, i, k) - mean_k) * (MGET(log_returns, i, l) - mean_l);
    }
    return sum / (log_returns->m-1);
}

void compute_covariances(const PnlMat *path, PnlMat *covariances, int start, int end){
    PnlMat *returns = pnl_mat_new();
    PnlVect *means_returns = pnl_vect_new();
    log_returns(path, returns, start, end);
    means(returns, means_returns);
    int n = path->n;
    pnl_mat_resize(covariances, n, n);

    for(int i = 0; i < n; ++i){
        MLET(covariances, i, i) = compute_covariance(returns, means_returns, i, i);
        for(int j = 0; j < i; ++j){
            double covar = compute_covariance(returns, means_returns, i, j);
            MLET(covariances, i, j) = covar;
            MLET(covariances, j, i) = covar;
        }
    }

    pnl_mat_mult_scalar(covariances, 250); // TODO: 250: nb de jours ouvrés TODO: changer pour mettre un taux variable basé sur les dim de market data

    pnl_mat_free(&returns);
    pnl_vect_free(&means_returns);
}

void compute_sigma(const PnlMat *path, PnlMat *sigma, int start, int end){
    compute_covariances(path, sigma, start, end);
    pnl_mat_chol(sigma);
}

void compute_volatility(const PnlMat *sigma, PnlVect *volatility){
    int size = sigma->n;
    pnl_vect_resize(volatility, size);
    PnlVect *tmp = pnl_vect_create(size);
    for(int i = 0; i < size; ++i){
        pnl_mat_get_row(tmp, sigma, i);
        LET(volatility, i) = pnl_vect_norm_two(tmp);
    }

    pnl_vect_free(&tmp);
}