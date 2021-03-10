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
        PnlVect *rates_;

        InterestRate(double t, DateTime * const current_date, const std::vector<DateTime*> &all_dates, PnlMat * const interest_path);
        ~InterestRate();

        double get_domestic_rate();
        double get_foreign_rate(int index);
        double compute_domestic_risk_free_asset(double t0, double t1);
        double compute_foreign_risk_free_asset(double t0, double t1, int index);
};