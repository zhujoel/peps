#include "gtest/gtest.h"
#include "datafeeds/YahooDataFeed.h"

class YahooDataFeedTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(YahooDataFeedTest, testaAAAA){

    char str[] = "../data/market_data/^SSMI.csv";
    YahooDataFeed *yahoo = new YahooDataFeed(str);
    PnlVect *vect = pnl_vect_new();
    DateTimeVector *dates = new DateTimeVector(0);
    yahoo->getData(dates, vect);
    EXPECT_EQ(1, 2);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
