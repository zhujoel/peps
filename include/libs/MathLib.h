#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"

void log_returns(const PnlMat *path, PnlMat *log_returns, int start, int end); // compute la log return entre deux dates (start et end sont des indices)
void means(const PnlMat *path, PnlVect *means);
double compute_covariance(const PnlMat *log_returns, const PnlVect *means, int k, int l); // compute covariance between index k and l
void compute_covariances(const PnlMat *path, PnlMat *covariances, int start, int end); // compute covariance entre deux dates
void compute_sigma( PnlMat *sigma, const PnlMat *path, int start, int end);
void compute_volatility(const PnlMat *sigma, PnlVect *volatility);