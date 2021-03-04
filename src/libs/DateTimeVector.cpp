#include "libs/DateTimeVector.h"
#include <fstream>
#include <sstream>
#include <string>

void parse_dates_file(std::vector<DateTime*> &dates, const std::string &fileName, int nbDates, char delimiter)
{
    std::ifstream datesStream(fileName);
    std::string date;
    for (int i = 0; i < nbDates; i++)
    {
        getline(datesStream, date);
        dates.push_back(parse_date_string(date, delimiter));
    }
}

void calcul_indices_dates(PnlVectInt *indices, const std::vector<DateTime*> &all_dates, const std::vector<DateTime*> &dates_subset)
{
    pnl_vect_int_resize(indices, dates_subset.size());
    int cnt = 0;
    for(unsigned int i = 0; i < all_dates.size(); ++i){
        if(all_dates[i]->compare(dates_subset[cnt]) == 0){
            LET_INT(indices, cnt++) = i;
            if(cnt == indices->size){
                break;
            }
        }
    }
}

int get_indice_from_date(const std::vector<DateTime*> &all_dates, const DateTime *date)
{
    for(unsigned int i = 0; i < all_dates.size(); ++i){
        if(all_dates[i]->compare(date) == 0){
            return i;
        }
    }
    return -1;
}

void same_dates(std::vector<DateTime*> &same_dates, const std::vector<DateTime*> &v1, const std::vector<DateTime*> &v2)
{
    unsigned int idx1 = 0;
    unsigned int idx2 = 0;
    while(idx1 < v1.size() && idx2 < v2.size()){
        int cmp = v1[idx1]->compare(v2[idx2]);
        if(cmp == 0){
            same_dates.push_back(v1[idx1]->copy());
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
}

void get_prices_from_date(PnlVect *prices, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlVect *allPrices)
{
    PnlVectInt* indices = pnl_vect_int_new();
    calcul_indices_dates(indices, allDates, subset);

    pnl_vect_resize(prices, indices->size);
    for(int i = 0; i < indices->size; ++i){
        LET(prices, i) = GET(allPrices, GET_INT(indices, i));
    }
    pnl_vect_int_free(&indices);
}

void get_subset_path_from_dates(PnlMat *subset_path, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlMat *path)
{
    PnlVectInt* indices = pnl_vect_int_new();
    calcul_indices_dates(indices, allDates, subset);
    pnl_mat_resize(subset_path, indices->size, path->n);
    for(int i = 0; i < indices->size; ++i){
        for(int j = 0; j < path->n; ++j){
            MLET(subset_path, i, j) = MGET(path, GET_INT(indices, i), j);
        }
    }
    pnl_vect_int_free(&indices);
}

void from_date_to_date(std::vector<DateTime*> &from_to, const std::vector<DateTime*> &allDates, const DateTime *from, const DateTime *to) // includes both from and to
{
    unsigned int idx = 0;
    bool hasStarted = false;

    while(idx < allDates.size()){
        int cmpFrom = allDates[idx]->compare(from);
        int cmpTo = allDates[idx]->compare(to);
        if(hasStarted && cmpTo == 1){
            break;
        }
        if(cmpFrom >= 0){
            from_to.push_back(allDates[idx]);
            hasStarted = true;
        }
        idx++;
    }
}

void delete_date_vector(std::vector<DateTime*> &list)
{
    for( std::vector<DateTime*>::iterator i = list.begin(), endI = list.end(); i != endI; ++i)
    {
        delete *i;
    }
    list.clear();
}