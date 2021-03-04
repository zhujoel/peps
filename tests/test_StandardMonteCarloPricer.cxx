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
            this->historical = new HistoricalMarketData("Ocelia", new DateTime(1, 1, 2005), new DateTime(1, 1, 2017));
            historical->set_data();

            // PROCESSING DES DONNEES
            std::vector<DateTime*> ocelia_dates;
            from_date_to_date(ocelia_dates, this->historical->dates_, new DateTime(15, 5, 2008), new DateTime(28, 4, 2016));
            this->ocelia_path = pnl_mat_new();
            get_subset_path_from_dates(this->ocelia_path, this->historical->dates_, ocelia_dates, this->historical->path_);
            std::vector<DateTime*> past_dates;
            from_date_to_date(past_dates, this->historical->dates_, new DateTime(15, 5, 2008), new DateTime(15, 5, 2008)); 
            this->past = pnl_mat_new();
            get_subset_path_from_dates(this->past, this->historical->dates_, past_dates, this->historical->path_);
            this->past_index = get_indice_from_date(this->historical->dates_, new DateTime(15, 5, 2008));

            // PARAMETERS
            this->estimation_start = get_indice_from_date(this->historical->dates_, new DateTime(15, 5, 2006));
            this->estimation_end = get_indice_from_date(this->historical->dates_, new DateTime(15, 5, 2008));
            this->sigma = pnl_mat_new();
            compute_sigma(this->sigma, this->historical->path_, this->estimation_start, this->estimation_end);
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
            this->fdStep = 0.00005; // NE PAS CHANGER !!!!!!!! TODO adapter en fonction de la Share Val
            this->nbSamples = 10; // TODO METTRE EN ZERO UN TRUC TRES ELEVEE CAR FORT IMPACT SUR LES DELTA EN ZERO
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
    double prix = 0.;
    double prix_std_dev = 0.;
    double finalPnL = 0.;
    PnlVect* delta = pnl_vect_create_from_zero(this->size);
    PnlVect* previous_delta = pnl_vect_create_from_zero(this->size);
    PnlVect* delta_std_dev = pnl_vect_create_from_zero(this->size);
    PnlVect* share_values = pnl_vect_new();
    pnl_mat_get_row(share_values, this->historical->path_, 0);
    this->ocelia->adjust_spot(share_values);
    this->ocelia->adjust_past(this->past);
    this->ocelia->adjust_sigma(this->sigma);

    this->mc->simulate(this->past, 0, this->sigma, prix, prix_std_dev, delta, delta_std_dev);
    pnl_vect_clone(previous_delta, delta);
    
    double V = prix - pnl_vect_scalar_prod(delta, share_values);
    finalPnL = V + pnl_vect_scalar_prod(delta, share_values) - prix;

    double riskFreeRate = exp(rd*this->T/this->nbTimeSteps); // TODO : ne pas supposer l'interval régulier ??

    std::cout << this->historical->dates_[this->past_index] << " : " << prix << ", prix sdt dev : " << prix_std_dev << std::endl;
    std::cout << "      k : " << 0 <<"  t : " << 0 << std::endl;
    std::cout << "      share values : ";
    pnl_vect_print_asrow(share_values);
    std::cout << "      delta : ";
    pnl_vect_print_asrow(delta);
    std::cout << "      delta std dev : ";
    pnl_vect_print_asrow(delta_std_dev);
    std::cout << "      V: " << V  <<"  PnL : " << finalPnL << std::endl;
    std::cout << std::endl;

    for(int k = 1; k < this->nbTimeSteps; ++k)
    {
        double t = k*(this->T/this->nbTimeSteps);

        compute_sigma(this->sigma, this->historical->path_, this->estimation_start+k, this->estimation_end+k);

        pnl_mat_get_row(share_values, this->historical->path_, this->past_index+k);
        this->ocelia->adjust_spot(share_values);
        pnl_mat_add_row(this->past, past->m, share_values);
        
        this->mc->simulate(this->past, t, this->sigma, prix, prix_std_dev, delta, delta_std_dev);

        V = V * riskFreeRate;
        for (int d = 0; d < this->size; ++d)
        {
             V -= (GET(delta, d) - GET(previous_delta, d)) * GET(share_values, d);
        }
        finalPnL = V + pnl_vect_scalar_prod(delta, share_values) - prix;
        pnl_vect_clone(previous_delta, delta);

        std::cout << this->historical->dates_[this->past_index+k] << " : " << prix << ", prix sdt dev : " << prix_std_dev << std::endl;
        std::cout << "      k : " << k <<"  t : " << t << std::endl;
        std::cout << "      share values : ";
        pnl_vect_print_asrow(share_values);
        std::cout << "      delta : ";
        pnl_vect_print_asrow(delta);
        std::cout << "      delta std dev : ";
        pnl_vect_print_asrow(delta_std_dev);
        std::cout << "      V: " << V  <<"  PnL : " << finalPnL << std::endl;
        std::cout << std::endl;
    }
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
