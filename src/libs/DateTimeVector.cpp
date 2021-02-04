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

void DateTimeVector::resize(int nbDates){
    for(int i = 0; i < this->nbDates_; ++i){
        delete this->dates_[i];
    }
    delete[] this->dates_;
    this->nbDates_ = nbDates;
    this->dates_ = new DateTime*[this->nbDates_];
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

DateTimeVector* parseDatesFile(std::string fileName, int nbDates, char delimiter){
    DateTimeVector *dates = new DateTimeVector(nbDates);
    std::ifstream datesStream(fileName);
    std::string date;
    for (int i = 0; i < dates->nbDates_; i++)
    {
        getline(datesStream, date);
        dates->dates_[i] = parseDateString(date, delimiter);
    }
    return dates;
}