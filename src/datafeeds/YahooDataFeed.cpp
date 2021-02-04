#include "datafeeds/YahooDataFeed.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "libs/Utilities.h"
#include <stdlib.h>

YahooDataFeed::YahooDataFeed(std::string filepath) : IDataFeed(filepath)
{
    this->filepath_ = filepath;
}

YahooDataFeed::~YahooDataFeed(){}

int YahooDataFeed::getNumberValidData()
{
    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[7];
    int count = 0;

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(line, ',', parsedLine);
        if(parsedLine[4] != "null") count++;
    }

    dataFile.close();
    return count;
}

void YahooDataFeed::getData(DateTimeVector *dates, PnlVect *path)
{
    int nbDates = this->getNumberValidData();
    dates->resize(nbDates);
    pnl_vect_resize(path, nbDates);

    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[7];
    int i = 0;

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(line, ',', parsedLine);
        if(parsedLine[4] == "null") continue;
        dates->dates_[i] = parseDateString(parsedLine[0], '-');
        LET(path, i) = std::atof(parsedLine[4].c_str()); // c_str(): needs a char* to use atof
        i++;
    }

    dataFile.close();
}


void YahooDataFeed::parseAndOutput(){
    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[7];


    std::ofstream datesOut(this->filepath_ + "-dates.csv");
    std::ofstream pricesOut(this->filepath_ + "-prices.csv");

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(line, ',', parsedLine);
        if(parsedLine[4] == "null") continue;
        datesOut << parsedLine[0] << std::endl;
        pricesOut << parsedLine[4] << std::endl;
    }

    datesOut.close();
    pricesOut.close();
    dataFile.close();
}