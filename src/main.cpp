#include <iostream>
#include "pnl/pnl_vector.h"
#include "QuantoOption.h"
#include "BlackScholesModel.h"
#include <ctime>
#include "pnl/pnl_random.h"
#include "StandardMonteCarloPricer.h"
#include "pnl/pnl_finance.h"

int main(){
    // TEST DE PRICE UNE OPTION QUANTO
    double T = 1;
    double nbTimeSteps = 100;
    double rf = 0.1;
    double K = 100.0;
    double nbProduits = 2;
    double rd = 0.1;
    double rho = 0.;
    double sigma_tx_change = 0.15;
    double sigma_actif = 0.1;
    double spot_actif_sans_risque = 1;
    double spot_actif_risque = 100;
    double nbSimul = 10000;

    QuantoOption *quanto = new QuantoOption(T, nbTimeSteps, nbProduits, rf, K) ;

    PnlVect *spot = pnl_vect_create(2);
    LET(spot, 0) = spot_actif_sans_risque;
    LET(spot, 1) = spot_actif_risque;

    PnlVect *sigma = pnl_vect_create(2);
    LET(sigma, 0) = sigma_tx_change;
    LET(sigma, 1) = sigma_actif;

    BlackScholesModel *model = new BlackScholesModel(nbProduits, rd, rho, spot, sigma);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, std::time(NULL));

    StandardMonteCarloPricer *pricer = new StandardMonteCarloPricer(model, quanto, rng, T/nbTimeSteps, nbSimul);

    // // outputs
    double prix = 0.0;
    double prix_std_dev = 0.0;
    pricer->price(prix, prix_std_dev);
    std::cout << "p1 : " << prix << " std dev : " << prix_std_dev << std::endl;

    double prix2 = 0.0;
    double prix_std_dev2 = 0.0;
    double sigma_actif_converti = sqrt(sigma_tx_change*sigma_tx_change+sigma_actif*sigma_actif+2*rho*sigma_tx_change*sigma_actif);
    double spot_actif_converti = spot_actif_risque*1*exp(-(rf-rd-rho*sigma_actif)*T);
    pnl_cf_call_bs(spot_actif_converti, K, T, rd, 0, sigma_actif_converti, &prix2, &prix_std_dev2);

    std::cout << "p2 : " << prix2 << std::endl;
}