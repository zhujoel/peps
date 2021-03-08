#pragma once

#include "pnl/pnl_matrix.h"
#include "libs/DateTime.h"
#include <vector>

class InterestRate{
    public:
        double t_;
        DateTime *current_date_;
        std::vector<DateTime*> all_dates_;
        PnlMat *interest_path_;

        InterestRate(double t, DateTime *current_date, std::vector<DateTime*> all_dates, PnlMat *interest_path);
        ~InterestRate();

        void set_current_date(double t, DateTime *current_date);
        // double actif_sans_risque_etranger();
        double get_domestic_rate();
        // double integrate();
};