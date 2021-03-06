#include "gtest/gtest.h"
#include "libs/InterestRate.h"

class InterestRateTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(InterestRateTest, test1){
    DateTime *current_date = new DateTime(1, 1, 2005);
    std::vector<DateTime*> all_dates;
    PnlMat *path = pnl_mat_new();
    InterestRate *ir = new InterestRate(1.254, current_date, all_dates, path);


}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
