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

void set_stream_from_filename(std::ostream &stream, std::fstream &file, const char* const filename){
    file.open(filename, std::ios::out | std::ios::in);
    if(!file.is_open()){
        std::cout << "unable to open file.";
        exit(EXIT_FAILURE);
    }
    stream.rdbuf(file.rdbuf());
}

std::string get_last_line(std::fstream &fin)
{
    if(fin.is_open()) {
        fin.seekg(-2, std::ios_base::end);                // go to one spot before the EOF
        bool keepLooping = true;
        while(keepLooping) {
            char ch;
            fin.get(ch);                            // Get current byte's data

            if((int)fin.tellg() <= 1) {             // If the data was at or before the 0th byte
                fin.seekg(0);                       // The first line is the last line
                keepLooping = false;                // So stop there
            }
            else if(ch == '\n') {                   // If the data was a newline
                keepLooping = false;                // Stop at the current position.
            }
            else {                                  // If the data was neither a newline nor at the 0 byte
                fin.seekg(-2, std::ios_base::cur);        // Move to the front of that data, then to the front of the data before it
            }
        }

        std::string lastLine;            
        getline(fin,lastLine);    
        return lastLine;     // Display it
    }
    return "";
}


// STREAMS
std::ostream output_stream(nullptr);
std::ostream delta_stream(nullptr);
std::fstream output_file;
std::fstream delta_file;

// MARKET DATA
HistoricalMarketData *historical;
PnlMat *ocelia_path = pnl_mat_new();
std::vector<DateTime*> ocelia_dates;
PnlMat *past = pnl_mat_new();
std::vector<DateTime*> past_dates;
int past_index;
int horizon_estimation;
int estimation_end;
int estimation_start;

// BLACK-SCHOLES
int size;
InterestRate *rates;
int nbTimeSteps;
IModel *model;

// OCELIA
double val_liquidative_initiale;
double T;
int nb_sous_jacents;
Ocelia *ocelia;
std::vector<DateTime*> dates_semestrielles;
std::vector<DateTime*> dates_valeurs_n_ans;

// MONTE-CARLO
double timestep;
double fdStep;
int nbSamples;
PnlRng *rng;
IPricer *mc;

// PAYOFF
double real_payoff;
double real_date_payoff;
DateTime* real_datetime_payoff;

// PRICING
double prix;
double prix_std_dev;
PnlVect *delta;
PnlVect *delta_std_dev;
PnlVect *share_values;
PnlMat estimation_window;

// REBALANCING PORTFOLIO
HedgingPortfolio *portfolio;
int rebalancement_horizon;

void simulate_all()
{
    output_stream << "Date,k,t,prix,prix_std_dev,V1,V2,Pf_couverture,Pnl,Valeur_liquidative,Tracking_error,Ocelia_Valeur_liquidative" << std::endl;
    delta_stream << "Date,k,delta,delta_std_dev" << std::endl;
    output_stream << historical->dates_[past_index] << "," << 0 << "," << 0 << "," << prix << "," << prix_std_dev << ",";
    output_stream << portfolio->V1_ << "," << portfolio->V2_ << "," << portfolio->get_portfolio_value(0, share_values) << ",";
    output_stream << portfolio->get_FinalPnL(0, prix, share_values) << "," << portfolio->get_valeur_liquidative(0, share_values) << ",";
    output_stream << portfolio->get_tracking_error(0, prix, share_values) << "," << MGET(historical->derivative_path_, past_index, 0) << std::endl;
    delta_stream << historical->dates_[past_index] << "," << 0 << "," << delta << "," << delta_std_dev << std::endl;

    // PRICING & HEADGING en t
    for(int k = 1; k < nbTimeSteps; ++k)
    {
        if (((historical->dates_[past_index+k])->compare(real_datetime_payoff))==1) {
            std::cout << "Un payoff de " << real_payoff << " à été versé au client le " << real_datetime_payoff << std::endl;
            break;
        }

        double t = k*timestep;

        estimation_window = pnl_mat_wrap_mat_rows(historical->path_, estimation_start+k, estimation_end+k);  

        // add historical values to past
        pnl_mat_get_row(share_values, historical->path_, past_index+k);
        ocelia->adjust_spot(share_values, t);
        pnl_mat_add_row(past, past->m, share_values);
        
        // rebalance
        if(k%rebalancement_horizon==0){
            mc->price_and_delta(past, estimation_window, t, prix, prix_std_dev, delta, delta_std_dev);
            portfolio->rebalancing(t, delta, share_values);
            delta_stream << historical->dates_[past_index+k] << "," << k << "," << delta << "," << delta_std_dev << std::endl;

        }

        // price
        mc->price(past, estimation_window, t, prix, prix_std_dev);
        output_stream << historical->dates_[past_index+k] << "," << k << "," << t << "," << prix << "," << prix_std_dev << ",";
        output_stream << portfolio->V1_ << "," << portfolio->V2_ << "," << portfolio->get_portfolio_value(t, share_values) << ",";
        output_stream << portfolio->get_FinalPnL(t, prix, share_values) << "," << portfolio->get_valeur_liquidative(t, share_values) << ",";
        output_stream << portfolio->get_tracking_error(t, prix, share_values) << "," << MGET(historical->derivative_path_, past_index+k, 0) << std::endl;
    }
}


// TODO: fix
void simulate_next()
{
    // PARSE LAST LINE
    // output
    std::string output_last_line = get_last_line(output_file);
    int output_size = 12;
    std::string output_last_line_split[output_size];
    split(output_last_line_split, output_last_line, ',');
    // delta
    std::string delta_last_line = get_last_line(delta_file);
    int delta_size = 4;
    std::string delta_last_line_split[delta_size];
    split(delta_last_line_split, delta_last_line, ',');

    std::string delta_split[size];
    std::string delta_std_dev_split[size];
    split(delta_split, delta_last_line_split[2], ' ');
    split(delta_std_dev_split, delta_last_line_split[3], ' ');
    for(int i = 0; i < size; ++i){
        LET(delta, i) = std::stod(delta_split[i]);
        LET(delta_std_dev, i) = std::stod(delta_std_dev_split[i]);
        LET(portfolio->delta_, i) = std::stod(delta_split[i]);
    }

    // SET DATA TO PAST DATA
    int k = std::stoi(output_last_line_split[1])+1;
    double t = k*timestep;
    prix = std::stod(output_last_line_split[3]);
    prix_std_dev = std::stod(output_last_line_split[4]);
    portfolio->V1_ = std::stod(output_last_line_split[5]);
    portfolio->V2_ = std::stod(output_last_line_split[6]);
    portfolio->last_rebalancing_t_ = std::stod(delta_last_line_split[1])*timestep;

    estimation_window = pnl_mat_wrap_mat_rows(historical->path_, estimation_start+k, estimation_end+k);  

    // add historical values to past
    pnl_mat_get_row(share_values, historical->path_, past_index+k);
    ocelia->adjust_spot(share_values, t);
    pnl_mat_add_row(past, past->m, share_values);
        
    // rebalance
    if(k%rebalancement_horizon==0){
        mc->price_and_delta(past, estimation_window, t, prix, prix_std_dev, delta, delta_std_dev);
        portfolio->rebalancing(t, delta, share_values);
        delta_stream << historical->dates_[past_index+k] << "," << k << "," << delta << "," << delta_std_dev << std::endl;
    }

    // price
    mc->price(past, estimation_window, t, prix, prix_std_dev);
    output_stream << historical->dates_[past_index+k] << "," << k << "," << t << "," << prix << "," << prix_std_dev << ",";
    output_stream << portfolio->V1_ << "," << portfolio->V2_ << "," << portfolio->get_portfolio_value(t, share_values) << ",";
    output_stream << portfolio->get_FinalPnL(t, prix, share_values) << "," << portfolio->get_valeur_liquidative(t, share_values) << ",";
    output_stream << portfolio->get_tracking_error(t, prix, share_values) << "," << MGET(historical->derivative_path_, past_index+k, 0) << std::endl;
}

int main(int argc, char* argv[])
{
    // STREAMS
    if(argc > 1){
        set_stream_from_filename(output_stream, output_file, argv[1]);
        if(argc > 2) set_stream_from_filename(delta_stream, delta_file, argv[2]);
        else delta_stream.rdbuf(std::cout.rdbuf());
    }
    else output_stream.rdbuf(std::cout.rdbuf());

    // PROCESSING MARKET DATA
    historical = new HistoricalMarketData("Ocelia", new DateTime(1, 1, 2005), new DateTime(1, 1, 2017));
    historical->set_data();
    from_date_to_date(ocelia_dates, historical->dates_, new DateTime(15, 5, 2008), new DateTime(28, 4, 2016));
    get_subset_path_from_dates(ocelia_path, historical->dates_, ocelia_dates, historical->path_);
    from_date_to_date(past_dates, historical->dates_, new DateTime(15, 5, 2008), new DateTime(15, 5, 2008)); 
    get_subset_path_from_dates(past, historical->dates_, past_dates, historical->path_);
    past_index = get_indice_from_date(historical->dates_, new DateTime(15, 5, 2008));
    horizon_estimation = 500;
    estimation_end = get_indice_from_date(historical->dates_, new DateTime(15, 5, 2008));
    estimation_start = estimation_end - horizon_estimation;
    
    // BLACK-SCHOLES
    size = 7;
    rates = new InterestRate(0, new DateTime(15, 5, 2008), historical->dates_, historical->interest_path_);
    nbTimeSteps = ocelia_dates.size();
    model = new BlackScholesModel(size, nbTimeSteps, rates);
    
    // OCELIA
    val_liquidative_initiale = 100.;
    T = 2920./365.25; // 2920 est le nb de jours entre 15/05/2008 et 13/05/2016
    nb_sous_jacents = 4;
    ocelia = new Ocelia(T, size, nb_sous_jacents, val_liquidative_initiale, rates);
    parse_dates_file(dates_semestrielles, "../data/dates/dates_semest.csv", 16, '-');
    parse_dates_file(dates_valeurs_n_ans, "../data/dates/dates_valeurs_n.csv", 35, '-');
    ocelia->init_indices(ocelia_dates, dates_semestrielles, dates_valeurs_n_ans);

    // MONTE CARLO
    timestep = T/nbTimeSteps;
    fdStep = 0.1;
    nbSamples = 100;
    rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, std::time(NULL));
    mc = new StandardMonteCarloPricer(model, ocelia, rng, fdStep, nbSamples);

    // PAYOFF EFFECTIVEMENT VERSE
    ocelia->adjust_past(ocelia_path, timestep);
    real_payoff = ocelia->payoff(ocelia_path);
    real_date_payoff = ocelia->get_annee_payoff(); 
    real_datetime_payoff; // par rapport à la date de début : 15/05/2008
    if(real_date_payoff == 4) real_datetime_payoff = new DateTime(11, 5, 2012);
    else if(real_date_payoff == 5) real_datetime_payoff = new DateTime(13, 5, 2013);
    else if(real_date_payoff == 6) real_datetime_payoff = new DateTime(13, 5, 2014);
    else if(real_date_payoff == 7) real_datetime_payoff = new DateTime(13, 5, 2015);
    else if(real_date_payoff == 8) real_datetime_payoff = new DateTime(13, 5, 2016);

    // PRICING & HEADGING en 0
    prix = 0.;
    prix_std_dev = 0.;
    delta = pnl_vect_create_from_zero(size);
    delta_std_dev = pnl_vect_create_from_zero(size);
    share_values = pnl_vect_new();
    pnl_mat_get_row(share_values, historical->path_, 0);
    ocelia->adjust_spot(share_values, 0);
    ocelia->adjust_past(past, timestep);
    estimation_window = pnl_mat_wrap_mat_rows(historical->path_, estimation_start, estimation_end);  
    mc->price_and_delta(past, estimation_window, 0, prix, prix_std_dev, delta, delta_std_dev);

    // REBALANCING PORTFOLIO
    portfolio = new HedgingPortfolio(prix, delta, share_values, rates, val_liquidative_initiale);
    rebalancement_horizon = 30;

    // ONLY THING TO CHANGE
    simulate_next();

    // DELETES
    delete historical;
    pnl_mat_free(&ocelia_path);
    pnl_mat_free(&past);
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
