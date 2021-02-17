#include "gtest/gtest.h"
#include "market_data/HistoricalMarketData.h"

class HistoricalMarketDataTest: public ::testing::Test{
    protected:

        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(HistoricalMarketDataTest, getData){
    HistoricalMarketData *historical = new HistoricalMarketData("name of the thing", NULL, NULL);
    historical->getData();
    EXPECT_EQ(1, 1);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
