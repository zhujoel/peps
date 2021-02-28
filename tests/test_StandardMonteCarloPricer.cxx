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
        PnlMat *estimation_path;
        PnlMat *past;

        virtual void SetUp(){
            // BLACK-SCHOLES
            // TODO: mettre fenetre d'estimation
            this->historical = new HistoricalMarketData("Ocelia", new DateTime(1, 1, 2000), new DateTime(1, 1, 2017));
            historical->get_data();

            // PROCESSING DES DONNEES
            std::vector<DateTime*> ocelia_dates = from_date_to_date(this->historical->dates_, new DateTime(15, 5, 2008), new DateTime(28, 4, 2016));
            std::vector<DateTime*> estimation_window = from_date_to_date(this->historical->dates_, new DateTime(15, 5, 2006), new DateTime(15, 5, 2008));
            this->estimation_path = get_path_from_dates(this->historical->dates_, estimation_window, this->historical->path_);
            
            std::vector<DateTime*> past_dates = from_date_to_date(this->historical->dates_, new DateTime(15, 5, 2008), new DateTime(15, 5, 2008));
            this->past = get_path_from_dates(this->historical->dates_, past_dates, this->historical->path_);

            // PARAMETERS
            this->sigma = compute_sigma(estimation_path, 0, estimation_window.size()-1);
            this->size = 7;
            this->rd = 0.03;
            this->nbTimeSteps = past->m + 10000;
                this->T = this->nbTimeSteps/250;
            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));
            this->model = new BlackScholesModel(this->size, this->rd);

            // OCELIA
            this->nb_sous_jacents = 4;
            this->ocelia = new Ocelia(T, nbTimeSteps, size, nb_sous_jacents, ocelia_dates);

            // MONTE CARLO
            this->fdStep = 0.05;
            this->nbSamples = 1000;
            this->mc = new StandardMonteCarloPricer(model, ocelia, rng, fdStep, nbSamples);
        }

        virtual void TearDown(){
            delete this->historical;
            pnl_mat_free(&this->sigma);
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
    PnlVect* delta = pnl_vect_create_from_zero(this->size);
    PnlVect* delta_std_dev = pnl_vect_create_from_zero(this->size);

    this->ocelia->adjust_sigma(this->sigma);
    this->ocelia->adjust_past(this->past);

    // for(int k = 0; k < this->nbTimeSteps; ++k){

        this->mc->simulate(this->past, 0, this->sigma, prix, prix_std_dev, delta, delta_std_dev); // en t=0
        
    // }

    std::cout << "prix: " << prix << std::endl;
    std::cout << "prix_std_dev: " << prix_std_dev << std::endl;
    std::cout << "delta: " << std::endl;
    pnl_vect_print(delta);
    // pnl_vect_print(delta_std_dev);
    
    EXPECT_EQ(1, 1);

    pnl_vect_free(&delta);
    pnl_vect_free(&delta_std_dev);
}



int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
