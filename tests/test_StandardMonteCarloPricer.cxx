#include "gtest/gtest.h"
#include "libs/MathLib.h"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include "models/BlackScholesModel.h"
#include "market_data/HistoricalMarketData.h"
#include "derivatives/Ocelia.h"
#include "pricers/StandardMonteCarloPricer.h"

class StandardMonteCarloPricerTest: public ::testing::Test{
    protected:
        IMarketData *historical;
        PnlMat *sigma;
        PnlVect *volatility;
        IPricer *mc;
        Ocelia *ocelia;
        IModel *model;
        PnlRng *rng;
        int size;
        double rd;
        int nbTimeSteps;
        double T;
        int nb_sous_jacents;
        double fdStep;
        int nbSamples;

        virtual void SetUp(){
            // BLACK-SCHOLES
            // TODO: mettre fenetre d'estimation
            this->historical = new HistoricalMarketData("Ocelia", new DateTime(15, 5, 2006), new DateTime(15, 5, 2016));
            historical->get_data();
            this->sigma = compute_sigma(this->historical->path_, 0, this->historical->path_->m);
            this->size = 7;
            this->rd = 0.03;
            this->nbTimeSteps = historical->dates_.size();
            this->T = this->nbTimeSteps/250;
            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));
            this->model = new BlackScholesModel(this->size, this->rd);

            // OCELIA
            this->nb_sous_jacents = 4;
            this->ocelia = new Ocelia(T, nbTimeSteps, size, nb_sous_jacents, historical->dates_);

            // MONTE CARLO
            this->fdStep = 0.05;
            this->nbSamples = 1000;
            this->mc = new StandardMonteCarloPricer(model, ocelia, rng, fdStep, nbSamples);
        }

        virtual void TearDown(){
            delete this->historical;
            pnl_mat_free(&this->sigma);
            pnl_vect_free(&this->volatility);
            delete this->mc;
            delete this->ocelia;
            delete this->model;
            pnl_rng_free(&this->rng);
        }
};

TEST_F(StandardMonteCarloPricerTest, simul)
{
    double prix = 0.0;
    double prix_std_dev = 0.0;
    PnlVect* delta = pnl_vect_create(this->size);
    PnlVect* delta_std_dev = pnl_vect_create(this->size);

    // this->ocelia->adjust_sigma(this->model_->sigma_);
    // this->ocelia->adjust_past(past); // TODO: a virer d'ici
    this->mc->simulate(this->historical->path_, 0, this->sigma, prix, prix_std_dev, delta, delta_std_dev);

    std::cout << "prix: " << prix << std::endl;
    std::cout << "prix_std_dev: " << prix_std_dev << std::endl;
    // pnl_vect_print(delta);
    // pnl_vect_print(delta_std_dev);
    
    EXPECT_EQ(1, 1);

    pnl_vect_free(&delta);
    pnl_vect_free(&delta_std_dev);
}



int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
