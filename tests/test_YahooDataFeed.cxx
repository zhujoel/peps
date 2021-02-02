#include "gtest/gtest.h"
#include "datafeeds/YahooDataFeed.h"

class YahooDataFeedTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(YahooDataFeedTest, frgfzorgzk){
    EXPECT_EQ(1, 2);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
