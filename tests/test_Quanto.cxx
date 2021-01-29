#include "gtest/gtest.h"
#include "QuantoOption.h"
#include "BlackScholesModel.h"
#include "StandardMonteCarloPricer.h"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"
#include <fstream>

class QuantoTest: public ::testing::Test{
    protected:
        IDerivative *quanto;
        IModel *model;
        IPricer *pricer;
        PnlRng *rng;
        PnlMat *sigma;
        PnlVect *spot;
        PnlVect *volatility;
        double T = 1;
        double nbTimeSteps = 365;
        double rf = 0.05;
        double K = 90.0;
        double nbProduits = 2;
        double rd = 0.03; // taux constants pour l'instant
        double sigma_tx_change = 0.05;
        double sigma_actif = 0.1;
        double spot_actif_sans_risque = 1;
        double spot_actif_risque = 100;
        double nbSimul = 1000;
        double spot_taux_change_initial = 1.2;
        double rho = 0.2; // corrélation entre zc et actif risqué étranger
        double h = 0.01;

        virtual void SetUp(){

            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));

            // TODO : ce calcul du sigma ne fonctionnera (enfin il dépend de la taille et du contenu de la matrice du pf de couverture)
            this->sigma = pnl_mat_create(nbProduits, nbProduits); // c'est une matrice de covariance
            MLET(this->sigma, 0, 0) = sigma_tx_change * sigma_tx_change;
            MLET(this->sigma, 1, 1) = sigma_actif * sigma_actif;
            MLET(this->sigma, 0, 1) = sigma_tx_change * sigma_actif * rho;
            MLET(this->sigma, 1, 0) = sigma_tx_change * sigma_actif * rho;
            pnl_mat_chol(this->sigma);
            MLET(this->sigma, 1, 0) += MGET(this->sigma, 0, 0); // ce calcul dépend de où ce trouve dans la matrice les actifs risqués et non risqués
            MLET(this->sigma, 1, 1) += MGET(this->sigma, 0, 1);

            this->volatility = pnl_vect_create(2);
            PnlVect *tmp = pnl_vect_create(nbProduits);

            for(int i = 0; i < nbProduits; ++i){
                pnl_mat_get_row(tmp, this->sigma, i);
                double sigma_d = pnl_vect_norm_two(tmp);
                LET(this->volatility, i) = sigma_d;
            }

            this->spot = pnl_vect_create(2);
            LET(this->spot, 0) = spot_actif_sans_risque*spot_taux_change_initial;
            LET(this->spot, 1) = spot_actif_risque*spot_taux_change_initial;

            this->quanto = new QuantoOption(T, nbTimeSteps, nbProduits, rf, K) ;
            this->model = new BlackScholesModel(nbProduits, rd, this->sigma, this->volatility, this->spot);
            this->pricer = new StandardMonteCarloPricer(this->model, this->quanto, rng, h, nbSimul);

            pnl_vect_free(&tmp);
        }

        virtual void TearDown(){
            pnl_mat_free(&this->sigma);
            pnl_vect_free(&this->volatility);
            delete(this->quanto);
            pnl_vect_free(&this->spot);
            delete(this->model);
            pnl_rng_free(&this->rng);
            delete(this->pricer);
        }
};

// TODO: add asserts
TEST_F(QuantoTest, price){
    // prix théorique avec pnl
    double prix2 = 0.0;
    double delta2 = 0.0;
    double sigma_actif_converti = sqrt(sigma_tx_change*sigma_tx_change+sigma_actif*sigma_actif+2*rho*sigma_tx_change*sigma_actif);
    pnl_cf_call_bs(spot_taux_change_initial*spot_actif_risque, K, T, rd, 0, sigma_actif_converti, &prix2, &delta2);
    std::cout << "prix théorique : " << prix2 << " delta théorique : " << delta2<< std::endl;

    // simulation
    double prix = 0.0;
    double prix_std_dev = 0.0;
    PnlVect* delta = pnl_vect_create(this->quanto->size_);
    PnlVect* delta_std_dev = pnl_vect_create(this->quanto->size_);
    pricer->simulate(prix, prix_std_dev, delta, delta_std_dev);
    std::cout << "prix simulé : " << prix << " std dev : " << prix_std_dev << std::endl;
    std::cout << "price est dedans : " << (abs(prix2 - prix) <= 1.96*prix_std_dev) << std::endl;

    std::cout << "delta simulé : " << std::endl;
    pnl_vect_print(delta);
    pnl_vect_print(delta_std_dev);
    std::cout << "delta est dedans : " << (abs(GET(delta, 1) - delta2) <= 1.96*GET(delta_std_dev, 1)) << std::endl;

    double delta_zc_sans_risque = spot_actif_risque*delta2 * exp(-rf*0);
    std::cout << "delta sous-jacent théorique : " << delta_zc_sans_risque << std::endl;

    pnl_vect_free(&delta);
    pnl_vect_free(&delta_std_dev);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
