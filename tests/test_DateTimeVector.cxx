#include "gtest/gtest.h"
#include "DateTimeVector.h"
#include <fstream>

class DateTimeVectorTest: public ::testing::Test{
    protected:

        DateTimeVector *dates;

        virtual void SetUp(){
            this->dates = new DateTimeVector("../tests/test_data/dateVector", 16);
        }

        virtual void TearDown(){
            delete this->dates;
        }
};

TEST_F(DateTimeVectorTest, constructorSize){
    EXPECT_EQ(16, this->dates->nbDates_);
}

TEST_F(DateTimeVectorTest, constructorArrayElement0){
    EXPECT_EQ(14, this->dates->dates_[0]->dd_);
    EXPECT_EQ(11, this->dates->dates_[0]->mm_);
    EXPECT_EQ(2008, this->dates->dates_[0]->yyyy_);
}

TEST_F(DateTimeVectorTest, constructorArrayElement15){
    EXPECT_EQ(28, this->dates->dates_[15]->dd_);
    EXPECT_EQ(4, this->dates->dates_[15]->mm_);
    EXPECT_EQ(2016, this->dates->dates_[15]->yyyy_);
}

TEST_F(DateTimeVectorTest, operatorCrochetElement0){
    EXPECT_EQ(14, (*this->dates)[0]->dd_);
    EXPECT_EQ(11, (*this->dates)[0]->mm_);
    EXPECT_EQ(2008, (*this->dates)[0]->yyyy_);
}

TEST_F(DateTimeVectorTest, operatorCrochetElement15){
    EXPECT_EQ(28, (*this->dates)[15]->dd_);
    EXPECT_EQ(4, (*this->dates)[15]->mm_);
    EXPECT_EQ(2016, (*this->dates)[15]->yyyy_);
}

TEST_F(DateTimeVectorTest, calculIndicesDates){
    DateTimeVector *subset = new DateTimeVector("../tests/test_data/dateVectorSubset", 5);
    PnlVectInt *indices = pnl_vect_int_create(5);
    calcul_indices_dates(this->dates, subset, indices);
    EXPECT_EQ(0, GET_INT(indices, 0));
    EXPECT_EQ(7, GET_INT(indices, 1));
    EXPECT_EQ(10, GET_INT(indices, 2));
    EXPECT_EQ(14, GET_INT(indices, 3));
    EXPECT_EQ(15, GET_INT(indices, 4));

    delete subset;
    pnl_vect_int_free(&indices);
}


int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
