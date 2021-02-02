#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"

class MathLib {
  public:
      static PnlMat* log_return(PnlMat *market_data);
      static PnlVect* mean(PnlMat *log_returns);
      static double compute_covariance(PnlMat *log_returns, PnlVect *means, int k, int l);
      static PnlMat* compute_covariance(PnlMat *market_data);
      static void compute_sigma_volatility(PnlMat *covariance, PnlMat *sigma, PnlVect *volatility);

};