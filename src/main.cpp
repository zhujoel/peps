#include <iostream>
#include <vector>
#include <ctime>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "libs/MathLib.h"
#include "derivatives/Ocelia.h"
#include "models/BlackScholesModel.h"
#include "market_data/HistoricalMarketData.h"
#include "pricers/StandardMonteCarloPricer.h"
#include "libs/HedgingPortfolio.h"

/**
 * TODO:
 * 4. taux d'intéret étrangers
 * 7. output/input montecarlo en t
 */

int main(){

    // PROCESSING MARKET DATA
    IMarketData *historical = new HistoricalMarketData("Ocelia", new DateTime(1, 1, 2005), new DateTime(1, 1, 2017));
    historical->set_data();

    std::vector<DateTime*> ocelia_dates;
    PnlMat *ocelia_path = pnl_mat_new();
    from_date_to_date(ocelia_dates, historical->dates_, new DateTime(15, 5, 2008), new DateTime(28, 4, 2016));
    get_subset_path_from_dates(ocelia_path, historical->dates_, ocelia_dates, historical->path_);
    
    std::vector<DateTime*> past_dates;
    PnlMat *past = pnl_mat_new();
    from_date_to_date(past_dates, historical->dates_, new DateTime(15, 5, 2008), new DateTime(15, 5, 2008)); 
    get_subset_path_from_dates(past, historical->dates_, past_dates, historical->path_);
    
    int past_index = get_indice_from_date(historical->dates_, new DateTime(15, 5, 2008));
    int estimation_start = get_indice_from_date(historical->dates_, new DateTime(15, 5, 2006));
    int estimation_end = get_indice_from_date(historical->dates_, new DateTime(15, 5, 2008));
    
    PnlMat *sigma = pnl_mat_new();
    compute_sigma(sigma, historical->path_, estimation_start, estimation_end);
    

    // BLACK-SCHOLES
    int size = 7;
    double rd = 0.03;
    int nbTimeSteps = ocelia_dates.size();
    IModel *model = new BlackScholesModel(size, nbTimeSteps, rd);

    // OCELIA
    double T = 2920./365.25; // 2920 est le nb de jours entre 15/05/2008 et 13/05/2016
    int nb_sous_jacents = 4;
    Ocelia *ocelia = new Ocelia(T, size, nb_sous_jacents);
    std::vector<DateTime*> dates_semestrielles;
    std::vector<DateTime*> dates_valeurs_n_ans;
    parse_dates_file(dates_semestrielles, "../tests/test_data/ocelia/dates_semest.csv", 16, '-');
    parse_dates_file(dates_valeurs_n_ans, "../tests/test_data/ocelia/dates_valeurs_n.csv", 35, '-');
    ocelia->init_indices(ocelia_dates, dates_semestrielles, dates_valeurs_n_ans);

    // MONTE CARLO
    double fdStep = 0.1;
    int nbSamples = 10;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, std::time(NULL));
    IPricer *mc = new StandardMonteCarloPricer(model, ocelia, rng, fdStep, nbSamples);

    /**
     * ********* SIMULATION *********
     */


    // PAYOFF EFFECTIVEMENT VERSE
    ocelia->adjust_past(ocelia_path);
    double real_payoff = ocelia->payoff(ocelia_path);
    double real_date_payoff = ocelia->get_annee_payoff(); 
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
    PnlVect* delta = pnl_vect_create_from_zero(size);
    PnlVect* delta_std_dev = pnl_vect_create_from_zero(size);
    PnlVect* share_values = pnl_vect_new();
    pnl_mat_get_row(share_values, historical->path_, 0);
    ocelia->adjust_spot(share_values);
    ocelia->adjust_past(past);
    ocelia->adjust_sigma(sigma);

    mc->price_and_delta(past, 0, sigma, prix, prix_std_dev, delta, delta_std_dev);
    
    double val_liquidative_initiale = 100.;
    HedgingPortfolio *portfolio = new HedgingPortfolio(prix, delta, share_values, rd, val_liquidative_initiale);

    std::cout << historical->dates_[past_index] << " prix : " << prix << ", prix sdt dev : " << prix_std_dev << std::endl;
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

    // TODO: faire un vrai truc pour les dates de rebalancement
    int rebalance_counter = 0;

    // PRICING & HEADGING en t
    for(int k = 1; k < nbTimeSteps; ++k)
    {
        if (((historical->dates_[past_index+k])->compare(real_datetime_payoff))==1) {
            std::cout << "Un payoff de " << real_payoff << " à été versé au client le " << real_datetime_payoff << std::endl;
            break;
        }

        double t = k*(T/nbTimeSteps);
        compute_sigma(sigma, historical->path_, estimation_start+k, estimation_end+k);

        pnl_mat_get_row(share_values, historical->path_, past_index+k);
        ocelia->adjust_spot(share_values);
        pnl_mat_add_row(past, past->m, share_values);
        
        if(rebalance_counter == 30){
            mc->price_and_delta(past, t, sigma, prix, prix_std_dev, delta, delta_std_dev);
            rebalance_counter = 0;
            portfolio->rebalancing(t, delta, share_values);
        }

        mc->price(past, t, sigma, prix, prix_std_dev);
        rebalance_counter++;

        std::cout << historical->dates_[past_index+k] << " : " << prix << ", prix sdt dev : " << prix_std_dev << std::endl;
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

    delete historical;
    pnl_mat_free(&sigma);
    delete mc;
    delete ocelia;
    delete model;
    pnl_rng_free(&rng);
}