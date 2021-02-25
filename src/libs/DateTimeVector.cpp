#include "libs/DateTimeVector.h"
#include <fstream>
#include <sstream>
#include <string>

std::vector<DateTime*> parse_dates_file(std::string fileName, int nbDates, char delimiter)
{
    std::vector<DateTime*> dates;
    std::ifstream datesStream(fileName);
    std::string date;
    for (int i = 0; i < nbDates; i++)
    {
        getline(datesStream, date);
        dates.push_back(parse_date_string(date, delimiter));
    }
    return dates;
}

PnlVectInt* calcul_indices_dates(std::vector<DateTime*> all_dates, std::vector<DateTime*> dates)
{
    PnlVectInt *indices = pnl_vect_int_create(dates.size());
    int cnt = 0;
    for(unsigned int i = 0; i < all_dates.size(); ++i){
        if(all_dates[i]->compare(dates[cnt]) == 0){
            LET_INT(indices, cnt++) = i;
            if(cnt == indices->size){
                break;
            }
        }
    }
    return indices;
}

std::vector<DateTime*> same_dates(std::vector<DateTime*> v1, std::vector<DateTime*> v2)
{
    std::vector<DateTime*> result;
    unsigned int len1 = v1.size();
    unsigned int len2 = v2.size();
    unsigned int idx = 0;
    unsigned int idx1 = 0;
    unsigned int idx2 = 0;
    std::vector<DateTime*> tmp_vect;
    while(idx < std::max(len1, len2) && idx1 < len1 && idx2 < len2){
        int cmp = v1[idx1]->compare(v2[idx2]);
        if(cmp == 0){
            result.push_back(v1[idx1]->copy());
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

    return result;   
}

PnlVect* get_prices_from_date(std::vector<DateTime*> allDates, std::vector<DateTime*> subset, PnlVect *allPrices)
{
    PnlVectInt* indices = calcul_indices_dates(allDates, subset);
    PnlVect *prices = pnl_vect_create(indices->size);
    for(int i = 0; i < indices->size; ++i){
        LET(prices, i) = GET(allPrices, GET_INT(indices, i));
    }
    pnl_vect_int_free(&indices);
    return prices;
}

std::vector<DateTime*> from_date_to_date(std::vector<DateTime*> allDates, DateTime *from, DateTime *to)
{
    std::vector<DateTime*> result;
    unsigned int idx = 0;
    bool hasStarted = false;

    while(idx < allDates.size()){
        int cmpFrom = allDates[idx]->compare(from);
        int cmpTo = allDates[idx]->compare(to);
        if(hasStarted && cmpTo == 1){
            break;
        }
        if(cmpFrom >= 0){
            result.push_back(allDates[idx]);
            hasStarted = true;
        }
        idx++;
    }

    return result;
}

void delete_date_vector(std::vector<DateTime*> list)
{
    for( std::vector<DateTime*>::iterator i = list.begin(), endI = list.end(); i != endI; ++i)
    {
        delete *i;
    }
    list.clear();
}