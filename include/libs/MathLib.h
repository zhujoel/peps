#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"

PnlMat* log_returns(PnlMat *path, int start, int end); // compute la log return entre deux dates (start et end sont des indices)
PnlVect* means(PnlMat *path);
double compute_covariance(PnlMat *log_returns, PnlVect *means, int k, int l); // compute covariance between index k and l
PnlMat* compute_covariance(PnlMat *path, int start, int end); // compute covariance entre deux dates
PnlMat* compute_sigma(PnlMat *path, int start, int end);
PnlVect* compute_volatility(PnlMat *sigma);