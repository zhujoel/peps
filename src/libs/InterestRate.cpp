#include "libs/InterestRate.h"
#include "libs/DateTimeVector.h"
#include "pnl/pnl_mathtools.h"

InterestRate::InterestRate(double t, DateTime * const current_date, const std::vector<DateTime*> &all_dates, PnlMat * const interest_path){
    this->t_ = t;
    this->current_date_ = current_date;
    this->all_dates_ = all_dates;
    this->interest_path_ = interest_path;
    this->rates_ = pnl_vect_create_from_zero(this->interest_path_->n);
    // int idx = get_indice_from_date(this->all_dates_, this->current_date_);
    // pnl_mat_get_row(this->rates_, this->interest_path_, idx);
    LET(this->rates_, 3) = 0.03;
}

InterestRate::~InterestRate(){
    pnl_vect_free(&this->rates_);
}

double InterestRate::get_domestic_rate(){
    return GET(this->rates_, this->rates_->size-1);
}

double InterestRate::get_foreign_rate(int index){
    return GET(this->rates_, index);
}

double InterestRate::compute_domestic_risk_free_asset(double t0, double t1){
    return exp(this->get_domestic_rate()*(t1-t0));
}

double InterestRate::compute_foreign_risk_free_asset(double t0, double t1, int index){
    return exp(this->get_foreign_rate(index)*(t1-t0));
}
