#include "gtest/gtest.h"
#include "datafeeds/MarketDataFeed.h"
#include <string>
#include "libs/Utilities.h"
#include "pnl/pnl_vector.h"

class DataFeedsTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }
        virtual void TearDown(){
        }
};

TEST_F(DataFeedsTest, getNumberValidData1)
{
    PnlVectInt *columns = pnl_vect_int_create(1);
    LET_INT(columns, 0) = 4;
    unsigned int nb_columns = 7;
    unsigned int date_column = 0;
    IDataFeed *dataFeed = new MarketDataFeed(columns, date_column, nb_columns, "../tests/test_data/market_data/yahoo1.csv");
    int number = dataFeed->get_number_valid_data();

    EXPECT_EQ(7, number);

    delete dataFeed;
    pnl_vect_int_free(&columns);
}

TEST_F(DataFeedsTest, getNumberValidData2)
{
    PnlVectInt *columns = pnl_vect_int_create(1);
    LET_INT(columns, 0) = 4;
    unsigned int nb_columns = 7;
    unsigned int date_column = 0;
    IDataFeed *dataFeed = new MarketDataFeed(columns, date_column, nb_columns, "../tests/test_data/market_data/yahoo2.csv");
    int number = dataFeed->get_number_valid_data();
    
    EXPECT_EQ(4, number);

    delete dataFeed;
    pnl_vect_int_free(&columns);
}

TEST_F(DataFeedsTest, getNumberValidData3)
{
    PnlVectInt *columns = pnl_vect_int_create(1);
    LET_INT(columns, 0) = 4;
    unsigned int nb_columns = 7;
    unsigned int date_column = 0;
    IDataFeed *dataFeed = new MarketDataFeed(columns, date_column, nb_columns, "../tests/test_data/market_data/yahoo3.csv");
    int number = dataFeed->get_number_valid_data();
    
    EXPECT_EQ(0, number);

    delete dataFeed;
    pnl_vect_int_free(&columns);
}

TEST_F(DataFeedsTest, getData1)
{
    PnlVectInt *columns = pnl_vect_int_create(1);
    LET_INT(columns, 0) = 4;
    unsigned int nb_columns = 7;
    unsigned int date_column = 0;
    IDataFeed *dataFeed = new MarketDataFeed(columns, date_column, nb_columns, "../tests/test_data/market_data/yahoo1.csv");
    dataFeed->set_data();

    EXPECT_EQ("3-1-2003", dataFeed->dates_[0]->str());
    EXPECT_EQ(4899.5, MGET(dataFeed->prices_, 0, 0));
    EXPECT_EQ("6-1-2003", dataFeed->dates_[1]->str());
    EXPECT_EQ(4916.200195, MGET(dataFeed->prices_, 1, 0));
    EXPECT_EQ("7-1-2003", dataFeed->dates_[2]->str());
    EXPECT_EQ(4887, MGET(dataFeed->prices_, 2, 0));
    EXPECT_EQ("8-1-2003", dataFeed->dates_[3]->str());
    EXPECT_EQ(4882.299805, MGET(dataFeed->prices_, 3, 0));
    EXPECT_EQ("9-1-2003", dataFeed->dates_[4]->str());
    EXPECT_EQ(4857.899902, MGET(dataFeed->prices_, 4, 0));
    EXPECT_EQ("10-1-2003", dataFeed->dates_[5]->str());
    EXPECT_EQ(4854.200195, MGET(dataFeed->prices_, 5, 0));
    EXPECT_EQ("13-1-2003", dataFeed->dates_[6]->str());
    EXPECT_EQ(4887.799805, MGET(dataFeed->prices_, 6, 0));

    delete dataFeed;
    pnl_vect_int_free(&columns);
}


TEST_F(DataFeedsTest, tauxChangeGetData1)
{
    PnlVectInt *columns = pnl_vect_int_create(3);
    LET_INT(columns, 0) = 1;
    LET_INT(columns, 1) = 2;
    LET_INT(columns, 2) = 3;
    unsigned int nb_columns = 4;
    unsigned int date_column = 0;
    IDataFeed *dataFeed = new MarketDataFeed(columns, date_column, nb_columns, "../tests/test_data/market_data/taux_de_change1.csv");
    dataFeed->set_data();

    EXPECT_EQ("2-3-1990", dataFeed->dates_[0]->str());
    EXPECT_EQ(1.39386421, MGET(dataFeed->prices_, 0, 0));
    EXPECT_EQ(0.005593467, MGET(dataFeed->prices_, 0, 1));
    EXPECT_EQ(0.556544969, MGET(dataFeed->prices_, 0, 2));

    delete dataFeed;
    pnl_vect_int_free(&columns);
}

TEST_F(DataFeedsTest, tauxChangeGetData2)
{
    PnlVectInt *columns = pnl_vect_int_create(2);
    LET_INT(columns, 0) = 1;
    LET_INT(columns, 1) = 3;
    unsigned int nb_columns = 4;
    unsigned int date_column = 0;
    IDataFeed *dataFeed = new MarketDataFeed(columns, date_column, nb_columns, "../tests/test_data/market_data/taux_de_change1.csv");
    dataFeed->set_data();

    EXPECT_EQ("2-3-1990", dataFeed->dates_[0]->str());
    EXPECT_EQ(1.39386421, MGET(dataFeed->prices_, 0, 0));
    EXPECT_EQ(0.556544969, MGET(dataFeed->prices_, 0, 1));

    delete dataFeed;
    pnl_vect_int_free(&columns);
}

int main(int argc, char** argv){    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
