#include "gtest/gtest.h"
#include "datafeeds/YahooDataFeed.h"
#include "datafeeds/FTSEDataFeed.h"
#include <string>

class DataFeedsTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

// TEST_F(DataFeedsTest, YahooGetData)
// {
//     char str[] = "../data/market_data/^SSMI.csv";
//     YahooDataFeed *yahoo = new YahooDataFeed(str);
//     PnlVect *vect = pnl_vect_new();
//     DateTimeVector *dates = new DateTimeVector(0);
//     yahoo->getData(dates, vect);
//     EXPECT_EQ(1, 1);
// }

TEST_F(DataFeedsTest, YahooParseAndOutput)
{
    char str[] = "../data/market_data/SSMI.csv";
    YahooDataFeed *yahoo = new YahooDataFeed(str);
    yahoo->parseAndOutput();
    EXPECT_EQ(1, 2);
}

TEST_F(DataFeedsTest, FTSEGetData)
{
    std::string filepath = "../data/market_data/FTSE80R.csv";
    FTSEDataFeed *ftse = new FTSEDataFeed(filepath);
    PnlVect *vect = pnl_vect_new();
    DateTimeVector *dates = new DateTimeVector(0);
    ftse->getData(dates, vect);
    pnl_vect_print(vect);
    EXPECT_EQ(1, 1);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
