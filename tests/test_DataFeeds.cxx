#include "gtest/gtest.h"
#include "datafeeds/YahooDataFeed.h"
#include "datafeeds/FTSEDataFeed.h"
#include <string>
#include "libs/Utilities.h"

class DataFeedsTest: public ::testing::Test{
    protected:
        virtual void SetUp(){
        }

        virtual void TearDown(){
        }
};

TEST_F(DataFeedsTest, YahooGetData)
{
    std::string ssmi_filepath = "../data/market_data/SSMI.csv";
    YahooDataFeed *yahoo = new YahooDataFeed(ssmi_filepath);
    PnlVect *pricesSSMI = pnl_vect_new();
    DateTimeVector *datesSSMI = new DateTimeVector(0);
    yahoo->getData(datesSSMI, pricesSSMI);
    EXPECT_EQ(1, 1);
}

TEST_F(DataFeedsTest, YahooParseAndOutput)
{
    char str[] = "../data/market_data/SSMI.csv";
    YahooDataFeed *yahoo = new YahooDataFeed(str);
    yahoo->parseAndOutput();
    EXPECT_EQ(1, 1);
}

TEST_F(DataFeedsTest, FTSEGetData)
{
    std::string filepath = "../data/market_data/FTSE80R.csv";
    FTSEDataFeed *ftse = new FTSEDataFeed(filepath);
    PnlVect *vect = pnl_vect_new();
    DateTimeVector *dates = new DateTimeVector(0);
    ftse->getData(dates, vect);
    // pnl_vect_print(vect);
    EXPECT_EQ(1, 1);
}

TEST_F(DataFeedsTest, sameDates){
    std::string ftse_filepath = "../data/market_data/FTSE80R.csv";
    FTSEDataFeed *ftse = new FTSEDataFeed(ftse_filepath);
    PnlVect *pricesFTSE = pnl_vect_new();
    DateTimeVector *datesFTSE = new DateTimeVector(0);
    ftse->getData(datesFTSE, pricesFTSE);

    std::string ssmi_filepath = "../data/market_data/SSMI.csv";
    YahooDataFeed *yahoo = new YahooDataFeed(ssmi_filepath);
    PnlVect *pricesSSMI = pnl_vect_new();
    DateTimeVector *datesSSMI = new DateTimeVector(0);
    yahoo->getData(datesSSMI, pricesSSMI);

    std::string n225_filepath = "../data/market_data/N225.csv";
    YahooDataFeed *n225 = new YahooDataFeed(n225_filepath);
    PnlVect *pricesN225 = pnl_vect_new();
    DateTimeVector *datesN225 = new DateTimeVector(0);
    n225->getData(datesN225, pricesN225);

    DateTimeVector *result = new DateTimeVector(0);
    sameDates(datesFTSE, datesSSMI, result);
    sameDates(result, datesN225, result);

    std::cout << "ftse size: " << datesFTSE->nbDates_ << std::endl;
    std::cout << "ssmi size: " << datesSSMI->nbDates_ << std::endl;
    std::cout << "n225 size: " << datesSSMI->nbDates_ << std::endl;
    std::cout << "result size: " << result->nbDates_ << std::endl;

    EXPECT_EQ(1, 1);
}


TEST_F(DataFeedsTest, getPricesFromDate){
    std::string ftse_filepath = "../data/market_data/FTSE80R.csv";
    FTSEDataFeed *ftse = new FTSEDataFeed(ftse_filepath);
    PnlVect *pricesFTSE = pnl_vect_new();
    DateTimeVector *datesFTSE = new DateTimeVector(0);
    ftse->getData(datesFTSE, pricesFTSE);

    std::string ssmi_filepath = "../data/market_data/SSMI.csv";
    YahooDataFeed *yahoo = new YahooDataFeed(ssmi_filepath);
    PnlVect *pricesSSMI = pnl_vect_new();
    DateTimeVector *datesSSMI = new DateTimeVector(0);
    yahoo->getData(datesSSMI, pricesSSMI);

    DateTimeVector *dateResult = new DateTimeVector(0);
    sameDates(datesFTSE, datesSSMI, dateResult);

    std::cout << "ftse: " << datesFTSE->nbDates_ << std::endl;
    std::cout << "ssmi: " << datesSSMI->nbDates_ << std::endl;
    std::cout << "result: " << dateResult->nbDates_ << std::endl;

    PnlVect *priceResult = pnl_vect_new();
    getPricesFromDate(datesSSMI, dateResult, pricesSSMI, priceResult);

    pnl_vect_print(priceResult);
    std::cout << "price size: " << priceResult->size << std::endl;

    EXPECT_EQ(1, 1);
}


int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
