#include <iostream>
#include "pnl/pnl_vector.h"
#include <ctime>
#include "BlackScholesModel.h"
#include "pnl/pnl_random.h"
#include "StandardMonteCarloPricer.h"
#include "pnl/pnl_finance.h"
#include "Ocelia.h"
#include "DateTime.h"
#include "DateTimeVector.h"
#include "IMarketData.h"
#include "SimulatedMarketData.h"
#include "MathLib.h"
#include "pnl/pnl_mathtools.h"


void bs_test(){
    double rd = 0.05;
    double T = 1;
    double nbTimeSteps = 365;
    double size = 7;
    PnlMat *path = pnl_mat_create(nbTimeSteps+1, size);
    PnlVect *spot = pnl_vect_create(size);
    LET(spot, 0) = 100;
    LET(spot, 1) = 100;
    LET(spot, 2) = 100;
    LET(spot, 3) = 100;
    LET(spot, 4) = 1;
    LET(spot, 5) = 1;
    LET(spot, 6) = 1;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, std::time(NULL));


    // BlackScholesModel *bs = new BlackScholesModel();
    // bs->asset(path, sigma, volatility, rd, T, nbTimeSteps, spot, rng);

    // pnl_mat_print(path);
}

int main(){
    // bs_test();
}