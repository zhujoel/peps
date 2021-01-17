#include "gtest/gtest.h"
#include "DateTime.h"
#include <fstream>
#include <chrono>

class DateTimeTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(DateTimeTest, constructorDefault){
    DateTime *dt = new DateTime(1, 1, 1);
    EXPECT_EQ(1, dt->dd_);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
