#include "gtest/gtest.h"
#include "libs/MathLib.h"
#include "pnl/pnl_matrix.h"
#include <fstream>

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

TEST_F(MathLibTest, log_returns){

    PnlMat *returns = pnl_mat_new();
    log_returns(returns, this->path, 0, 2);

    EXPECT_NEAR(0.04879, MGET(returns, 0, 0), 0.000001);
    EXPECT_NEAR(0.04652, MGET(returns, 1, 0), 0.000001);
    EXPECT_NEAR(0.09531018, MGET(returns, 0, 1), 0.000001);
    EXPECT_NEAR(-0.09531, MGET(returns, 1, 1), 0.000001);

    pnl_mat_free(&returns);
}

TEST_F(MathLibTest, means){
    PnlVect *means_path = pnl_vect_new();
    means(means_path, this->path);

    EXPECT_NEAR(105, GET(means_path, 0), 0.000001);
    EXPECT_NEAR(103.333333, GET(means_path, 1), 0.000001);

    pnl_vect_free(&means_path);
}

TEST_F(MathLibTest, compute_covariance_k_l){
    PnlMat *returns = pnl_mat_new();
    log_returns(returns, this->path, 0, 2);
    PnlVect *means_returns = pnl_vect_new();
    means(means_returns, returns);
    double covariance_0_0 = compute_covariance(returns, means_returns, 0, 0);
    EXPECT_NEAR(0.0000025764, covariance_0_0, 0.000000001);

    pnl_mat_free(&returns);
    pnl_vect_free(&means_returns);
}

TEST_F(MathLibTest, compute_covariances){
    PnlMat *covariances = pnl_mat_new();
    compute_covariances(covariances, this->path, 0, 2);

    EXPECT_NEAR(0.0006441, MGET(covariances, 0, 0), 0.000001);
    EXPECT_NEAR(0.054092, MGET(covariances, 0, 1), 0.000001);
    EXPECT_NEAR(0.054092, MGET(covariances, 1, 0), 0.000001);
    EXPECT_NEAR(4.5420152, MGET(covariances, 1, 1), 0.000001);

    pnl_mat_free(&covariances);
}

TEST_F(MathLibTest, compute_sigma){
    PnlMat *sigma = pnl_mat_new();
    compute_sigma(sigma, this->path, 0, 2);

    EXPECT_NEAR(0.025381, MGET(sigma, 0, 0), 0.000001);
    EXPECT_NEAR(0.054092, MGET(sigma, 0, 1), 0.000001);
    EXPECT_NEAR(2.1312, MGET(sigma, 1, 0), 0.000001);
    EXPECT_NEAR(0, MGET(sigma, 1, 1), 0.000001);

    pnl_mat_free(&sigma);
}

TEST_F(MathLibTest, compute_volatility){
    PnlMat *sigma = pnl_mat_new();
    compute_sigma(sigma, this->path, 0, 2);
    PnlVect *volatility = pnl_vect_new();
    compute_volatility(volatility, sigma);

    EXPECT_NEAR(0.059751, GET(volatility, 0), 0.000001);
    EXPECT_NEAR(2.1312, GET(volatility, 1), 0.000001);

    pnl_mat_free(&sigma);
    pnl_vect_free(&volatility);
}


int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
