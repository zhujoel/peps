#include "DateTime.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

DateTime::DateTime(int dd, int mm, int yyyy)
{
    // TODO: faire les vérifs
    this->dd_ = dd;
    this->mm_ = mm;
    this->yyyy_ = yyyy;
}

DateTime::~DateTime(){

}

int DateTime::compare(const DateTime *dt) const{
    if(this->yyyy_ < dt->yyyy_){
        return -1;
    }
    else if(this->yyyy_ > dt->yyyy_){
        return 1;
    }
    else{
        if(this->mm_ < dt->mm_){
            return -1;
        }
        else if(this->mm_ > dt->mm_){
            return 1;
        }
        else{
            if(this->dd_ < dt->dd_){
                return -1;
            }
            else if(this->dd_ > dt->dd_){
                return 1;
            }
        }
    }
    return 0;
}


std::ostream &operator<<(std::ostream &output, const DateTime *dt){
    output << dt->dd_ << "/" << dt->mm_ << "/" << dt->yyyy_ << std::endl;
    return output;
}

void fill_dates_from_file(DateTime **dates, std::string fileName, int nbDates) {
    std::ifstream inputFileStream(fileName);
    std::string jour, mois, annee;
    std::string line;
    for (int i = 0; i < nbDates; i++)
    {
        std::getline(inputFileStream, line);
        std::istringstream lineStream(line);
        std::getline(lineStream, jour, ',');
        std::getline(lineStream, mois, ',');
        std::getline(lineStream, annee, ',');
        dates[i] = new DateTime(std::stoi(jour), std::stoi("1"), std::stoi("199"));
    }
}
