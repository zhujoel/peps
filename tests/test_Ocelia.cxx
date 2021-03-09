#include "gtest/gtest.h"
#include "derivatives/Ocelia.h"
#include "market_data/IMarketData.h"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include <vector>

class OceliaTest: public ::testing::Test{
    protected:
        Ocelia *ocelia;
        std::vector<DateTime*> all_dates;
        PnlMat *path;
        std::vector<DateTime*> dates_semestrielles;
        std::vector<DateTime*> dates_valeurs_n_ans;

        virtual void SetUp(){
            parse_dates_file(this->all_dates, "../tests/test_data/ocelia/all_dates_constatation.csv", 49, '-');
            this->path = pnl_mat_create(49, 7);
            for(int i = 0; i < 49; ++i){
                MLET(path, i, 0) = 100+i;
                MLET(path, i, 1) = 200+i;
                MLET(path, i, 2) = 300+i;
                MLET(path, i, 3) = 400+i;
                MLET(path, i, 4) = 1;
                MLET(path, i, 5) = 1;
                MLET(path, i, 6) = 1;
            }

            this->ocelia = new Ocelia(1, 7, 4, 100);
            parse_dates_file(this->dates_semestrielles, "../tests/test_data/ocelia/dates_semest.csv", 16, '-');
            parse_dates_file(this->dates_valeurs_n_ans, "../tests/test_data/ocelia/dates_valeurs_n.csv", 35, '-');
            this->ocelia->init_indices(this->all_dates, this->dates_semestrielles, this->dates_valeurs_n_ans);
        }

        virtual void TearDown(){
            delete this->ocelia;
            delete_date_vector(this->all_dates);
            pnl_mat_free(&this->path);
            delete_date_vector(this->dates_semestrielles);
            delete_date_vector(this->dates_valeurs_n_ans);
        }
};

TEST_F(OceliaTest, constructor_size){
    EXPECT_EQ(7, this->ocelia->size_);
}

TEST_F(OceliaTest, constructor_Maturity){
    EXPECT_EQ(1, this->ocelia->T_);
}

TEST_F(OceliaTest, constructor_date_error){
    
    std::vector<DateTime*> wrong_dates;
    parse_dates_file(wrong_dates, "../tests/test_data/ocelia/wrong_dates.csv", 48, '-');
    Ocelia *der = new Ocelia(1, 7, 4, 100);

    try{
        der->init_indices(wrong_dates, this->dates_semestrielles, this->dates_valeurs_n_ans);
    }
    catch(std::logic_error const &err){
        EXPECT_EQ(err.what(), std::string("dates_subset is not a subset of all_dates!"));
    }
    
    delete der;
    delete_date_vector(wrong_dates);
}

TEST_F(OceliaTest, indices_semestrielles_size){
    EXPECT_EQ(16, ocelia->indices_dates_semestrielles_->size);
}

TEST_F(OceliaTest, indices_n_ans_size){
    EXPECT_EQ(35, ocelia->indices_dates_valeurs_n_ans_->size);
}

TEST_F(OceliaTest, indices_semestrielles_last_value){
    EXPECT_EQ(48, GET_INT(ocelia->indices_dates_semestrielles_, 15));
}

TEST_F(OceliaTest, indices_n_ans_last_value){
    EXPECT_EQ(48, GET_INT(ocelia->indices_dates_valeurs_n_ans_, 34));
}

TEST_F(OceliaTest, valeurs_n_ans_size){
    EXPECT_EQ(ocelia->nb_sous_jacents_, ocelia->valeurs_n_ans_->size);
}

TEST_F(OceliaTest, perfs_size){
    EXPECT_EQ(ocelia->nb_sous_jacents_, ocelia->perfs_->size);
}

TEST_F(OceliaTest, valeurs_initiales_size){
    EXPECT_EQ(ocelia->nb_sous_jacents_, ocelia->valeurs_initiales_->size);
}

TEST_F(OceliaTest, nouveau_depart_size){
    EXPECT_EQ(ocelia->nb_sous_jacents_, ocelia->nouveau_depart_->size);
}

TEST_F(OceliaTest, compute_valeurs_0_ans){
    ocelia->compute_valeurs_n_ans(ocelia->valeurs_n_ans_, this->path, 0);
    EXPECT_EQ(102, GET(ocelia->valeurs_n_ans_, 0));
    EXPECT_EQ(202, GET(ocelia->valeurs_n_ans_, 1));
    EXPECT_EQ(302, GET(ocelia->valeurs_n_ans_, 2));
    EXPECT_EQ(402, GET(ocelia->valeurs_n_ans_, 3));
}

TEST_F(OceliaTest, compute_valeurs_0_ans_valeurs_initiales){
    ocelia->compute_valeurs_n_ans(ocelia->valeurs_initiales_, this->path, 0);
    EXPECT_EQ(102, GET(ocelia->valeurs_initiales_, 0));
    EXPECT_EQ(202, GET(ocelia->valeurs_initiales_, 1));
    EXPECT_EQ(302, GET(ocelia->valeurs_initiales_, 2));
    EXPECT_EQ(402, GET(ocelia->valeurs_initiales_, 3));
}

TEST_F(OceliaTest, compute_valeurs_1_ans){
    ocelia->compute_valeurs_n_ans(ocelia->valeurs_n_ans_, this->path, 1);
    EXPECT_EQ(108, GET(ocelia->valeurs_n_ans_, 0));
    EXPECT_EQ(208, GET(ocelia->valeurs_n_ans_, 1));
    EXPECT_EQ(308, GET(ocelia->valeurs_n_ans_, 2));
    EXPECT_EQ(408, GET(ocelia->valeurs_n_ans_, 3));
}

TEST_F(OceliaTest, compute_valeurs_4_ans){
    ocelia->compute_valeurs_n_ans(ocelia->valeurs_n_ans_, this->path, 4);
    EXPECT_EQ(118, GET(ocelia->valeurs_n_ans_, 0));
    EXPECT_EQ(218, GET(ocelia->valeurs_n_ans_, 1));
    EXPECT_EQ(318, GET(ocelia->valeurs_n_ans_, 2));
    EXPECT_EQ(418, GET(ocelia->valeurs_n_ans_, 3));
}

TEST_F(OceliaTest, compute_valeurs_8_ans){
    ocelia->compute_valeurs_n_ans(ocelia->valeurs_n_ans_, this->path, 8);
    EXPECT_EQ(146, GET(ocelia->valeurs_n_ans_, 0));
    EXPECT_EQ(246, GET(ocelia->valeurs_n_ans_, 1));
    EXPECT_EQ(346, GET(ocelia->valeurs_n_ans_, 2));
    EXPECT_EQ(446, GET(ocelia->valeurs_n_ans_, 3));
}

TEST_F(OceliaTest, compute_nouveau_depart){
    ocelia->compute_nouveau_depart(this->path);
    EXPECT_EQ(102, GET(ocelia->nouveau_depart_, 0));
    EXPECT_EQ(202, GET(ocelia->nouveau_depart_, 1));
    EXPECT_EQ(302, GET(ocelia->nouveau_depart_, 2));
    EXPECT_EQ(402, GET(ocelia->nouveau_depart_, 3));
}


TEST_F(OceliaTest, compute_perfs_1_ans){
    ocelia->compute_nouveau_depart(this->path);
    ocelia->compute_perfs_n_ans(ocelia->perfs_, this->path, 1);
    EXPECT_NEAR(6./102, GET(ocelia->perfs_, 0), 0.00001);
    EXPECT_NEAR(6./202, GET(ocelia->perfs_, 1), 0.00001);
    EXPECT_NEAR(6./302, GET(ocelia->perfs_, 2), 0.00001);
    EXPECT_NEAR(6./402, GET(ocelia->perfs_, 3), 0.00001);
}

TEST_F(OceliaTest, compute_perfs_4_ans){
    ocelia->compute_nouveau_depart(this->path);
    ocelia->compute_perfs_n_ans(ocelia->perfs_, this->path, 4);
    EXPECT_NEAR(16./102, GET(ocelia->perfs_, 0), 0.00001);
    EXPECT_NEAR(16./202, GET(ocelia->perfs_, 1), 0.00001);
    EXPECT_NEAR(16./302, GET(ocelia->perfs_, 2), 0.00001);
    EXPECT_NEAR(16./402, GET(ocelia->perfs_, 3), 0.00001);
}

TEST_F(OceliaTest, compute_perfs_8_ans){
    ocelia->compute_nouveau_depart(this->path);
    ocelia->compute_perfs_n_ans(ocelia->perfs_, this->path, 8);
    EXPECT_NEAR(44./102, GET(ocelia->perfs_, 0), 0.00001);
    EXPECT_NEAR(44./202, GET(ocelia->perfs_, 1), 0.00001);
    EXPECT_NEAR(44./302, GET(ocelia->perfs_, 2), 0.00001);
    EXPECT_NEAR(44./402, GET(ocelia->perfs_, 3), 0.00001);
}

TEST_F(OceliaTest, compute_perf_moyenne_panier){
    ocelia->compute_valeurs_n_ans(ocelia->valeurs_initiales_, this->path, 0);
    double moy = ocelia->compute_perf_moyenne_panier(this->path);
    EXPECT_NEAR(0.1130428, moy, 0.0000001);
}

TEST_F(OceliaTest, compute_flux_1_ans){
    double flux = ocelia->compute_flux_n_ans(this->path, 1);
    EXPECT_EQ(0.0, flux);
}

TEST_F(OceliaTest, compute_flux_5_ans){
    double flux = ocelia->compute_flux_n_ans(this->path, 5);
    EXPECT_EQ(1.32, flux);
}

TEST_F(OceliaTest, compute_flux_7_ans){
    double flux = ocelia->compute_flux_n_ans(this->path, 7);
    EXPECT_EQ(1.48, flux);
}

TEST_F(OceliaTest, compute_flux_8_ans){
    ocelia->compute_nouveau_depart(this->path);
    double flux = ocelia->compute_flux_n_ans(this->path, 8);
    EXPECT_EQ(1.56, flux);
}

TEST_F(OceliaTest, get_foreign_index_market_value){
    double value = ocelia->get_foreign_index_market_value(this->path, 1, 0);
    EXPECT_EQ(101, value);
}

TEST_F(OceliaTest, payoff){
    double payoff = ocelia->payoff(this->path);
    EXPECT_EQ(124, payoff);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}