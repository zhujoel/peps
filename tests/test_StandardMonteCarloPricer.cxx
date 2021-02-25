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
        IPricer *mc;
        Ocelia *ocelia;
        IModel *model;
        PnlRng *rng;
        PnlVect *volatility;
        PnlMat *sigma;
        PnlMat *past;
        int size;
        double rd;
        int nbTimeSteps;
        double T;
        PnlVect *spot;
        int nb_sous_jacents;
        double fdStep;
        int nbSamples;


        virtual void SetUp(){
            // BLACK-SCHOLES
            // TODO: mettre fenetre d'estimation
            HistoricalMarketData *historical = new HistoricalMarketData("Ocelia", new DateTime(15, 5, 2008), new DateTime(28, 4, 2016));
            historical->getData();
            this->size = 7;
            this->rd = 0.03;
            this->nbTimeSteps = historical->dates_.size();
            this->T = this->nbTimeSteps/250;
            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));
            this->model = new BlackScholesModel(size, rd, historical->path_);

            // OCELIA
            this->nb_sous_jacents = 4;
            this->ocelia = new Ocelia(T, nbTimeSteps, size, nb_sous_jacents, historical->dates_);

            // MONTE CARLO
            this->fdStep = 0.05;
            this->nbSamples = 1000;
            this->mc = new StandardMonteCarloPricer(model, ocelia, rng, fdStep, nbSamples);
        }

        virtual void TearDown(){
        }
};

TEST_F(StandardMonteCarloPricerTest, simul)
{
    double prix = 0.0;
    double prix_std_dev = 0.0;
    PnlVect* delta = pnl_vect_create(7);
    PnlVect* delta_std_dev = pnl_vect_create(7);
    this->mc->simulate(prix, prix_std_dev, delta, delta_std_dev);

    std::cout << "prix: " << prix << std::endl;
    std::cout << "prix_std_dev: " << prix_std_dev << std::endl;
    pnl_vect_print(delta);
    pnl_vect_print(delta_std_dev);
    
    EXPECT_EQ(1, 1);
}



int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
