#include "gtest/gtest.h"
#include "libs/DateTimeVector.h"
#include <fstream>

class DateTimeVectorTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }
        virtual void TearDown(){
        }
};

TEST_F(DateTimeVectorTest, parseDatesFile){
    std::vector<DateTime*> dates = parseDatesFile("../tests/test_data/dateTimeVector/dateVectorSubset.csv", 5, '-');

    EXPECT_EQ(5, dates.size());
    EXPECT_EQ("14-11-2008", dates[0]->str());
    EXPECT_EQ("15-5-2012", dates[1]->str());
    EXPECT_EQ("15-11-2013", dates[2]->str());
    EXPECT_EQ("13-11-2015", dates[3]->str());
    EXPECT_EQ("28-4-2016", dates[4]->str());

    delete_date_vector(dates);
}

TEST_F(DateTimeVectorTest, calculIndicesDates){
    std::vector<DateTime*> dates = parseDatesFile("../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    std::vector<DateTime*> subset = parseDatesFile("../tests/test_data/dateTimeVector/dateVectorSubset.csv", 5, '-');
    PnlVectInt *indices = calcul_indices_dates(dates, subset);
    
    EXPECT_EQ(5, indices->size);
    EXPECT_EQ(0, GET_INT(indices, 0));
    EXPECT_EQ(7, GET_INT(indices, 1));
    EXPECT_EQ(10, GET_INT(indices, 2));
    EXPECT_EQ(14, GET_INT(indices, 3));
    EXPECT_EQ(15, GET_INT(indices, 4));
    
    pnl_vect_int_free(&indices);
    delete_date_vector(dates);
    delete_date_vector(subset);
}

TEST_F(DateTimeVectorTest, sameDates){
    std::vector<DateTime*> dates1 = parseDatesFile("../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    std::vector<DateTime*> dates2 = parseDatesFile("../tests/test_data/dateTimeVector/sameDates.csv", 6, '-');
    std::vector<DateTime*> same = sameDates(dates1, dates2);

    EXPECT_EQ(3, same.size());
    EXPECT_EQ("14-11-2008", same[0]->str());
    EXPECT_EQ("15-5-2015", same[1]->str());
    EXPECT_EQ("28-4-2016", same[2]->str());
    
    delete_date_vector(dates1);
    delete_date_vector(dates2);
    delete_date_vector(same);
}

TEST_F(DateTimeVectorTest, getPricesFromDate){
    std::vector<DateTime*> dates = parseDatesFile("../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    std::vector<DateTime*> subset = parseDatesFile("../tests/test_data/dateTimeVector/dateVectorSubset.csv", 5, '-');
    int size = dates.size();
    PnlVect *allPrices = pnl_vect_create(size);
    for(int i = 0; i < size; ++i) LET(allPrices, i) = i;

    PnlVect *result = getPricesFromDate(dates, subset, allPrices);

    EXPECT_EQ(5, result->size);
    EXPECT_EQ(0, GET(result, 0));
    EXPECT_EQ(7, GET(result, 1));
    EXPECT_EQ(10, GET(result, 2));
    EXPECT_EQ(14, GET(result, 3));
    EXPECT_EQ(15, GET(result, 4));

    pnl_vect_free(&result);
    pnl_vect_free(&allPrices);
    delete_date_vector(dates);
    delete_date_vector(subset);
}

TEST_F(DateTimeVectorTest, fromDateToDate){
    std::vector<DateTime*> dates = parseDatesFile("../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    DateTime *from = new DateTime(1, 1, 2013);
    DateTime *to = new DateTime(1, 1, 2014);
    std::vector<DateTime*> result = fromDateToDate(dates, from, to);

    EXPECT_EQ(2, result.size());
    EXPECT_EQ("15-5-2013", result[0]->str());
    EXPECT_EQ("15-11-2013", result[1]->str());

    delete_date_vector(dates);
    delete from;
    delete to;
}


int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
