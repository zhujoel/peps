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
        PnlMat *ocelia_path;
        PnlMat *estimation_path;
        PnlMat *past;
        int estimation_start;
        int estimation_end;
        int past_index;

        virtual void SetUp(){
            // BLACK-SCHOLES
            // TODO: mettre fenetre d'estimation
            this->historical = new HistoricalMarketData("Ocelia", new DateTime(1, 1, 2005), new DateTime(1, 1, 2017));
            historical->get_data();

            // PROCESSING DES DONNEES
            std::vector<DateTime*> ocelia_dates = from_date_to_date(this->historical->dates_, new DateTime(15, 5, 2008), new DateTime(28, 4, 2016));
            this->ocelia_path = get_path_from_dates(this->historical->dates_, ocelia_dates, this->historical->path_);
            std::vector<DateTime*> past_dates = from_date_to_date(this->historical->dates_, new DateTime(15, 5, 2008), new DateTime(15, 5, 2008));
            this->past = get_path_from_dates(this->historical->dates_, past_dates, this->historical->path_);
            this->past_index = get_indice_from_date(this->historical->dates_, new DateTime(15, 5, 2008));

            // PARAMETERS
            this->estimation_start = get_indice_from_date(this->historical->dates_, new DateTime(15, 5, 2006));
            this->estimation_end = get_indice_from_date(this->historical->dates_, new DateTime(15, 5, 2008));
            this->sigma = compute_sigma(this->historical->path_, this->estimation_start, this->estimation_end);
            this->size = 7;
            this->rd = 0.03;
            this->nbTimeSteps = ocelia_dates.size();
            this->T = 2920./365.25; // 2920 est le nb de jours entre 15/05/2008 et 13/05/2016
            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));
            this->model = new BlackScholesModel(this->size, this->rd);

            // OCELIA
            this->nb_sous_jacents = 4;
            this->ocelia = new Ocelia(T, nbTimeSteps, size, nb_sous_jacents, ocelia_dates);

            // MONTE CARLO
            this->fdStep = 0.05;
            this->nbSamples = 100;
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
    
    PnlVect* newPastRow = pnl_vect_new();

    // pnl_mat_print(this->ocelia_path);
    // std::cout << this->ocelia->payoff(this->ocelia_path) << std::endl;
    // std::cout << this->ocelia->get_annee_payoff() << std::endl;
    // std::cout << this->ocelia->compute_perf_moyenne_panier(this->ocelia_path) << std::endl;

    for(int k = 1; k < this->nbTimeSteps; ++k)
    {
        std::cout << k << std::endl;
        
        pnl_mat_get_row(newPastRow, this->historical->path_, this->past_index+k);
        pnl_mat_add_row(this->past, past->m, newPastRow);

        pnl_mat_free(&this->sigma);

        this->sigma = compute_sigma(this->historical->path_, this->estimation_start+k, this->estimation_end+k);
        
        this->mc->simulate(this->past, k*(this->T/this->nbTimeSteps), this->sigma, prix, prix_std_dev, delta, delta_std_dev);

        std::cout << this->historical->dates_[this->past_index+k] << " : " << prix << std::endl;
    }

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
