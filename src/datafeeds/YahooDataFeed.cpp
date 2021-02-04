#include "datafeeds/YahooDataFeed.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "libs/Utilities.h"

YahooDataFeed::YahooDataFeed(char* filepath){
    this->filepath_ = filepath;
}
YahooDataFeed::~YahooDataFeed(){}

void YahooDataFeed::getData(DateTimeVector *dates, PnlVect *path){
    std::ifstream dataFile(this->filepath_);
    std::string line;
    std::string parsedLine[7];
    char delimiter = ',';
    int nbDates = 0;

    // on fait 2 parcours: un pour avoir le nombre de dates valides, et un pour remplir les vecteurs
    // TODO: a voir si on peut faire qu'un seul parcours
    while (dataFile >> line) 
    {
        split(line, delimiter, parsedLine);
        if(parsedLine[4] != "null") nbDates++;
        std::cout << parsedLine[0] << std::endl;

    }

    dates = new DateTimeVector(nbDates);
    path = pnl_vect_create(nbDates);
    int idx = 0;


}