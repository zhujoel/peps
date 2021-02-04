#include "libs/DateTimeVector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

DateTimeVector::DateTimeVector(int nbDates)
{
    this->nbDates_ = nbDates;
    this->dates_ = new DateTime*[this->nbDates_];
}

DateTimeVector::~DateTimeVector(){
    for(int i = 0; i < this->nbDates_; ++i){
        delete this->dates_[i];
    }
    delete[] this->dates_;
}

void DateTimeVector::parseFile(std::string fileName){
    std::ifstream inputFileStream(fileName);
    std::string jour, mois, annee;
    std::string line;
    for (int i = 0; i < this->nbDates_; i++)
    {
        getline(inputFileStream, line);
        std::istringstream lineStream(line);
        getline(lineStream, jour, ';');
        getline(lineStream, mois, ';');
        getline(lineStream, annee, ';');
        this->dates_[i] = new DateTime(std::stoi(jour), std::stoi(mois), std::stoi(annee));
    }
}

DateTime* DateTimeVector::operator[](int index){
    if(index < 0 || index >= this->nbDates_){
        throw std::out_of_range("index out of bounds");
    }
    return this->dates_[index];
}

void calcul_indices_dates(DateTimeVector *all_dates, DateTimeVector *dates, PnlVectInt *indices)
{
    int cnt = 0;
    for(int i = 0; i < all_dates->nbDates_; ++i){
        if((*all_dates)[i]->compare((*dates)[cnt]) == 0){
            LET_INT(indices, cnt++) = i;
            if(cnt == indices->size){
                break;
            }
        }
    }
}