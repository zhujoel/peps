#include "libs/DateTimeVector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::vector<DateTime*> parseDatesFile(std::string fileName, int nbDates, char delimiter)
{
    std::vector<DateTime*> dates;
    std::ifstream datesStream(fileName);
    std::string date;
    for (int i = 0; i < nbDates; i++)
    {
        getline(datesStream, date);
        dates.push_back(parseDateString(date, delimiter));
    }
    return dates;
}

void calcul_indices_dates(std::vector<DateTime*> all_dates, std::vector<DateTime*> *dates, PnlVectInt *indices)
{
    int cnt = 0;
    for(int i = 0; i < all_dates->size; ++i){
        if(all_dates[i]->compare(dates[cnt]) == 0){
            LET_INT(indices, cnt++) = i;
            if(cnt == indices->size){
                break;
            }
        }
    }
}

// TODO: facto and opti
std::vector<DateTime*> sameDates(std::vector<DateTime*> v1, std::vector<DateTime*> v2)
{
    std::vector<DateTime*> result;
    int len1 = v1->size;
    int len2 = v2->size;
    int idx = 0;
    int idx1 = 0;
    int idx2 = 0;
    std::vector<DateTime*> tmp_vect;
    while(idx < std::max(len1, len2) && idx1 < len1 && idx2 < len2){
        int cmp = v1[idx1]->compare(v2[idx2]);
        if(cmp == 0){
            tmp_vect[idx++] = v1[idx1]->copy();
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

    for(int i = 0; i < idx; ++i){
        result.push_back(tmp_vect[i]->copy());
    }

    return result;   
}

// TODO: we could also use getIndicesFromDates?
void getPricesFromDate(std::vector<DateTime*> allDates, std::vector<DateTime*> relevantDates, PnlVect *allPrices, PnlVect *result)
{
    pnl_vect_resize(result, relevantDates->size);
    int idx = 0;
    int idx2 = 0;
    while(idx < relevantDates->size){
        if((allDates[idx2]->compare(relevantDates[idx]) == 0)){
            LET(result, idx++) = GET(allPrices, idx2++);
        }
        else{
            idx2++;
        }
    }
}

std::vector<DateTime*> fromDateToDate(std::vector<DateTime*> allDates, DateTime *from, DateTime *to)
{
    std::vector<DateTime*> result;
    int idx = 0;
    int startIdx = 0;
    int endIdx = 0;

    bool flag = false;
    while(idx < allDates->size){
        int cmpFrom = allDates[idx]->compare(from);
        if(!flag && cmpFrom >= 0){
            flag = true;
            startIdx = idx;
        }
        if(flag){
            int cmpTo = allDates[idx]->compare(to);
            if(cmpTo == 1){
                endIdx = idx;
                break;
            }
        }
        idx++;
    }

    int i = 0;
    while(startIdx < endIdx){
        result.push_back(allDates[startIdx++]);
    }

    return result;
}