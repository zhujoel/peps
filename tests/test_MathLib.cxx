#include "gtest/gtest.h"
#include "libs/MathLib.h"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include "market_data/HistoricalMarketData.h"

// TODO: add more tests
class MathLibTest: public ::testing::Test{
    protected:
        PnlMat *path;
        virtual void SetUp(){
            this->path = pnl_mat_create(3, 2);
            MLET(this->path, 0, 0) = 100;
            MLET(this->path, 1, 0) = 105;
            MLET(this->path, 2, 0) = 110;
            MLET(this->path, 0, 1) = 100;
            MLET(this->path, 1, 1) = 110;
            MLET(this->path, 2, 1) = 100;
        }

        virtual void TearDown(){
            pnl_mat_free(&this->path);
        }
};

// TEST_F(MathLibTest, log_returns){
//     PnlMat *returns = log_returns(this->path, 0, 3);

//     std::cout << MGET(returns, 0, 0) << std::endl; // 0.0487902 ??

//     EXPECT_NEAR(1.05, MGET(returns, 0, 0), 0.000001);
//     EXPECT_NEAR(1.0476, MGET(returns, 1, 0), 0.000001);
//     EXPECT_NEAR(1.1, MGET(returns, 0, 1), 0.000001);
//     EXPECT_NEAR(0.9091, MGET(returns, 1, 1), 0.000001);

//     pnl_mat_free(&returns);
// }


TEST_F(MathLibTest, compute_sigma){
    PnlMat *sigma = compute_sigma(this->path, 0, 3);

    EXPECT_NEAR(0.025381, MGET(sigma, 0, 0), 0.000001);
    EXPECT_NEAR(0.054092, MGET(sigma, 0, 1), 0.000001);
    EXPECT_NEAR(2.1312, MGET(sigma, 1, 0), 0.000001);
    EXPECT_NEAR(0, MGET(sigma, 1, 1), 0.000001);

    pnl_mat_free(&sigma);
}

TEST_F(MathLibTest, compute_volatility){
    PnlMat *sigma = compute_sigma(this->path, 0, 3);
    PnlVect *volatility = compute_volatility(sigma);

    EXPECT_NEAR(0.059751, GET(volatility, 0), 0.000001);
    EXPECT_NEAR(2.1312, GET(volatility, 1), 0.000001);

    pnl_mat_free(&sigma);
    pnl_vect_free(&volatility);
}


int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
