#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"

class MathLib {
  public:
      // TODO: clean les doubles fonctions cause i don't think its necessary
      static PnlMat* log_return(PnlMat *market_data); // compute la log return des prix sur toute la durée
      static PnlMat* log_return(PnlMat *market_data, int start, int end); // compute la log return entre deux dates (start et end sont des indices)
      static PnlVect* mean(PnlMat *log_returns);
      static double compute_covariance(PnlMat *log_returns, PnlVect *means, int k, int l); // compute covariance between index k and l
      static PnlMat* compute_covariance(PnlMat *market_data);
      static PnlMat* compute_covariance(PnlMat *market_data, int start, int end); // compute covariance entre deux dates
      // TODO: remove compute_sigma_volatility
      static void compute_sigma_volatility(PnlMat *market_data, PnlMat *sigma, PnlVect *volatility);
      static void compute_sigma(PnlMat *market_data, PnlMat *sigma);
      static void compute_sigma(PnlMat *market_data, PnlMat *sigma, int start, int end);
      static void compute_volatility(PnlMat *sigma, PnlVect *volatility);
};