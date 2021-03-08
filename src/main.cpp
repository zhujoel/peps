#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "libs/Utilities.h"
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
 * 9. récupérer les vrais valeur de océlia
 * 10. dates de rebalancement (philippe)
 */

void set_stream_from_filename(std::ostream &stream, std::fstream &file, char* filename){
    file.open(filename, std::ios::out | std::ios::trunc);
    if(!file.is_open()){
        std::cout << "unable to open file.";
        exit(EXIT_FAILURE);
    }
    stream.rdbuf(file.rdbuf());
}

int main(int argc, char* argv[])
{
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
    InterestRate* rates = new InterestRate(0, new DateTime(15, 5, 2008), historical->dates_, historical->interest_path_);
    int nbTimeSteps = ocelia_dates.size();
    IModel *model = new BlackScholesModel(size, nbTimeSteps, rates);

    // OCELIA
    double T = 2920./365.25; // 2920 est le nb de jours entre 15/05/2008 et 13/05/2016
    int nb_sous_jacents = 4;
    Ocelia *ocelia = new Ocelia(T, size, nb_sous_jacents);
    std::vector<DateTime*> dates_semestrielles;
    std::vector<DateTime*> dates_valeurs_n_ans;
    parse_dates_file(dates_semestrielles, "../data/dates/dates_semest.csv", 16, '-');
    parse_dates_file(dates_valeurs_n_ans, "../data/dates/dates_valeurs_n.csv", 35, '-');
    ocelia->init_indices(ocelia_dates, dates_semestrielles, dates_valeurs_n_ans);

    // MONTE CARLO
    double fdStep = 0.1;
    int nbSamples = 100;
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
    DateTime* real_datetime_payoff; // par rapport à la date de début : 15/05/2008
    if(real_date_payoff == 4) real_datetime_payoff = new DateTime(11, 5, 2012);
    else if(real_date_payoff == 5) real_datetime_payoff = new DateTime(13, 5, 2013);
    else if(real_date_payoff == 6) real_datetime_payoff = new DateTime(13, 5, 2014);
    else if(real_date_payoff == 7) real_datetime_payoff = new DateTime(13, 5, 2015);
    else if(real_date_payoff == 8) real_datetime_payoff = new DateTime(13, 5, 2016);

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
    
    double val_liquidative_initiale = 100.; // TODO: a facto car c'est codé en dur dans océlia aussi
    HedgingPortfolio *portfolio = new HedgingPortfolio(prix, delta, share_values, rates, val_liquidative_initiale);

    // PRINT 
    std::ostream output_stream(nullptr);
    std::ostream delta_stream(nullptr);
    std::fstream output_file;
    std::fstream delta_file;

    if(argc > 1){
        set_stream_from_filename(output_stream, output_file, argv[1]);
        if(argc > 2) set_stream_from_filename(delta_stream, delta_file, argv[2]);
        else delta_stream.rdbuf(std::cout.rdbuf());
    }
    else output_stream.rdbuf(std::cout.rdbuf());

    output_stream << "Date,k,t,prix,prix_std_dev,V1,V2,Pf_couverture,Pnl,Valeur_liquidative,Tracking_error" << std::endl;
    delta_stream << "Date,k,delta,delta_std_dev" << std::endl;

    output_stream << historical->dates_[past_index] << "," << 0 << "," << 0 << "," << prix << "," << prix_std_dev << ",";
    output_stream << portfolio->V1_ << "," << portfolio->V2_ << "," << portfolio->get_portfolio_value(0, share_values) << ",";
    output_stream << portfolio->get_FinalPnL(0, prix, share_values) << "," << portfolio->get_valeur_liquidative(0, share_values) << ",";
    output_stream << portfolio->get_tracking_error(0, prix, share_values) << std::endl;
    delta_stream << historical->dates_[past_index] << "," << 0 << "," << delta << "," << delta_std_dev << std::endl;

    // PRICING & HEADGING en t
    for(int k = 1; k < nbTimeSteps; ++k)
    {
        // TODO: mettre >= 0 plutot ?
        if (((historical->dates_[past_index+k])->compare(real_datetime_payoff))==1) {
            std::cout << "Un payoff de " << real_payoff << " à été versé au client le " << real_datetime_payoff << std::endl;
            break;
        }

        double t = k*(T/nbTimeSteps);

        // update the sigma to take into account changes in real life
        compute_sigma(sigma, historical->path_, estimation_start+k, estimation_end+k);

        // add historical values to past
        pnl_mat_get_row(share_values, historical->path_, past_index+k);
        ocelia->adjust_spot(share_values);
        pnl_mat_add_row(past, past->m, share_values);
        
        // rebalance
        if(k%30==0){
            mc->price_and_delta(past, t, sigma, prix, prix_std_dev, delta, delta_std_dev);
            portfolio->rebalancing(t, delta, share_values);
            delta_stream << historical->dates_[past_index] << "," << k << "," << delta << "," << delta_std_dev << std::endl;

        }

        // price
        mc->price(past, t, sigma, prix, prix_std_dev);
        output_stream << historical->dates_[past_index] << "," << k << "," << t << "," << prix << "," << prix_std_dev << ",";
        output_stream << portfolio->V1_ << "," << portfolio->V2_ << "," << portfolio->get_portfolio_value(t, share_values) << ",";
        output_stream << portfolio->get_FinalPnL(t, prix, share_values) << "," << portfolio->get_valeur_liquidative(t, share_values) << ",";
        output_stream << portfolio->get_tracking_error(t, prix, share_values) << std::endl;
    }


    // DELETE
    delete historical;
    pnl_mat_free(&ocelia_path);
    pnl_mat_free(&past);
    pnl_mat_free(&sigma);
    delete rates;
    delete model;
    delete ocelia;
    delete_date_vector(dates_semestrielles);
    delete_date_vector(dates_valeurs_n_ans);
    pnl_rng_free(&rng);
    delete mc;
    delete real_datetime_payoff;
    pnl_vect_free(&delta);
    pnl_vect_free(&delta_std_dev);
    pnl_vect_free(&share_values);
    delete portfolio;
    if(argc == 1){
        output_file.close();
        if(argc > 2) delta_file.close();
    }
}