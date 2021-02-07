#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"

class DateTimeVector{
    public:
        DateTime **dates_;
        int nbDates_;

        DateTimeVector(int nbDates);
        ~DateTimeVector();

        void resize(int nbDates);
        DateTime *operator[](int index);
};

void calcul_indices_dates(DateTimeVector *all_dates, DateTimeVector *dates, PnlVectInt *indices); // LES DATES DOIVENT ETRE DANS L'ORDRE
DateTimeVector* parseDatesFile(std::string fileName, int nbDates, char delimiter);
void sameDates(DateTimeVector *v1, DateTimeVector *v2, DateTimeVector *result);
void getPricesFromDate(DateTimeVector *allDates, DateTimeVector *relevantDates, PnlVect *allPrices, PnlVect *result);