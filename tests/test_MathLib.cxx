#include "gtest/gtest.h"
#include "libs/MathLib.h"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include "market_data/HistoricalMarketData.h"

// TODO: add more tests
class MathLibTest: public ::testing::Test{
    protected:
        PnlMat *market_data;
        virtual void SetUp(){
            this->market_data = pnl_mat_create(3, 2);
            MLET(this->market_data, 0, 0) = 100;
            MLET(this->market_data, 1, 0) = 105;
            MLET(this->market_data, 2, 0) = 110;
            MLET(this->market_data, 0, 1) = 100;
            MLET(this->market_data, 1, 1) = 110;
            MLET(this->market_data, 2, 1) = 100;
        }

        virtual void TearDown(){
            pnl_mat_free(&this->market_data);
        }
};

TEST_F(MathLibTest, compute_sigma_volatility){
    PnlMat *sigma = pnl_mat_new();
    PnlVect *vol = pnl_vect_new();
    MathLib::compute_sigma_volatility(this->market_data, sigma, vol);

    EXPECT_NEAR(0.025381, MGET(sigma, 0, 0), 0.000001);
    EXPECT_NEAR(0.054092, MGET(sigma, 0, 1), 0.000001);
    EXPECT_NEAR(2.1312, MGET(sigma, 1, 0), 0.000001);
    EXPECT_NEAR(0, MGET(sigma, 1, 1), 0.000001);
    EXPECT_NEAR(0.059751, GET(vol, 0), 0.000001);
    EXPECT_NEAR(2.1312, GET(vol, 1), 0.000001);

    pnl_mat_free(&sigma);
    pnl_vect_free(&vol);
}

TEST_F(MathLibTest, compute_sigma_volatility_for_market_data){
    HistoricalMarketData *historical = new HistoricalMarketData("Ocelia market data", new DateTime(01, 01, 2003), new DateTime(01, 01, 2013));
    PnlVect *volatility = pnl_vect_new();
    PnlMat *sigma = pnl_mat_new();

    PnlMat *past = historical->getData();

    MathLib::compute_sigma_volatility(past, sigma, volatility);

    std::cout << "sigma: " << std::endl;
    pnl_mat_print(sigma);

    std::cout << "volatility: " << std::endl;
    pnl_vect_print(volatility);

    pnl_mat_free(&sigma);
    pnl_vect_free(&volatility);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
