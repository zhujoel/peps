#include "gtest/gtest.h"
#include "libs/Utilities.h"
#include "pnl/pnl_vector.h"
#include <string>

class UtilitiesTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(UtilitiesTest, split){
    std::string str = "hello,world,?";
    std::string output[3];
    split(output, str, ',');

    EXPECT_EQ("hello", output[0]);
    EXPECT_EQ("world", output[1]);
    EXPECT_EQ("?", output[2]);
}

TEST_F(UtilitiesTest, trunc){
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

TEST_F(UtilitiesTest, trunc_zero){
    PnlVect *vect = pnl_vect_create(1);
    LET(vect, 0) = 0.0000000;
    trunc(vect, 4);
    EXPECT_DOUBLE_EQ(0.0000, GET(vect, 0));

    pnl_vect_free(&vect);
}

TEST_F(UtilitiesTest, are_all_positive_false){
    PnlVect *vect = pnl_vect_create(3);
    LET(vect, 0) = -1.123456;
    LET(vect, 1) = 0.547;
    LET(vect, 2) = 3.14153;
    EXPECT_FALSE(are_all_positive(vect));

    pnl_vect_free(&vect);
}

TEST_F(UtilitiesTest, are_all_positive_true){
    PnlVect *vect = pnl_vect_create(3);
    LET(vect, 0) = 1.123456;
    LET(vect, 1) = 0.547;
    LET(vect, 2) = 3.14153;
    EXPECT_TRUE(are_all_positive(vect));

    pnl_vect_free(&vect);
}

TEST_F(UtilitiesTest, has_no_index_value_null1){
    PnlVectInt *vect = pnl_vect_int_create(2);
    std::string arr[5] = {"null", "hello", "hello2", "hello3", "hello4"};
    LET_INT(vect, 0) = 2;
    LET_INT(vect, 1) = 0;
    EXPECT_FALSE(has_no_index_value_null(arr, vect));

    pnl_vect_int_free(&vect);
}

TEST_F(UtilitiesTest, has_no_index_value_null2){
    PnlVectInt *vect = pnl_vect_int_create(0);
    std::string arr[5] = {"null", "hello"};
    EXPECT_TRUE(has_no_index_value_null(arr, vect));

    pnl_vect_int_free(&vect);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
