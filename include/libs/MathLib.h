#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"

void log_returns(PnlMat * const log_returns, const PnlMat * const path, int start, int end); // compute la log return entre deux dates (start et end sont des indices)
void means(PnlVect * const means, const PnlMat * const path);
double compute_covariance(const PnlMat * const log_returns, const PnlVect * const means, int k, int l); // compute covariance between index k and l
void compute_covariances(PnlMat * const covariances, const PnlMat * const path, int start, int end); // compute covariance entre deux dates
void compute_sigma(PnlMat * const sigma, const PnlMat * const path, int start, int end);
void compute_volatility(PnlVect * const volatility, const PnlMat * const sigma);