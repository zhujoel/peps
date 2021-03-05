#include "libs/MathLib.h"
#include "pnl/pnl_mathtools.h"

void log_returns(PnlMat *log_returns, const PnlMat *path, int start, int end){ // start and end included
    int nbDates = 1+end-start; // +1 to include index end
    pnl_mat_resize(log_returns, nbDates-1, path->n);
    for(int i = 0; i < nbDates-1; ++i){
        for(int j = 0; j < path->n; ++j){
            MLET(log_returns, i, j) = log(MGET(path, i+start+1, j) / MGET(path, i+start, j));
        }
    }
}

void means(PnlVect *means, const PnlMat *path, int start, int end){
    pnl_vect_resize(means, path->n);
    PnlMat sub_path = pnl_mat_wrap_mat_rows(path, start, end);
    pnl_mat_sum_vect(means, &sub_path, 'r');
    pnl_vect_div_scalar(means, 1+end-start);
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

void compute_covariances(PnlMat *covariances, const PnlMat *path, int start, int end){
    PnlMat *returns = pnl_mat_new();
    PnlVect *means_returns = pnl_vect_new();
    log_returns(returns, path, start, end);
    means(means_returns, returns, start, end-1);
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

    // 260: nb de jours ouvrés en une année
    pnl_mat_mult_scalar(covariances, 260);

    pnl_mat_free(&returns);
    pnl_vect_free(&means_returns);
}

void compute_sigma(PnlMat *sigma, const PnlMat *path, int start, int end){
    compute_covariances(sigma, path, start, end);
    pnl_mat_chol(sigma);
}

void compute_volatility(PnlVect *volatility, const PnlMat *sigma){
    int size = sigma->n;
    pnl_vect_resize(volatility, size);
    PnlVect *tmp = pnl_vect_create(size);
    for(int i = 0; i < size; ++i){
        pnl_mat_get_row(tmp, sigma, i);
        LET(volatility, i) = pnl_vect_norm_two(tmp);
    }
    pnl_vect_free(&tmp);
}