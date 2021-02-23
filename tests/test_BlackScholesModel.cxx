#include "gtest/gtest.h"
#include "libs/MathLib.h"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include "models/BlackScholesModel.h"
#include "market_data/HistoricalMarketData.h"

// TODO: add more tests
class BlackScholesModelTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

// TEST_F(BlackScholesModelTest, asset){
//     HistoricalMarketData *historical = new HistoricalMarketData("Ocelia market data", new DateTime(01, 01, 2003), new DateTime(01, 01, 2013));
//     PnlVect *volatility = pnl_vect_new();
//     PnlMat *sigma = pnl_mat_new();

//     PnlMat *past = historical->getData(NULL);

//     MathLib::compute_sigma_volatility(past, sigma, volatility);

//     pnl_mat_print(sigma);
//     pnl_vect_print(volatility);

//     int size = 7;
//     double rd = 0;
//     int nbTimeSteps = 10000;
//     double T = 10000/250;
//     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
//     pnl_rng_sseed(rng, std::time(NULL));
//     PnlVect *spot = pnl_vect_new();
//     pnl_mat_get_row(spot, past, (past->n)-1);
//     BlackScholesModel *bs = new BlackScholesModel(size, rd, sigma, volatility, spot);

//     PnlMat *path = pnl_mat_create(nbTimeSteps+1, 7);

//     bs->asset(path, T, nbTimeSteps, rng);

//     // pnl_mat_print(path);

//     PnlVect *bs_volatility = pnl_vect_new();
//     PnlMat *bs_sigma = pnl_mat_new();
//     MathLib::compute_sigma_volatility(path, bs_sigma, bs_volatility);

//     pnl_mat_print(bs_sigma);
//     pnl_vect_print(bs_volatility);

//     pnl_mat_free(&sigma);
//     pnl_vect_free(&volatility);

//     // TODO: mettre des asserts (near) sur sigma et vol
//     EXPECT_EQ(1, 1);
// }



int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
