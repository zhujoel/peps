#include "gtest/gtest.h"
#include "market_data/HistoricalMarketData.h"
#include "pnl/pnl_matrix.h"

class HistoricalMarketDataTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(HistoricalMarketDataTest, getData){
    HistoricalMarketData *historical = new HistoricalMarketData("Ocelia", new DateTime(1, 1, 2007), new DateTime(10, 1, 2007));
    historical->set_data();

    EXPECT_EQ(4, historical->dates_.size());

    delete historical;
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
