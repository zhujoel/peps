#pragma once

#include <ostream>
#include "pnl/pnl_vector.h"

class DateTime{
    public:
        int dd_; // number of day
        int mm_; // number of month
        int yyyy_; // number of year

        DateTime(int dd_, int mm_, int yyyy_);
        ~DateTime();
        
        int compare(const DateTime *dt) const;

};


std::ostream &operator<<(std::ostream &output, const DateTime *dt);
void fill_dates_from_file(DateTime **dates, std::string fileName, int nbDates);
void calcul_indices_dates(DateTime **all_dates, int all_dates_size, DateTime **dates, PnlVectInt *indices);