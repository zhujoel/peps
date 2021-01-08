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
    double sigma_tx_change = 0.05;
    double sigma_actif = 0.1;
    double spot_actif_sans_risque = 1;
    double spot_actif_risque = 100;
    double nbSimul = 3;
    double spot_taux_change_initial = 1.2;
    double rho = 0.2; // corrélation entre zc et actif risqué étranger
    double h = 0.01;

    // calcul du sigma
    PnlMat* sigma = pnl_mat_create(nbProduits, nbProduits); // previously rho
    MLET(sigma, 0, 0) = sigma_tx_change * sigma_tx_change;
    MLET(sigma, 1, 1) = sigma_actif * sigma_actif;
    MLET(sigma, 0, 1) = sigma_tx_change * sigma_actif * rho;
    MLET(sigma, 1, 0) = sigma_tx_change * sigma_actif * rho;
    pnl_mat_chol(sigma);
    MLET(sigma, 1, 0) += MGET(sigma, 0, 0); // ce calcul dépend de où ce trouve dans la matrice les actifs risqués et non risqués
    MLET(sigma, 1, 1) += MGET(sigma, 0, 1);

    QuantoOption *quanto = new QuantoOption(T, nbTimeSteps, nbProduits, rf, K) ;

    PnlVect *spot = pnl_vect_create(2);
    LET(spot, 0) = spot_actif_sans_risque*spot_taux_change_initial;
    LET(spot, 1) = spot_actif_risque*spot_taux_change_initial;

    BlackScholesModel *model = new BlackScholesModel(nbProduits, rd, sigma, spot);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, std::time(NULL));

    StandardMonteCarloPricer *pricer = new StandardMonteCarloPricer(model, quanto, rng, h, nbSimul);

    // outputs
    // price
    double prix = 0.0;
    double prix_std_dev = 0.0;
    pricer->price(prix, prix_std_dev);
    std::cout << "p1 : " << prix << " std dev : " << prix_std_dev << std::endl;

    double prix2 = 0.0;
    double delta2 = 0.0;
    double sigma_actif_converti = sqrt(sigma_tx_change*sigma_tx_change+sigma_actif*sigma_actif+2*rho*sigma_tx_change*sigma_actif);
    pnl_cf_call_bs(spot_taux_change_initial*spot_actif_risque, K, T, rd, 0, sigma_actif_converti, &prix2, &delta2);
    std::cout << "p2 : " << prix2 << " delta: " << delta2<< std::endl;

    std::cout << "price est dedans : " << (abs(prix2 - prix) <= 1.96*prix_std_dev) << std::endl;

    // delta
    PnlVect* delta = pnl_vect_create(quanto->size_);
    PnlVect* delta_std_dev = pnl_vect_create(quanto->size_);
    pricer->delta(delta, delta_std_dev);
    std::cout << "delta 1: " << std::endl;
    pnl_vect_print(delta);
    pnl_vect_print(delta_std_dev);

    std::cout << "delta est dedans : " << (abs(GET(delta, 1) - delta2) <= 1.96*GET(delta_std_dev, 1)) << std::endl;

    std::cout << "delta sous-jacent théorique : " << std::endl;
    double delta_zc_sans_risque = spot_actif_risque*delta2 * exp(-rf*0);
    std::cout << delta_zc_sans_risque << std::endl;
}