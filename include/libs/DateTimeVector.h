#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"

class DateTimeVector{
    public:
        DateTime **dates_;
        int nbDates_;

        DateTimeVector(std::string fileName, int nbDates);
        ~DateTimeVector();

        DateTime *operator[](int index);
};


// LES DATES DOIVENT ETRE DANS L'ORDRE
void calcul_indices_dates(DateTimeVector *all_dates, DateTimeVector *dates, PnlVectInt *indices);