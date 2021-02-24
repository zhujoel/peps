#include "gtest/gtest.h"
#include "datafeeds/YahooDataFeed.h"
#include <string>
#include "libs/Utilities.h"

class DataFeedsTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }
        virtual void TearDown(){
        }
};

TEST_F(DataFeedsTest, getNumberValidData1)
{
    IDataFeed *dataFeed = new YahooDataFeed("../tests/test_data/market_data/yahoo1.csv");
    int number = dataFeed->getNumberValidData();

    EXPECT_EQ(7, number);

    delete dataFeed;
}

TEST_F(DataFeedsTest, getNumberValidData2)
{
    IDataFeed *dataFeed = new YahooDataFeed("../tests/test_data/market_data/yahoo2.csv");
    int number = dataFeed->getNumberValidData();
    
    EXPECT_EQ(4, number);

    delete dataFeed;
}

TEST_F(DataFeedsTest, getNumberValidData3)
{
    IDataFeed *dataFeed = new YahooDataFeed("../tests/test_data/market_data/yahoo3.csv");
    int number = dataFeed->getNumberValidData();
    
    EXPECT_EQ(0, number);

    delete dataFeed;
}

TEST_F(DataFeedsTest, getData1)
{
    IDataFeed *dataFeed = new YahooDataFeed("../tests/test_data/market_data/yahoo1.csv");
    dataFeed->getData();

    EXPECT_EQ("3-1-2003", dataFeed->dates_[0]->str());
    EXPECT_EQ(4899.5, GET(dataFeed->prices_, 0));
    EXPECT_EQ("6-1-2003", dataFeed->dates_[1]->str());
    EXPECT_EQ(4916.200195, GET(dataFeed->prices_, 1));
    EXPECT_EQ("7-1-2003", dataFeed->dates_[2]->str());
    EXPECT_EQ(4887, GET(dataFeed->prices_, 2));
    EXPECT_EQ("8-1-2003", dataFeed->dates_[3]->str());
    EXPECT_EQ(4882.299805, GET(dataFeed->prices_, 3));
    EXPECT_EQ("9-1-2003", dataFeed->dates_[4]->str());
    EXPECT_EQ(4857.899902, GET(dataFeed->prices_, 4));
    EXPECT_EQ("10-1-2003", dataFeed->dates_[5]->str());
    EXPECT_EQ(4854.200195, GET(dataFeed->prices_, 5));
    EXPECT_EQ("13-1-2003", dataFeed->dates_[6]->str());
    EXPECT_EQ(4887.799805, GET(dataFeed->prices_, 6));

    delete dataFeed;
}

int main(int argc, char** argv){    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
