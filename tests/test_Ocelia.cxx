#include "gtest/gtest.h"
#include "Ocelia.h"
#include "IMarketData.h"
#include "SimulatedMarketData.h"
#include "ForeignUnderlying.h"
#include <fstream>

class OceliaTest: public ::testing::Test{
    protected:
        Ocelia *ocelia;
        DateTimeVector *all_dates;
        IUnderlying **underlyings;

        virtual void SetUp(){
            this->all_dates = new DateTimeVector("../data/all_dates_constatation", 49);
            IUnderlying *eur = new ForeignUnderlying(100, 10, 365);
            IUnderlying *gbp = new ForeignUnderlying(100, 10, 365);
            IUnderlying *chf = new ForeignUnderlying(100, 10, 365);
            IUnderlying *jpy = new ForeignUnderlying(100, 10, 365);
            this->underlyings = new IUnderlying*[4];
            for(int i = 0; i < 49; ++i){
                LET(eur->price_, i) = 100+i;
                LET(gbp->price_, i) = 200+i;
                LET(chf->price_, i) = 300+i;
                LET(jpy->price_, i) = 400+i;
            }
            this->underlyings[0] = eur;
            this->underlyings[1] = gbp;
            this->underlyings[2] = chf;
            this->underlyings[3] = jpy;

            this->ocelia = new Ocelia(1, 49, 4, underlyings, all_dates);
        }

        virtual void TearDown(){
            delete this->ocelia;
            delete this->all_dates;
            for(int i = 0; i < 4; ++i){
                delete this->underlyings[i];
            }
            delete[] this->underlyings;
        }
};

TEST_F(OceliaTest, constructorSize){
    EXPECT_EQ(4, this->ocelia->size_);
}

TEST_F(OceliaTest, constructorNbTimeSteps){
    EXPECT_EQ(49, this->ocelia->nbTimeSteps_);
}

TEST_F(OceliaTest, constructorMaturity){
    EXPECT_EQ(1, this->ocelia->T_);
}

TEST_F(OceliaTest, trunc){
    PnlVect *vect = pnl_vect_create(3);
    LET(vect, 0) = 1.123456;
    LET(vect, 1) = 0.547;
    LET(vect, 2) = 3.14153;
    trunc(vect, 4);
    EXPECT_DOUBLE_EQ(1.1235, GET(vect, 0));
    EXPECT_DOUBLE_EQ(0.547, GET(vect, 1));
    EXPECT_DOUBLE_EQ(3.1415, GET(vect, 2));

    pnl_vect_free(&vect);
}

TEST_F(OceliaTest, trunc_zero){
    PnlVect *vect = pnl_vect_create(1);
    LET(vect, 0) = 0.0000000;
    trunc(vect, 4);
    EXPECT_DOUBLE_EQ(0.0000, GET(vect, 0));

    pnl_vect_free(&vect);
}

TEST_F(OceliaTest, are_all_positive_false){
    PnlVect *vect = pnl_vect_create(3);
    LET(vect, 0) = -1.123456;
    LET(vect, 1) = 0.547;
    LET(vect, 2) = 3.14153;
    EXPECT_FALSE(are_all_positive(vect));

    pnl_vect_free(&vect);
}

TEST_F(OceliaTest, are_all_positive_true){
    PnlVect *vect = pnl_vect_create(3);
    LET(vect, 0) = 1.123456;
    LET(vect, 1) = 0.547;
    LET(vect, 2) = 3.14153;
    EXPECT_TRUE(are_all_positive(vect));

    pnl_vect_free(&vect);
}

TEST_F(OceliaTest, compute_valeurs_n_ans){
    ocelia->compute_valeurs_n_ans(ocelia->valeurs_n_ans_, 0);
    EXPECT_EQ(102, GET(ocelia->valeurs_n_ans_, 0));
    EXPECT_EQ(202, GET(ocelia->valeurs_n_ans_, 1));
    EXPECT_EQ(302, GET(ocelia->valeurs_n_ans_, 2));
    EXPECT_EQ(402, GET(ocelia->valeurs_n_ans_, 3));
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}