#include "libs/MathLib.h"
#include "pnl/pnl_mathtools.h"

PnlMat* MathLib::log_return(PnlMat *market_data){
    PnlMat *log_returns = pnl_mat_create(market_data->m-1, market_data->n);
    for(int i = 0; i < market_data->m-1; ++i){
        for(int j = 0; j < market_data->n; ++j){
            MLET(log_returns, i, j) = log(MGET(market_data, i+1, j) / MGET(market_data, i, j));
        }
    }
    return log_returns;
}

PnlVect* MathLib::mean(PnlMat *log_returns){
    PnlVect *means = pnl_vect_create(log_returns->n);
    pnl_mat_sum_vect(means, log_returns, 'r');
    pnl_vect_div_scalar(means, log_returns->m);
    return means;
}

double MathLib::compute_covariance(PnlMat *log_returns, PnlVect *means, int k, int l){
    double sum = 0;
    double mean_k = GET(means, k);
    double mean_l = GET(means, l);
    for(int i = 0; i < log_returns->m; ++i){
        sum += (MGET(log_returns, i, k) - mean_k) * (MGET(log_returns, i, l) - mean_l);
    }

    return sum / (log_returns->m-1);
}

PnlMat* MathLib::compute_covariance(PnlMat *market_data){
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

    pnl_mat_mult_scalar(covariances, 250); // TODO: 250: nb de jours ouvrés TODO: changer pour mettre un taux variable basé sur les dim de market data

    pnl_mat_free(&log_returns);
    pnl_vect_free(&means);
    return covariances;
}

// TODO: maybe separate sigma and volatility into two functions
void MathLib::compute_sigma_volatility(PnlMat *market_data, PnlMat *sigma, PnlVect *volatility){
    PnlMat *covariance = compute_covariance(market_data);
    int size = covariance->n;
    pnl_mat_clone(sigma, covariance);
    pnl_mat_chol(sigma);
    pnl_vect_resize(volatility, size);
    PnlVect *tmp = pnl_vect_create(size);
    // TODO: mettre le adjust en générique
    MathLib::adjust_sigma_Ocelia(sigma);
    for(int i = 0; i < size; ++i){
        pnl_mat_get_row(tmp, sigma, i);
        double sigma_d = pnl_vect_norm_two(tmp);
        LET(volatility, i) = sigma_d;
    }

    pnl_vect_free(&tmp);
    pnl_mat_free(&covariance);
}


// FONCTION QUI SERT A CALCULER LE SIGMA AVEC LES ZERO COUPONS 
// TODO: généraliser et facto
void MathLib::adjust_sigma_Ocelia(PnlMat *sigma){
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < sigma->n; ++j){
            MLET(sigma, i, j) += MGET(sigma, i+4, j);
        }
    }
}

// TODO: factooooooo
void MathLib::adjust_spot_Ocelia(PnlVect *spot){
    for(int i = 0; i < 3; ++i){
        LET(spot, i) = GET(spot, i)*GET(spot, i+4);
    }
}