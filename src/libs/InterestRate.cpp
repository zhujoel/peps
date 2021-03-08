#include "libs/InterestRate.h"

InterestRate::InterestRate(double t, DateTime *current_date, std::vector<DateTime*> all_dates, PnlMat *interest_path){
    this->t_ = t;
    this->current_date_ = current_date;
    this->all_dates_ = all_dates;
    this->interest_path_ = interest_path;
}

InterestRate::~InterestRate(){}

void InterestRate::set_current_date(double t, DateTime *current_date){
    this->t_ = t;
    this->current_date_ = current_date;
}

double InterestRate::get_domestic_rate(){
    // idx = get_indice_from_date(this->all_dates, this->current_date_);
    // return MGET(this->interest_path_, idx, 3);
    return 0.03;
}