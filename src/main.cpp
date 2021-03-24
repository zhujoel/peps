#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
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
std::vector<DateTime*> ocelia_dates; // tous les jours ouvrés d'océlia du début à la fin
std::vector<DateTime*> all_relevant_dates; // rassemblement de dates_semestrielles et dates_valeurs_n_ans
std::vector<DateTime*> dates_semestrielles;
std::vector<DateTime*> dates_valeurs_n_ans;
PnlMat *ocelia_path = pnl_mat_new(); // les prix d'océlia lors des dates de constatation seulement
int past_index;
int estimation_end;
int estimation_start;

// INTEREST RATES
InterestRate *rates;

// OCELIA
double val_liquidative_initiale = 100.;
double T = 8.;
int size = 7;
int nb_sous_jacents = 4;
Ocelia *ocelia;

// BLACK-SCHOLES
int nb_jours_ouvres;
double regular_timestep;
int nbTimeSteps;
PnlVectInt *all_relevant_dates_indices = pnl_vect_int_new();
PnlVect *computed_t_;
IModel *model;

// MONTE-CARLO
double fdStep = 0.1;
PnlRng *rng;
IPricer *mc;

// PAYOFF
double real_payoff;
double real_date_payoff;
DateTime* real_datetime_payoff;

// PRICING
double prix = 0.;
double prix_std_dev = 0.;
PnlVect *delta;
PnlVect *delta_std_dev;
PnlVect *spot = pnl_vect_new();
PnlMat estimation_window;

// REBALANCING PORTFOLIO
HedgingPortfolio *portfolio;

// SIMULATION PARAMETERS
int horizon_estimation = 500;
int nbSamples = 100;
int rebalancement_horizon = 30;
int nb_dates_a_simuler = 2000;

void simulate_all()
{
    output_stream << "Date,k,t,prix,prix_std_dev,V1,V2,Pf_couverture,Pnl,Valeur_liquidative,Tracking_error,Ocelia_Valeur_liquidative" << std::endl;
    delta_stream << "Date,k,delta,delta_std_dev" << std::endl;
    output_stream << historical->dates_[past_index] << "," << 0 << "," << 0 << "," << prix << "," << prix_std_dev << ",";
    output_stream << portfolio->V1_ << "," << portfolio->V2_ << "," << portfolio->get_portfolio_value(0, spot) << ",";
    output_stream << portfolio->get_FinalPnL(0, prix, spot) << "," << portfolio->get_valeur_liquidative(0, spot) << ",";
    output_stream << portfolio->get_tracking_error(0, prix, spot) << "," << MGET(historical->derivative_path_, past_index, 0) << std::endl;
    delta_stream << historical->dates_[past_index] << "," << 0 << "," << delta << "," << delta_std_dev << std::endl;

    int constatation_cnt = 1;
    // PRICING & HEADGING en t
    for(int k = 1; k <= nb_dates_a_simuler; ++k)
    {
        if (((historical->dates_[past_index+k])->compare(real_datetime_payoff))==1) {
            std::cout << "Un payoff de " << real_payoff << " à été versé au client le " << real_datetime_payoff << std::endl;
            break;
        }

        double t = k*regular_timestep;

        // compute sigma and volatility
        estimation_window = pnl_mat_wrap_mat_rows(historical->path_, estimation_start+k, estimation_end+k);  
        compute_sigma(model->sigma_, &estimation_window, 0, estimation_window.m-1);
        ocelia->adjust_sigma(model->sigma_);
        compute_volatility(model->volatility_, model->sigma_);

        // compute spot
        pnl_mat_get_row(spot, historical->path_, past_index+k);
        ocelia->adjust_spot(spot, t);

        // rebalance
        if(k%rebalancement_horizon==0){
            mc->price_and_delta(spot, t, prix, prix_std_dev, delta, delta_std_dev);
            portfolio->rebalancing(t, delta, spot);
            delta_stream << historical->dates_[past_index+k] << "," << k << "," << delta << "," << delta_std_dev << std::endl;
        }

        // price
        mc->price(spot, t, prix, prix_std_dev);
        output_stream << historical->dates_[past_index+k] << "," << k << "," << t << "," << prix << "," << prix_std_dev << ",";
        output_stream << portfolio->V1_ << "," << portfolio->V2_ << "," << portfolio->get_portfolio_value(t, spot) << ",";
        output_stream << portfolio->get_FinalPnL(t, prix, spot) << "," << portfolio->get_valeur_liquidative(t, spot) << ",";
        output_stream << portfolio->get_tracking_error(t, prix, spot) << "," << MGET(historical->derivative_path_, past_index+k, 0) << std::endl;

        // si on est a une date de constatation, on ajoute le vrai prix à path de façon définitive pour les prochaines simulations
        if(k == GET_INT(all_relevant_dates_indices, constatation_cnt)){
            pnl_mat_set_row(mc->path_, spot, constatation_cnt++);
        }
    }
}

// Does not work if no file are indicated
void simulate_next()
{
    /** PARSE LAST LINE OF FILES */
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

    /** SET VARIABLES TO PREVIOUS DATE TO SIMULATE NEXT STEP **/
    // PARSE OUTPUT DATA
    int previous_k = std::stoi(output_last_line_split[1]);
    prix = std::stod(output_last_line_split[3]);
    prix_std_dev = std::stod(output_last_line_split[4]);
    portfolio->V1_ = std::stod(output_last_line_split[5]);
    portfolio->V2_ = std::stod(output_last_line_split[6]);
    // PARSE DELTA DATA
    std::string delta_split[size];
    std::string delta_std_dev_split[size];
    split(delta_split, delta_last_line_split[2], ' ');
    split(delta_std_dev_split, delta_last_line_split[3], ' ');
    for(int i = 0; i < size; ++i){
        LET(delta, i) = std::stod(delta_split[i]);
        LET(delta_std_dev, i) = std::stod(delta_std_dev_split[i]);
        LET(portfolio->delta_, i) = std::stod(delta_split[i]);
    }
    portfolio->last_rebalancing_t_ = std::stod(delta_last_line_split[1])*regular_timestep;

    int constatation_cnt = first_index_gte(computed_t_, previous_k*regular_timestep);
    int path_cnt = 0;
    // ADDS ALL CONSTATATION DATA INTO PATH
    for(int i = 0; i < constatation_cnt; ++i){
        pnl_mat_get_row(spot, historical->path_, past_index+GET_INT(all_relevant_dates_indices, i));
        ocelia->adjust_spot(spot, GET_INT(all_relevant_dates_indices, i)*regular_timestep);
        pnl_mat_set_row(mc->path_, spot, path_cnt++);
    }

    // PRICING & HEADGING en t
    for(int k = previous_k+1; k <= previous_k+nb_dates_a_simuler; ++k)
    {
        if (((historical->dates_[past_index+k])->compare(real_datetime_payoff))==1) {
            std::cout << "Un payoff de " << real_payoff << " à été versé au client le " << real_datetime_payoff << std::endl;
            break;
        }

        double t = k*regular_timestep;

        // compute sigma and volatility
        estimation_window = pnl_mat_wrap_mat_rows(historical->path_, estimation_start+k, estimation_end+k);  
        compute_sigma(model->sigma_, &estimation_window, 0, estimation_window.m-1);
        ocelia->adjust_sigma(model->sigma_);
        compute_volatility(model->volatility_, model->sigma_);

        // compute spot
        pnl_mat_get_row(spot, historical->path_, past_index+k);
        ocelia->adjust_spot(spot, t);

        // rebalance
        if(k%rebalancement_horizon==0){
            mc->price_and_delta(spot, t, prix, prix_std_dev, delta, delta_std_dev);
            portfolio->rebalancing(t, delta, spot);
            delta_stream << historical->dates_[past_index+k] << "," << k << "," << delta << "," << delta_std_dev << std::endl;
        }

        // price
        mc->price(spot, t, prix, prix_std_dev);
        output_stream << historical->dates_[past_index+k] << "," << k << "," << t << "," << prix << "," << prix_std_dev << ",";
        output_stream << portfolio->V1_ << "," << portfolio->V2_ << "," << portfolio->get_portfolio_value(t, spot) << ",";
        output_stream << portfolio->get_FinalPnL(t, prix, spot) << "," << portfolio->get_valeur_liquidative(t, spot) << ",";
        output_stream << portfolio->get_tracking_error(t, prix, spot) << "," << MGET(historical->derivative_path_, past_index+k, 0) << std::endl;

        // si on est a une date de constatation, on ajoute le vrai prix à path de façon définitive pour les prochaines simulations
        if(k == GET_INT(all_relevant_dates_indices, constatation_cnt)){
            pnl_mat_set_row(mc->path_, spot, constatation_cnt++);
        }
    }
}

int main(int argc, char* argv[])
{
    // /** STREAMS **/
    if(argc > 2){
        set_stream_from_filename(output_stream, output_file, argv[2]);
        if(argc > 3) set_stream_from_filename(delta_stream, delta_file, argv[3]);
    }
    else{
        output_stream.rdbuf(std::cout.rdbuf());
        delta_stream.rdbuf(std::cout.rdbuf());
    }

    /** PROCESSING MARKET DATA **/
    historical = new HistoricalMarketData("Ocelia", new DateTime(1, 1, 2005), new DateTime(1, 1, 2017));
    historical->set_data();
    // GET DATE AND PRICES
    from_date_to_date(ocelia_dates, historical->dates_, new DateTime(15, 5, 2008), new DateTime(28, 4, 2016));
    parse_dates_file(all_relevant_dates, "../data/dates/all_dates_constatation.csv", 49, '-');
    parse_dates_file(dates_semestrielles, "../data/dates/dates_semest.csv", 16, '-');
    parse_dates_file(dates_valeurs_n_ans, "../data/dates/dates_valeurs_n.csv", 35, '-');
    get_prices_from_date(ocelia_path, historical->dates_, all_relevant_dates, historical->path_);
    // GET INDEXES
    past_index = get_indice_from_date(historical->dates_, new DateTime(15, 5, 2008));
    estimation_end = get_indice_from_date(historical->dates_, new DateTime(15, 5, 2008));
    estimation_start = estimation_end - horizon_estimation;
    
    /** INTEREST RATES **/
    rates = new InterestRate(new DateTime(15, 5, 2008), historical->dates_, historical->interest_path_);
    
    /** BLACK-SCHOLES **/
    nb_jours_ouvres = ocelia_dates.size();
    regular_timestep = T/nb_jours_ouvres;
    nbTimeSteps = all_relevant_dates.size();
    // CALCUL DES t_{i}
    calcul_indices_dates(all_relevant_dates_indices, ocelia_dates, all_relevant_dates);
    computed_t_ = pnl_vect_create(nbTimeSteps);
    for(int i = 0; i < computed_t_->size; ++i){
        LET(computed_t_, i) = GET_INT(all_relevant_dates_indices, i)*regular_timestep;
    }
    model = new BlackScholesModel(size, nbTimeSteps, rates, computed_t_);

    /** OCELIA **/
    ocelia = new Ocelia(T, size, nb_sous_jacents, val_liquidative_initiale, computed_t_, rates);
    ocelia->init_indices(all_relevant_dates, dates_semestrielles, dates_valeurs_n_ans);

    /** CALCUL DU PAYOFF VERSÉ DANS LA VRAIE VIE **/
    ocelia->adjust_past(ocelia_path);
    real_payoff = ocelia->payoff(ocelia_path);
    real_date_payoff = ocelia->get_annee_payoff(); 
    if(real_date_payoff == 4) real_datetime_payoff = new DateTime(11, 5, 2012); // par rapport à la date de début : 15/05/2008
    else if(real_date_payoff == 5) real_datetime_payoff = new DateTime(13, 5, 2013);
    else if(real_date_payoff == 6) real_datetime_payoff = new DateTime(13, 5, 2014);
    else if(real_date_payoff == 7) real_datetime_payoff = new DateTime(13, 5, 2015);
    else if(real_date_payoff == 8) real_datetime_payoff = new DateTime(13, 5, 2016);

    /** MONTE CARLO **/
    rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, std::time(NULL));
    mc = new StandardMonteCarloPricer(model, ocelia, rng, fdStep, nbSamples);

    /** PRICING & HEDGING en 0 **/
    delta = pnl_vect_create(size);
    delta_std_dev = pnl_vect_create(size);
    // GET AND ADJUST SPOT
    pnl_mat_get_row(spot, historical->path_, past_index);
    ocelia->adjust_spot(spot, 0.);
    // COMPUTE SIGMA AND VOLATILITY
    estimation_window = pnl_mat_wrap_mat_rows(historical->path_, estimation_start, estimation_end);
    compute_sigma(model->sigma_, &estimation_window, 0, estimation_window.m-1);
    ocelia->adjust_sigma(model->sigma_);
    compute_volatility(model->volatility_, model->sigma_);
    mc->price_and_delta(spot, 0, prix, prix_std_dev, delta, delta_std_dev);
    pnl_mat_set_row(mc->path_, spot, 0); // on fixe le spot dans le path car on a plus besoin de le calculer

    /** REBALANCING PORTFOLIO **/
    portfolio = new HedgingPortfolio(prix, delta, spot, rates, val_liquidative_initiale);

    /** PARSE INPUT **/
    if(argc == 1){
        std::cout << "ERROR: command is: ./main (all | next) [output_filepath] [delta_filepath]" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1], "all") == 0){
        simulate_all();
    }
    else if(strcmp(argv[1], "next") == 0){
        simulate_next();
    }
    else{
        exit(EXIT_FAILURE);
    }

    /** DELETES **/
    // STREAMS
    if(argc == 2){
        output_file.close();
        if(argc > 3) delta_file.close();
    }
    // MARKET DATA
    delete historical;
    delete_date_vector(all_relevant_dates);
    delete_date_vector(dates_semestrielles);
    delete_date_vector(dates_valeurs_n_ans);
    pnl_mat_free(&ocelia_path);
    // INTEREST RATES
    delete rates;
    // OCELIA
    delete ocelia;
    // BLACK-SCHOLES
    pnl_vect_int_free(&all_relevant_dates_indices);
    pnl_vect_free(&computed_t_);
    delete model;
    // MONTE-CARLO
    pnl_rng_free(&rng);
    delete mc;
    // PAYOFF
    delete real_datetime_payoff;
    // PRICING
    pnl_vect_free(&delta);
    pnl_vect_free(&delta_std_dev);
    pnl_vect_free(&spot);
    // PORTFOLIO
    delete portfolio;
}
