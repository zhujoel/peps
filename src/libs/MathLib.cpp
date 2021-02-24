#include "libs/MathLib.h"
#include "pnl/pnl_mathtools.h"

PnlMat* log_returns(PnlMat *path, int start, int end){
    int nbDates = end-start;
    PnlMat *log_returns = pnl_mat_create(nbDates-1, path->n);
    for(int i = 0; i < nbDates-1; ++i){
        for(int j = 0; j < path->n; ++j){
            MLET(log_returns, i+start, j) = log(MGET(path, i+start+1, j) / MGET(path, i+start, j));
        }
    }
    return log_returns;
}

PnlVect* means(PnlMat *path){
    PnlVect *means = pnl_vect_create(path->n);
    pnl_mat_sum_vect(means, path, 'r');
    pnl_vect_div_scalar(means, path->m);
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

PnlMat* compute_covariance(PnlMat *path, int start, int end){
    PnlMat *returns = log_returns(path, start, end);
    PnlVect *means_returns = means(returns);
    int n = path->n;
    PnlMat *covariances = pnl_mat_create(n, n);

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
    return covariances;
}

PnlMat* compute_sigma(PnlMat *path, int start, int end){
    PnlMat *covariance = compute_covariance(path, start, end);
    pnl_mat_chol(covariance);
    return covariance;
}

PnlVect* compute_volatility(PnlMat *sigma){
    int size = sigma->n;
    PnlVect *volatility = pnl_vect_create(size);
    PnlVect *tmp = pnl_vect_create(size);
    for(int i = 0; i < size; ++i){
        pnl_mat_get_row(tmp, sigma, i);
        LET(volatility, i) = pnl_vect_norm_two(tmp);
    }

    pnl_vect_free(&tmp);
    return volatility;
}