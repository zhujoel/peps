#include "gtest/gtest.h"
#include "DateTime.h"
#include "pnl/pnl_vector.h"
#include <fstream>

class DateTimeTest: public ::testing::Test{
    protected:
        DateTime *dt;

        virtual void SetUp(){
            this->dt = new DateTime(15, 7, 1992);
        }

        virtual void TearDown(){
            delete this->dt;
        }
};

TEST_F(DateTimeTest, constructorJour){
    EXPECT_EQ(15, this->dt->dd_);
}

TEST_F(DateTimeTest, constructorMonth){
    EXPECT_EQ(7, this->dt->mm_);
}

TEST_F(DateTimeTest, constructorYear){
    EXPECT_EQ(1992, this->dt->yyyy_);
}

TEST_F(DateTimeTest, equalsDayBefore){
    DateTime *dt2 = new DateTime(25, 7, 1992);
    EXPECT_EQ(-1, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, equalsDaySame){
    DateTime *dt2 = new DateTime(15, 7, 1992);
    EXPECT_EQ(0, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, equalsDayAfter){
    DateTime *dt2 = new DateTime(10, 7, 1992);
    EXPECT_EQ(1, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, equalsMonthBefore){
    DateTime *dt2 = new DateTime(15, 10, 1992);
    EXPECT_EQ(-1, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, equalsMonthSame){
    DateTime *dt2 = new DateTime(15, 7, 1992);
    EXPECT_EQ(0, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, equalsMonthAfter){
    DateTime *dt2 = new DateTime(15, 3, 1992);
    EXPECT_EQ(1, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, equalsYearBefore){
    DateTime *dt2 = new DateTime(15, 7, 1993);
    EXPECT_EQ(-1, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, equalsYearSame){
    DateTime *dt2 = new DateTime(15, 7, 1992);
    EXPECT_EQ(0, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, equalsYearAfter){
    DateTime *dt2 = new DateTime(15, 7, 1990);
    EXPECT_EQ(1, this->dt->compare(dt2));
    delete dt2;
}

TEST_F(DateTimeTest, operatorPrint){
    std::stringstream str;
    str << this->dt;
    EXPECT_EQ("15/7/1992", str.str());
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
