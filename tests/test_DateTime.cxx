#include "gtest/gtest.h"
#include "libs/DateTime.h"
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

TEST_F(DateTimeTest, parseDateString_dash){
    DateTime *date = parse_date_string("2012-08-30", '-');
    EXPECT_EQ(date->yyyy_, 2012);
    EXPECT_EQ(date->mm_, 8);
    EXPECT_EQ(date->dd_, 30);
    delete date;
}

TEST_F(DateTimeTest, parseDateString_slash){
    DateTime *date = parse_date_string("2012/08/30", '/');
    EXPECT_EQ(date->yyyy_, 2012);
    EXPECT_EQ(date->mm_, 8);
    EXPECT_EQ(date->dd_, 30);
    delete date;
}

TEST_F(DateTimeTest, str){
    DateTime *date = parse_date_string("2012/08/30", '/');
    EXPECT_EQ("30-8-2012", date->str());
    delete date;
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
