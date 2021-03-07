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
    std::vector<DateTime*> dates;
    parse_dates_file(dates, "../tests/test_data/dateTimeVector/dateVectorSubset.csv", 5, '-');

    EXPECT_EQ(5, dates.size());
    EXPECT_EQ("14-11-2008", dates[0]->str());
    EXPECT_EQ("15-5-2012", dates[1]->str());
    EXPECT_EQ("15-11-2013", dates[2]->str());
    EXPECT_EQ("13-11-2015", dates[3]->str());
    EXPECT_EQ("28-4-2016", dates[4]->str());

    delete_date_vector(dates);
}

TEST_F(DateTimeVectorTest, calculIndicesDates){
    std::vector<DateTime*> dates;
    parse_dates_file(dates, "../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    std::vector<DateTime*> subset;
    parse_dates_file(subset, "../tests/test_data/dateTimeVector/dateVectorSubset.csv", 5, '-');
    PnlVectInt *indices = pnl_vect_int_new();
    calcul_indices_dates(indices, dates, subset);
    
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
    std::vector<DateTime*> dates1;
    parse_dates_file(dates1, "../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    std::vector<DateTime*> dates2;
    parse_dates_file(dates2, "../tests/test_data/dateTimeVector/sameDates.csv", 6, '-');
    std::vector<DateTime*> same = same_dates(dates1, dates2);

    EXPECT_EQ(3, same.size());
    EXPECT_EQ("14-11-2008", same[0]->str());
    EXPECT_EQ("15-5-2015", same[1]->str());
    EXPECT_EQ("28-4-2016", same[2]->str());
    
    delete_date_vector(dates1);
    delete_date_vector(dates2);
    delete_date_vector(same);
}

TEST_F(DateTimeVectorTest, getPricesFromDate){
    std::vector<DateTime*> dates;
    parse_dates_file(dates, "../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    std::vector<DateTime*> subset;
    parse_dates_file(subset, "../tests/test_data/dateTimeVector/dateVectorSubset.csv", 5, '-');
    int size = dates.size();
    PnlVect *allPrices = pnl_vect_create(size);
    for(int i = 0; i < size; ++i) LET(allPrices, i) = i;

    PnlVect *result = pnl_vect_new();
    get_prices_from_date(result, dates, subset, allPrices);

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
    std::vector<DateTime*> dates;
    parse_dates_file(dates, "../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    DateTime *from = new DateTime(1, 1, 2013);
    DateTime *to = new DateTime(1, 1, 2014);
    std::vector<DateTime*> result;
    from_date_to_date(result, dates, from, to);

    EXPECT_EQ(2, result.size());
    EXPECT_EQ("15-5-2013", result[0]->str());
    EXPECT_EQ("15-11-2013", result[1]->str());

    delete_date_vector(dates);
    delete from;
    delete to;
}

TEST_F(DateTimeVectorTest, get_indice_from_date){
    std::vector<DateTime*> dates;
    parse_dates_file(dates, "../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    DateTime *date_indice = new DateTime(15, 5, 2013);
    int indice = get_indice_from_date(dates, date_indice);

    EXPECT_EQ(9, indice);

    delete_date_vector(dates);
    delete date_indice;
}

TEST_F(DateTimeVectorTest, get_subset_path_from_dates){
    std::vector<DateTime*> dates;
    parse_dates_file(dates, "../tests/test_data/dateTimeVector/dateVector.csv", 16, '-');
    std::vector<DateTime*> dates_subset;
    parse_dates_file(dates_subset, "../tests/test_data/dateTimeVector/dateVectorSubset.csv", 5, '-');
    PnlMat *path = pnl_mat_create(16, 2);
    for(int i = 0; i < 16; ++i){
        for(int j = 0; j < 2; ++j){
            MLET(path, i, j) = i+j;
        }
    }

    PnlMat *subset_path = pnl_mat_new();
    get_subset_path_from_dates(subset_path, dates, dates_subset, path);

    EXPECT_EQ(0, MGET(subset_path, 0, 0));
    EXPECT_EQ(1, MGET(subset_path, 0, 1));
    EXPECT_EQ(7, MGET(subset_path, 1, 0));
    EXPECT_EQ(8, MGET(subset_path, 1, 1));
    EXPECT_EQ(10, MGET(subset_path, 2, 0));
    EXPECT_EQ(11, MGET(subset_path, 2, 1));
    EXPECT_EQ(14, MGET(subset_path, 3, 0));
    EXPECT_EQ(15, MGET(subset_path, 3, 1));
    EXPECT_EQ(15, MGET(subset_path, 4, 0));
    EXPECT_EQ(16, MGET(subset_path, 4, 1));

    delete_date_vector(dates);
    delete_date_vector(dates_subset);
    pnl_mat_free(&path);
    pnl_mat_free(&subset_path);
}


int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
