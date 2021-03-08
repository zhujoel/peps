
#include <stdexcept>
#include "libs/HedgingPortfolio.h"
#include "pnl/pnl_mathtools.h"

HedgingPortfolio::HedgingPortfolio(double prix, const PnlVect * const delta, const PnlVect * const share_values, InterestRate * const rates, double val_liquidative_initiale)
{
    double marge_initiale = val_liquidative_initiale - prix;
    if (marge_initiale<0) 
    {
        throw std::runtime_error("La marge (val_liquidative_initiale - prix) ne peut pas être négative !");
    } 
    this->val_liquidative_initiale_ = val_liquidative_initiale;
    this->last_rebalancing_t_ = 0;
    this->rates_ = rates;
    this->delta_ = pnl_vect_new();
    pnl_vect_clone(this->delta_, delta);
    this->V1_ = prix - pnl_vect_scalar_prod(delta, share_values);
    this->V2_ = val_liquidative_initiale - pnl_vect_scalar_prod(delta, share_values);
}

HedgingPortfolio::~HedgingPortfolio()
{
    pnl_vect_free(&this->delta_);
}

void HedgingPortfolio::rebalancing(double t, const PnlVect * const delta, const PnlVect * const share_values)
{
    this->V1_ *= exp(this->rates_->get_domestic_rate()*(t-this->last_rebalancing_t_)); 
    this->V2_ *= exp(this->rates_->get_domestic_rate()*(t-this->last_rebalancing_t_));

    double risk_variation = 0.;
    for (int d = 0; d < delta->size; ++d)
    {
        risk_variation += (GET(delta, d) - GET(this->delta_, d)) * GET(share_values, d);
    }
    this->V1_ -= risk_variation;
    this->V2_ -= risk_variation;
    
    this->last_rebalancing_t_ = t;
    pnl_vect_clone(this->delta_, delta);
}

double HedgingPortfolio::get_portfolio_value(double t, const PnlVect * const share_values) const
{
    return exp(this->rates_->get_domestic_rate()*(t-this->last_rebalancing_t_))*this->V1_ + pnl_vect_scalar_prod(this->delta_, share_values);
}

double HedgingPortfolio::get_tracking_error(double t, double prix, const PnlVect * const share_values) const
{
    return this->get_portfolio_value(t, share_values) - prix;
}

double HedgingPortfolio::get_valeur_liquidative(double t, const PnlVect * const share_values) const
{
    return exp(this->rates_->get_domestic_rate()*(t-this->last_rebalancing_t_))*this->V2_ + pnl_vect_scalar_prod(this->delta_, share_values);
}

double HedgingPortfolio::get_FinalPnL(double t, double payoff, const PnlVect * const share_values) const
{
    return this->get_valeur_liquidative(t, share_values) - payoff;
}