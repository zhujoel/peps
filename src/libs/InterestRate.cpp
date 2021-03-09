#include "libs/InterestRate.h"

InterestRate::InterestRate(double t, DateTime * const current_date, const std::vector<DateTime*> &all_dates, PnlMat * const interest_path){
    this->t_ = t;
    this->current_date_ = current_date;
    this->all_dates_ = all_dates;
    this->interest_path_ = interest_path;
}

InterestRate::~InterestRate(){}

void InterestRate::set_current_date(double t, DateTime * const current_date){
    this->t_ = t;
    this->current_date_ = current_date;
}

double InterestRate::get_domestic_rate(){
    int idx = get_indice_from_date(this->all_dates_, this->current_date_);
    return MGET(this->interest_path_, idx, this->interest_path_->m-1);
}