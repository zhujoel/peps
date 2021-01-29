#include <iostream>
#include "pnl/pnl_vector.h"
#include "QuantoOption.h"
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

// void print_path(IUnderlying* und, DateTimeVector *dates){
//     for(int i = 0; i < dates->nbDates_; ++i){
//         std::cout << (*dates)[i] << " : price: " << GET(und->price_, i) << std::endl;
//     }
// }

// void print_all_paths(IUnderlying** unds, DateTimeVector *dates){
//     for(int i = 0; i < dates->nbDates_; ++i){
//         std::cout << (*dates)[i] << " : ";
//         for(int j = 0; j < 4; ++j){
//             std::cout << GET(unds[j]->price_, i) << " | ";
//         }
//         std::cout << std::endl;
//     }
// }

// void underlying_test()
// {
//     // ***** DONNEES *****
//     DateTimeVector *all_dates = new DateTimeVector("../data/all_dates", 3288);
//     IMarketData *marketData = new SimulatedMarketData(all_dates);
//     IUnderlying **underlyings = marketData->getMarketdata(4);
//     Ocelia *ocelia = new Ocelia(1, 3288, 4, underlyings, all_dates);
//     PnlMat *sigma = pnl_mat_create(4, 4);
//     BlackScholesModel *blackscholes = new BlackScholesModel(ocelia, sigma);
//     blackscholes->simulateMarket(4);

//     // ***** AFFICHAGE *****
//     print_all_paths(underlyings, all_dates);
//     std::cout << ocelia->payoff() << std::endl;
// }

// void bs_test(){
//     double rd = 0.05;
//     double T = 1;
//     double nbTimeSteps = 365;
//     double size = 7;
//     PnlMat *path = pnl_mat_create(nbTimeSteps+1, size);
//     PnlVect *spot = pnl_vect_create(size);
//     LET(spot, 0) = 100;
//     LET(spot, 1) = 100;
//     LET(spot, 2) = 100;
//     LET(spot, 3) = 100;
//     LET(spot, 4) = 1;
//     LET(spot, 5) = 1;
//     LET(spot, 6) = 1;
//     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
//     pnl_rng_sseed(rng, std::time(NULL));


//     BlackScholesModel *bs = new BlackScholesModel();
//     bs->asset(path, sigma, volatility, rd, T, nbTimeSteps, spot, rng);

//     pnl_mat_print(path);
// }

int main(){
    // underlying_test();
    // bs_test();
}