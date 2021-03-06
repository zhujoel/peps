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

        // double actif_sans_risque_etranger();
        // double taux_overnight();
        // double integrate();
};