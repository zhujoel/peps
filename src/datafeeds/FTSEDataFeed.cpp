#include "datafeeds/FTSEDataFeed.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "libs/Utilities.h"
#include <stdlib.h>

FTSEDataFeed::FTSEDataFeed(std::string filepath) : IDataFeed(filepath)
{
    this->filepath_ = filepath;
}

FTSEDataFeed::~FTSEDataFeed(){}

int FTSEDataFeed::getNumberValidData()
{
    std::ifstream dataFile(this->filepath_);
    std::string line;
    int count = 0;

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        count++;
    }

    dataFile.close();
    return count;
}

void FTSEDataFeed::getData(DateTimeVector *dates, PnlVect *path)
{
    int nbDates = this->getNumberValidData();
    dates->resize(nbDates);
    pnl_vect_resize(path, nbDates);

    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[3];
    int i = 0;

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(line, ',', parsedLine);
        dates->dates_[i] = parseDateString(parsedLine[0], '-');
        LET(path, i) = std::atof(parsedLine[1].c_str()); // c_str(): needs a char* to use atof
        i++;
    }

    dataFile.close();
}


void FTSEDataFeed::parseAndOutput(){
    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[3];


    std::ofstream datesOut(this->filepath_ + "-dates.csv");
    std::ofstream pricesOut(this->filepath_ + "-prices.csv");

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(line, ',', parsedLine);
        datesOut << parsedLine[0] << std::endl;
        pricesOut << parsedLine[1] << std::endl;
    }

    datesOut.close();
    pricesOut.close();
    dataFile.close();
}