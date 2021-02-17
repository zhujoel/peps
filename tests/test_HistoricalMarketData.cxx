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
    HistoricalMarketData *historical = new HistoricalMarketData("name of the thing", new DateTime(1, 1, 2003), new DateTime(1, 1, 2008));
    DateTimeVector *dates = new DateTimeVector(0);
    historical->getData(dates);
    EXPECT_EQ(1, 1);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
