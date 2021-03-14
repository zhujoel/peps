#include "gtest/gtest.h"
#include "derivatives/QuantoOption.h"
#include "models/BlackScholesModel.h"
#include "pricers/StandardMonteCarloPricer.h"
#include "libs/DateTimeVector.h"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"

class QuantoTest: public ::testing::Test{
    protected:
        IDerivative *quanto;
        IModel *model;
        IPricer *pricer;
        PnlRng *rng;
        PnlVect *spot;
        InterestRate *rates;
        PnlVect *computed_ti;
        PnlMat *interest_path;
        std::vector<DateTime*> all_dates;
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

        virtual void SetUp()
        {
            this->rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(this->rng, std::time(NULL));

            // DATA
            this->spot = pnl_vect_create(2);
            LET(this->spot, 0) = spot_actif_sans_risque*spot_taux_change_initial;
            LET(this->spot, 1) = spot_actif_risque*spot_taux_change_initial;

            // INTEREST
            this->interest_path = pnl_mat_create_from_scalar(1, 4, rd);
            DateTime *current_date = new DateTime(1, 1, 2010);
            all_dates.push_back(current_date);
            this->rates = new InterestRate(0, current_date, all_dates, interest_path);

            // // QUANTO
            this->quanto = new QuantoOption(T, nbProduits, rf, K) ;

            // BLACK SCHOLES
            this->computed_ti = pnl_vect_create(this->nbTimeSteps);
            double timestep = this->T / this->nbTimeSteps;
            for(int i = 0; i < this->nbTimeSteps; ++i){
                LET(this->computed_ti, i) = timestep * i;
            }
            this->model = new BlackScholesModel(this->nbProduits, this->nbTimeSteps, this->rates, this->computed_ti);
            MLET(this->model->sigma_, 0, 0) = sigma_tx_change * sigma_tx_change;
            MLET(this->model->sigma_, 1, 1) = sigma_actif * sigma_actif;
            MLET(this->model->sigma_, 0, 1) = sigma_tx_change * sigma_actif * rho;
            MLET(this->model->sigma_, 1, 0) = sigma_tx_change * sigma_actif * rho;
            pnl_mat_chol(this->model->sigma_);
            MLET(this->model->sigma_, 1, 0) += MGET(this->model->sigma_, 0, 0); // ce calcul dépend de où ce trouve dans la matrice les actifs risqués et non risqués
            MLET(this->model->sigma_, 1, 1) += MGET(this->model->sigma_, 0, 1);

            LET(this->model->volatility_, 0) = MGET(this->model->sigma_, 0, 0);
            LET(this->model->volatility_, 1) = MGET(this->model->sigma_, 1, 1);

            // MONTE CARLO
            this->pricer = new StandardMonteCarloPricer(this->model, this->quanto, rng, h, nbSimul);
        }

        virtual void TearDown(){
            pnl_rng_free(&this->rng);
            pnl_vect_free(&this->spot);
            pnl_vect_free(&this->computed_ti);
            pnl_mat_free(&this->interest_path);
            delete(this->quanto);
            delete(this->model);
            delete(this->rates);
            delete(this->pricer);
            delete_date_vector(this->all_dates);
        }
};

TEST_F(QuantoTest, price){
    // prix théorique avec pnl
    double prix_theorique = 0.0;
    double delta_theorique = 0.0;
    double sigma_actif_converti = sqrt(sigma_tx_change*sigma_tx_change+sigma_actif*sigma_actif+2*rho*sigma_tx_change*sigma_actif);
    pnl_cf_call_bs(spot_taux_change_initial*spot_actif_risque, K, T, rd, 0, sigma_actif_converti, &prix_theorique, &delta_theorique);

    // simulation
    double prix = 0.0;
    double prix_std_dev = 0.0;
    PnlVect* delta = pnl_vect_create_from_zero(this->quanto->size_);
    PnlVect* delta_std_dev = pnl_vect_create_from_zero(this->quanto->size_);
    pricer->price_and_delta(this->spot, 0, prix, prix_std_dev, delta, delta_std_dev);

    EXPECT_EQ(1,(abs(prix_theorique - prix) <= 1.96*prix_std_dev));
    EXPECT_EQ(1,(abs(GET(delta, 1) - delta_theorique) <= 1.96*GET(delta_std_dev, 1)));
    pnl_vect_free(&delta);
    pnl_vect_free(&delta_std_dev);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
