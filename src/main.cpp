#include <iostream>
#include "pnl/pnl_vector.h"
#include "IUnderlying.h"
#include "DomesticUnderlying.h"
#include "ForeignUnderlying.h"
#include "QuantoOption.h"
#include "BlackScholesModel.h"
#include <ctime>
#include "pnl/pnl_random.h"
#include "StandardMonteCarloPricer.h"
#include "pnl/pnl_finance.h"
#include "Ocelia.h"
#include "DateTime.h"

// TODO: voir comment générer les .dll 
// TODO: TEST UNITAIRES en googletest
// TODO: Gestion des données
// TODO: Gestion des dates
// TODO: ajouter classe taux d'intéret ?
// TODO: ajouter des classes pour séparer les zc et les actifs risqué dans un pf ?
// TODO: calcul du sigma pour généraliser avec des dimensions > 2
// TODO: on a implémenté pour un quanto avec 2 sous jacents (zc et risqué) -> vérifier que le programme reste cohérent pour un instrment avec 3, 4, etc sous-jacents
// TODO: pricing en t
// TODO: mettre des const dans les fonctions au bons endroits
// TODO: abstraire pour des taux intérets non constants
// TODO: implémenter des MC + opti ?
// TODO: bcp + tard: feeder des données ?

/** CONVENTION QUANTO POUR L'INSTANT : zc en ligne 0 et risqué en ligne 1 */

// TODO: tests unitaires à mettre dans des googletest
// void datetime_tests(){
//     DateTime *dt1 = new DateTime(1, 1, 1995);
//     DateTime *dt2 = new DateTime(2, 2, 1995);
//     DateTime *dt3 = new DateTime(1, 1, 1995);

//     // compare
//     std::cout << "dt1 compare dt2 ? (-1 attendu) : " << dt1->compare(dt2) << std::endl;
//     std::cout << "dt2 compare dt1 ? (1 attendu) : " << dt2->compare(dt1) << std::endl;
//     std::cout << "dt1 compare dt3 ? (0 attendu) : " << dt1->compare(dt3) << std::endl;
//     std::cout << "dt3 compare dt1 ? (0 attendu) : " << dt3->compare(dt1) << std::endl;

//     // std cout
//     std::cout << "(1/1/1995 attendu) : " << dt1 << std::endl;
// }

// void ocelia_test(){
//     Ocelia *ocelia = new Ocelia(0, 0, 0.0, 0.0, 0.0, 0.0, 0.0);
//     ocelia->fill_dates_perf();
//     ocelia->fill_dates_valeurs();

//     ocelia->payoff(NULL);
//     // datetime_tests();

// }

void format_test(){

    /** DATA **/
    double T = 1;
    double nbTimeSteps = 365;
    double rf = 0.05;
    double K = 90.0;
    double nbProduits = 2;
    PnlVect *rd = pnl_vect_create_from_scalar(nbTimeSteps+1, 0.03); // taux constants pour l'instant
    double sigma_tx_change = 0.05;
    double sigma_actif = 0.1;
    double spot_actif_sans_risque = 1;
    double spot_actif_risque = 100;
    double nbSimul = 1000;
    double spot_taux_change_initial = 1.2;
    double rho = 0.2; // corrélation entre zc et actif risqué étranger
    double h = 0.01;
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, std::time(NULL));

    // TODO : ce calcul du sigma ne fonctionnera (enfin il dépend de la taille et du contenu de la matrice du pf de couverture)
    PnlMat* sigma = pnl_mat_create(nbProduits, nbProduits); // c'est une matrice de covariance
    MLET(sigma, 0, 0) = sigma_tx_change * sigma_tx_change;
    MLET(sigma, 1, 1) = sigma_actif * sigma_actif;
    MLET(sigma, 0, 1) = sigma_tx_change * sigma_actif * rho;
    MLET(sigma, 1, 0) = sigma_tx_change * sigma_actif * rho;
    pnl_mat_chol(sigma);
    MLET(sigma, 1, 0) += MGET(sigma, 0, 0); // ce calcul dépend de où ce trouve dans la matrice les actifs risqués et non risqués
    MLET(sigma, 1, 1) += MGET(sigma, 0, 1);

    double zc_spot = spot_actif_sans_risque*spot_taux_change_initial;
    double spot = spot_actif_risque*spot_taux_change_initial;

    ForeignUnderlying *foreign_stock = new ForeignUnderlying(spot, zc_spot);
    IUnderlying **underlyings = new IUnderlying*[1];
    underlyings[0] = foreign_stock;
    IDerivative *quanto = new QuantoOption(T, nbTimeSteps, 1, rf, K, underlyings);

    IModel *model = new BlackScholesModel(quanto, rd, sigma, nbTimeSteps, rng);
    model->price_all();

    // IPricer *pricer = new StandardMonteCarloPricer();
    // pricer->simulate();
}

int main(){
    format_test();
    // quanto_test();
    // ocelia_test();
}