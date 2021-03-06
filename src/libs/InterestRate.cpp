#include "libs/InterestRate.h"

InterestRate::InterestRate(double t, DateTime *current_date, std::vector<DateTime*> all_dates, PnlMat *interest_path){
    this->t_ = t;
    this->current_date_ = current_date;
    this->all_dates_ = all_dates;
    this->interest_path_ = interest_path;
}

InterestRate::~InterestRate(){}