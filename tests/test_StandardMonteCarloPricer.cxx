#include "gtest/gtest.h"
#include "libs/MathLib.h"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include "models/BlackScholesModel.h"
#include "market_data/HistoricalMarketData.h"
#include "derivatives/Ocelia.h"
#include "libs/HedgingPortfolio.h"
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
            this->fdStep = 0.1;
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
    // PAYOFF EFFECTIVEMENT VERSE
    this->ocelia->adjust_past(this->ocelia_path);
    double real_payoff = this->ocelia->payoff(this->ocelia_path);
    double real_date_payoff = this->ocelia->get_annee_payoff(); 
    DateTime* real_datetime_payoff;
    if(real_date_payoff == 4){ // par rapport à la date de début : 15/05/2008
        real_datetime_payoff = new DateTime(11, 5, 2012);
    }
    else if(real_date_payoff == 5){
        real_datetime_payoff = new DateTime(13, 5, 2013);
    }
    else if(real_date_payoff == 6){
        real_datetime_payoff = new DateTime(13, 5, 2014);
    }
    else if(real_date_payoff == 7){
        real_datetime_payoff = new DateTime(13, 5, 2015);
    }
    else if(real_date_payoff == 8){
        real_datetime_payoff = new DateTime(13, 5, 2016);
    }

    // PRICING & HEADGING en 0
    double prix = 0.;
    double prix_std_dev = 0.;
    PnlVect* delta = pnl_vect_create_from_zero(this->size);
    PnlVect* delta_std_dev = pnl_vect_create_from_zero(this->size);
    PnlVect* share_values = pnl_vect_new();
    pnl_mat_get_row(share_values, this->historical->path_, 0);
    this->ocelia->adjust_spot(share_values);
    this->ocelia->adjust_past(this->past);
    this->ocelia->adjust_sigma(this->sigma);

    this->mc->simulate(this->past, 0, this->sigma, prix, prix_std_dev, delta, delta_std_dev);
    
    double val_liquidative_initiale = 100.;
    HedgingPortfolio *portfolio = new HedgingPortfolio(prix, delta, share_values, this->rd, val_liquidative_initiale);

    std::cout << this->historical->dates_[this->past_index] << " prix : " << prix << ", prix sdt dev : " << prix_std_dev << std::endl;
    std::cout << "      k : " << 0 <<"  t : " << 0 << std::endl;
    std::cout << "      share values : ";
    pnl_vect_print_asrow(share_values);
    std::cout << "      delta : ";
    pnl_vect_print_asrow(delta);
    std::cout << "      delta std dev : ";
    pnl_vect_print_asrow(delta_std_dev);
    std::cout << "      V1 : " << portfolio->V1_ << ",  Pf de couverture : " << portfolio->get_portfolio_value(0, share_values) << ",  PnL : " << portfolio->get_tracking_error(0, prix, share_values) << std::endl;
    std::cout << "      V2 : " << portfolio->V2_ << ",  Valeur liquidative : " << portfolio->get_valeur_liquidative(0, share_values) << ",  PnL : " << portfolio->get_FinalPnL(0, prix, share_values) << std::endl;
    std::cout << std::endl;

    // PRICING & HEADGING en t
    for(int k = 1; k < this->nbTimeSteps; ++k)
    {
        if (((this->historical->dates_[this->past_index+k])->compare(real_datetime_payoff))==1) {
            std::cout << "Un payoff de " << real_payoff << " à été versé au client le " << real_datetime_payoff << std::endl;
            break;
        }

        double t = k*(this->T/this->nbTimeSteps);
        compute_sigma(this->sigma, this->historical->path_, this->estimation_start+k, this->estimation_end+k);

        pnl_mat_get_row(share_values, this->historical->path_, this->past_index+k);
        this->ocelia->adjust_spot(share_values);
        pnl_mat_add_row(this->past, past->m, share_values);
        
        this->mc->simulate(this->past, t, this->sigma, prix, prix_std_dev, delta, delta_std_dev);

        portfolio->rebalancing(t, delta, share_values);

        std::cout << this->historical->dates_[this->past_index+k] << " : " << prix << ", prix sdt dev : " << prix_std_dev << std::endl;
        std::cout << "      k : " << k <<"  t : " << t << std::endl;
        std::cout << "      share values : ";
        pnl_vect_print_asrow(share_values);
        std::cout << "      delta : ";
        pnl_vect_print_asrow(delta);
        std::cout << "      delta std dev : ";
        pnl_vect_print_asrow(delta_std_dev);
        std::cout << "      V1 : " << portfolio->V1_ << ",  Pf de couverture : " << portfolio->get_portfolio_value(t, share_values) << ",  Tracking error : " << portfolio->get_tracking_error(t, prix, share_values) << std::endl;
        std::cout << "      V2 : " << portfolio->V2_ << ",  Valeur liquidative : " << portfolio->get_valeur_liquidative(t, share_values) << ",  PnL : " << portfolio->get_FinalPnL(t, prix, share_values) << std::endl;
        std::cout << std::endl;
    }
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
