#include "gtest/gtest.h"
#include "libs/MathLib.h"
#include "pnl/pnl_matrix.h"
#include "models/BlackScholesModel.h"
#include "market_data/HistoricalMarketData.h"

class BlackScholesModelTest: public ::testing::Test{
    protected:
        IMarketData *historical;
        InterestRate* rates;
        PnlRng *rng;
        IModel *bs;
        PnlVect *computed_ti;
        int size = 7;
        double T = 10;
        int nb_jours_ouvres;
        double timestep;

        virtual void SetUp(){
            // DATA
            this->historical = new HistoricalMarketData("Ocelia", new DateTime(01, 01, 2003), new DateTime(01, 01, 2013));
            this->historical->set_data();
            
            // PARAMETERS
            rates = new InterestRate(0, new DateTime(28, 05, 2008), this->historical->dates_, this->historical->interest_path_);
            this->nb_jours_ouvres = this->historical->path_->m;
            this->timestep = this->T/this->nb_jours_ouvres;
            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));

            // COMPUTE t_i's
            this->computed_ti = pnl_vect_create(this->nb_jours_ouvres);
            for(int i = 0; i < nb_jours_ouvres; ++i)
            {
                LET(this->computed_ti, i) = + i*this->timestep;
            }
            this->bs = new BlackScholesModel(this->size, this->nb_jours_ouvres, this->rates, this->computed_ti);
            compute_sigma(this->bs->sigma_, this->historical->path_, 0, this->historical->path_->m-1);
            compute_volatility(this->bs->volatility_, this->bs->sigma_);
        }

        virtual void TearDown(){
            delete this->historical;
            delete this->rates;
            pnl_rng_free(&this->rng);
            delete this->bs;
            pnl_vect_free(&this->computed_ti);
        }
};

TEST_F(BlackScholesModelTest, asset){
    PnlMat *path = pnl_mat_create(this->nb_jours_ouvres, this->size); 
    PnlVect *spot = pnl_vect_new();
    pnl_mat_get_row(spot, this->historical->path_, 0);

    bs->asset(path, 0, rng, spot);

    PnlMat *bs_sigma = pnl_mat_new();
    compute_sigma(bs_sigma, path, 0, path->m-1);
    PnlVect *bs_volatility = pnl_vect_create(bs_sigma->m);
    compute_volatility(bs_volatility, bs_sigma);

    for(int i = 0; i < this->size; ++i){
        for(int j = 0; j < this->size; ++j){
            EXPECT_NEAR(MGET(this->bs->sigma_, i, j), MGET(bs_sigma, i, j), 0.01);
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
