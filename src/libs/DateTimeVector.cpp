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

std::ostream &operator<<(std::ostream &output, const DateTimeVector *vector){
    for(int i = 0; i < vector->nbDates_; ++i){
        output << vector->dates_[i] << std::endl;
    }
    return output;
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

// TODO: facto and opti
void sameDates(DateTimeVector *v1, DateTimeVector *v2, DateTimeVector *result)
{
    int len1 = v1->nbDates_;
    int len2 = v2->nbDates_;
    int idx = 0;
    int idx1 = 0;
    int idx2 = 0;
    DateTimeVector *tmp_vect = new DateTimeVector(std::min(len1, len2));
    while(idx < std::max(len1, len2) && idx1 < len1 && idx2 < len2){
        int cmp = v1->dates_[idx1]->compare(v2->dates_[idx2]);
        if(cmp == 0){
            tmp_vect->dates_[idx++] = v1->dates_[idx1]->copy();
            idx1++;
            idx2++;
        }
        else if(cmp > 0){
            idx2++;
        }
        else{
            idx1++;
        }
    }

    result->resize(idx);

    for(int i = 0; i < idx; ++i){
        result->dates_[i] = tmp_vect->dates_[i]->copy();
    }
}

// TODO: we could also use getIndicesFromDates?
void getPricesFromDate(DateTimeVector *allDates, DateTimeVector *relevantDates, PnlVect *allPrices, PnlVect *result)
{
    pnl_vect_resize(result, relevantDates->nbDates_);
    int idx = 0;
    int idx2 = 0;
    while(idx < relevantDates->nbDates_){
        if((allDates->dates_[idx2]->compare(relevantDates->dates_[idx]) == 0)){
            LET(result, idx++) = GET(allPrices, idx2++);
        }
        else{
            idx2++;
        }
    }
}

void fromDateToDate(DateTimeVector *allDates, DateTime *from, DateTime *to, DateTimeVector *result){
    int idx = 0;

    int startIdx = 0;
    int endIdx = 0;

    bool flag = false;
    while(idx < allDates->nbDates_){
        int cmpFrom = allDates->dates_[idx]->compare(from);
        if(!flag && cmpFrom >= 0){
            flag = true;
            startIdx = idx;
        }
        if(flag){
            int cmpTo = allDates->dates_[idx]->compare(to);
            if(cmpTo == 1){
                endIdx = idx;
                break;
            }
        }
        idx++;
    }

    result->resize(endIdx - startIdx);
    int i = 0;
    while(startIdx < endIdx){
        result->dates_[i++] = allDates->dates_[startIdx++];
    }
}