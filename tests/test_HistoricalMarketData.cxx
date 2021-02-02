#include "gtest/gtest.h"
#include "HistoricalMarketData.h"

class HistoricalMarketDataTest: public ::testing::Test{
    protected:

        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(HistoricalMarketDataTest, eezrgfrgz){
    EXPECT_EQ(1, 2);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
