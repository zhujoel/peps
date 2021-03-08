#include "gtest/gtest.h"
#include "libs/MathLib.h"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include "models/BlackScholesModel.h"
#include "market_data/HistoricalMarketData.h"

class BlackScholesModelTest: public ::testing::Test{
    protected:
        IMarketData *historical;
        PnlMat *sigma;
        PnlVect *volatility;
        int size;
        InterestRate* rates;
        int nbTimeSteps;
        double T;
        PnlRng *rng;
        IModel *bs;

        virtual void SetUp(){
            this->historical = new HistoricalMarketData("Ocelia", new DateTime(01, 01, 2003), new DateTime(01, 01, 2013));
            this->historical->set_data();
            this->sigma = pnl_mat_new();
            compute_sigma(this->sigma, this->historical->path_, 0, this->historical->path_->m-1);
            this->volatility = pnl_vect_create(this->sigma->m);
            compute_volatility(this->volatility, this->sigma);
            this->size = 7;
            rates = new InterestRate(0, new DateTime(28, 05, 2008), this->historical->dates_, this->historical->interest_path_);
            this->nbTimeSteps = this->historical->path_->m + 100;
            this->T = this->nbTimeSteps/250;
            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));
            this->bs = new BlackScholesModel(this->size, this->nbTimeSteps, this->rates);
        }

        virtual void TearDown(){
            delete this->historical;
            pnl_rng_free(&this->rng);
            pnl_mat_free(&this->sigma);
            pnl_vect_free(&this->volatility);
            delete this->bs;
        }
};

TEST_F(BlackScholesModelTest, asset){
    PnlMat *path = pnl_mat_create(this->nbTimeSteps, this->size);
    bs->asset(path, 0, T, rng, this->historical->path_, this->sigma);
    PnlMat *bs_sigma = pnl_mat_new();
    compute_sigma(bs_sigma, path, 0, path->m-1);
    PnlVect *bs_volatility = pnl_vect_create(bs_sigma->m);
    compute_volatility(bs_volatility, bs_sigma);

    for(int i = 0; i < this->size; ++i){
        for(int j = 0; j < this->size; ++j){
            EXPECT_NEAR(MGET(this->sigma, i, j), MGET(bs_sigma, i, j), 0.01);
        }
    }

    pnl_mat_free(&path);
    pnl_mat_free(&bs_sigma);
    pnl_vect_free(&bs_volatility);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
