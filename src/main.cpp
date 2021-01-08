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
    double nbTimeSteps = 365;
    double rf = 0.05;
    double K = 90.0;
    double nbProduits = 2;
    double rd = 0.03;
    double sigma_tx_change = 0.15;
    double sigma_actif = 0.1;
    double spot_actif_sans_risque = 1;
    double spot_actif_risque = 100;
    double nbSimul = 10000;
    double spot_taux_change_initial = 1.2;
    double rho = 0.2;
    PnlMat* cov = pnl_mat_create(nbProduits, nbProduits); // previously rho
    MLET(cov, 0, 0) = sigma_tx_change * sigma_tx_change;
    MLET(cov, 1, 1) = sigma_actif * sigma_actif;
    MLET(cov, 0, 1) = sigma_tx_change * sigma_actif * rho;
    MLET(cov, 1, 0) = sigma_tx_change * sigma_actif * rho;

    QuantoOption *quanto = new QuantoOption(T, nbTimeSteps, nbProduits, rf, K) ;

    PnlVect *spot = pnl_vect_create(2);
    LET(spot, 0) = spot_actif_sans_risque*spot_taux_change_initial;
    LET(spot, 1) = spot_actif_risque*spot_taux_change_initial;

    PnlVect *sigma = pnl_vect_create(2);
    LET(sigma, 0) = sigma_tx_change;
    LET(sigma, 1) = sigma_actif;

    BlackScholesModel *model = new BlackScholesModel(nbProduits, rd, cov, spot, sigma);

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
    // double spot_actif_converti = spot_actif_risque*1*exp(-(rf-rd-rho*sigma_actif)*T);
    pnl_cf_call_bs(spot_taux_change_initial*spot_actif_risque, K, T, rd, 0, sigma_actif_converti, &prix2, &prix_std_dev2);

    std::cout << "p2 : " << prix2 << std::endl;

    std::cout << "est dedans : " << (abs(prix2 - prix) <= 1.96*prix_std_dev) << std::endl;
}