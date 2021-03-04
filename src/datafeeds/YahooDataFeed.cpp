#include "datafeeds/YahooDataFeed.h"
#include <fstream>
#include <sstream>
#include "libs/Utilities.h"
#include <stdlib.h>

YahooDataFeed::YahooDataFeed(const std::string &filepath) : IDataFeed(filepath){}

int YahooDataFeed::get_number_valid_data() const
{
    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[7];
    int count = 0;

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(parsedLine, line, ',');
        if(parsedLine[4] != "null") count++;
    }

    dataFile.close();
    return count;
}

void YahooDataFeed::set_data()
{
    int nbDates = this->get_number_valid_data();
    pnl_vect_resize(this->prices_, nbDates);

    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[7];
    int i = 0;

    std::getline(dataFile, line); // skip header line
    while (std::getline(dataFile, line))
    {
        split(parsedLine, line, ',');
        if(parsedLine[4] == "null") continue;
        this->dates_.push_back(parse_date_string(parsedLine[0], '-'));
        LET(this->prices_, i++) = std::atof(parsedLine[4].c_str()); // c_str(): needs a char* to use atof
    }

    dataFile.close();
}