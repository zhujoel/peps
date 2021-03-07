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
    DateTime *start = new DateTime(15, 5, 2008);
    DateTime *end = new DateTime(20, 5, 2008);
    HistoricalMarketData *historical = new HistoricalMarketData("Ocelia", start, end);
    historical->set_data(); // il y a 4 leaks

    EXPECT_EQ(4, historical->dates_.size());

    delete start;
    delete end;
    delete historical;
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
