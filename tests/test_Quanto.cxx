#include "gtest/gtest.h"
#include "derivatives/QuantoOption.h"
#include "models/BlackScholesModel.h"
#include "pricers/StandardMonteCarloPricer.h"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"
#include <fstream>
#include <vector>

class QuantoTest: public ::testing::Test{
    protected:
        IDerivative *quanto;
        IModel *model;
        IPricer *pricer;
        PnlRng *rng;
        PnlMat *sigma;
        PnlMat *spot;
        InterestRate* rates;
        double T = 1;
        double nbTimeSteps = 365;
        double rf = 0.05;
        double K = 90.0;
        double nbProduits = 2;
        double rd = 0.03; // taux constant
        double sigma_tx_change = 0.05;
        double sigma_actif = 0.1;
        double spot_actif_sans_risque = 1;
        double spot_actif_risque = 100;
        double nbSimul = 1000;
        double spot_taux_change_initial = 1.2;
        double rho = 0.2; // corrélation entre le taux de change et actif risqué étranger
        double h = 0.01;

        virtual void SetUp(){

            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));

            this->sigma = pnl_mat_create(nbProduits, nbProduits); // c'est une matrice de covariance
            MLET(this->sigma, 0, 0) = sigma_tx_change * sigma_tx_change;
            MLET(this->sigma, 1, 1) = sigma_actif * sigma_actif;
            MLET(this->sigma, 0, 1) = sigma_tx_change * sigma_actif * rho;
            MLET(this->sigma, 1, 0) = sigma_tx_change * sigma_actif * rho;
            pnl_mat_chol(this->sigma);
            MLET(this->sigma, 1, 0) += MGET(this->sigma, 0, 0); // ce calcul dépend de où ce trouve dans la matrice les actifs risqués et non risqués
            MLET(this->sigma, 1, 1) += MGET(this->sigma, 0, 1);

            this->spot = pnl_mat_create(1, 2);
            MLET(this->spot, 0, 0) = spot_actif_sans_risque*spot_taux_change_initial;
            MLET(this->spot, 0, 1) = spot_actif_risque*spot_taux_change_initial;

            PnlMat* interest_path = pnl_mat_create_from_scalar(1, 4, rd);
            DateTime* curent_date = new DateTime(1, 1, 2010);
            std::vector<DateTime*> all_dates;
            all_dates.push_back(curent_date);
            this->rates = new InterestRate(0, curent_date, all_dates, interest_path);

            this->quanto = new QuantoOption(T, nbProduits, rf, K) ;
            this->model = new BlackScholesModel(nbProduits, nbTimeSteps, this->rates);
            MLET(this->model->sigma_, 0, 0) = sigma_tx_change * sigma_tx_change;
            MLET(this->model->sigma_, 1, 1) = sigma_actif * sigma_actif;
            MLET(this->model->sigma_, 0, 1) = sigma_tx_change * sigma_actif * rho;
            MLET(this->model->sigma_, 1, 0) = sigma_tx_change * sigma_actif * rho;
            pnl_mat_chol(this->model->sigma_);
            MLET(this->model->sigma_, 1, 0) += MGET(this->model->sigma_, 0, 0); // ce calcul dépend de où ce trouve dans la matrice les actifs risqués et non risqués
            MLET(this->model->sigma_, 1, 1) += MGET(this->model->sigma_, 0, 1);

            LET(this->model->volatility_, 0) = MGET(this->model->sigma_, 0, 0);
            LET(this->model->volatility_, 1) = MGET(this->model->sigma_, 1, 1);
            this->pricer = new StandardMonteCarloPricer(this->model, this->quanto, rng, h, nbSimul);
        }

        virtual void TearDown(){
            pnl_rng_free(&this->rng);
            pnl_mat_free(&this->sigma);
            pnl_mat_free(&this->spot);
            delete(this->quanto);
            delete(this->model);
            delete(this->pricer);
        }
};

TEST_F(QuantoTest, price){
    // // prix théorique avec pnl
    // double prix_theorique = 0.0;
    // double delta_theorique = 0.0;
    // double sigma_actif_converti = sqrt(sigma_tx_change*sigma_tx_change+sigma_actif*sigma_actif+2*rho*sigma_tx_change*sigma_actif);
    // pnl_cf_call_bs(spot_taux_change_initial*spot_actif_risque, K, T, rd, 0, sigma_actif_converti, &prix_theorique, &delta_theorique);
    // // std::cout << "prix théorique : " << prix_theorique << " delta théorique : " << delta_theorique << std::endl << std::endl;

    // // simulation
    // double prix = 0.0;
    // double prix_std_dev = 0.0;
    // PnlVect* delta = pnl_vect_create_from_zero(this->quanto->size_);
    // PnlVect* delta_std_dev = pnl_vect_create_from_zero(this->quanto->size_);
    // pricer->price_and_delta(this->spot, , 0, prix, prix_std_dev, delta, delta_std_dev);

    // EXPECT_EQ(1,(abs(prix_theorique - prix) <= 1.96*prix_std_dev));
    // EXPECT_EQ(1,(abs(GET(delta, 1) - delta_theorique) <= 1.96*GET(delta_std_dev, 1)));
    // pnl_vect_free(&delta);
    // pnl_vect_free(&delta_std_dev);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
