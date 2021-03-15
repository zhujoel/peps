#include "models/BlackScholesModel.h"
#include "libs/MathLib.h"
#include "libs/Utilities.h"

BlackScholesModel::BlackScholesModel(int size, int nbTimeSteps, InterestRate * const rates, PnlVect * const computed_ti) 
    : IModel(size, nbTimeSteps, rates, computed_ti)
{
    this->G_ = pnl_vect_create(this->size_); 
    this->B_ = pnl_vect_create(this->size_);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&this->G_);
    pnl_vect_free(&this->B_);
}

void BlackScholesModel::asset(PnlMat * const path, double t, PnlRng * const rng, const PnlVect * const spot)
{
    double startIndex = first_index_gte(this->computed_ti_, t); // TODO: on peut peut-etre opti en faisant un calcul direct
    double timestep = GET(this->computed_ti_, startIndex) - t;

    // calcule 1 prix par rapport au spot
    pnl_vect_rng_normal(this->G_, this->size_, rng); // G Vecteur gaussien
    pnl_mat_mult_vect_inplace(this->B_, this->sigma_, this->G_);
    for (int d = 0; d < this->size_; ++d)
    {
        double sigma_d = GET(this->volatility_, d);
        MLET(path, startIndex, d) = GET(spot, d) * exp( (this->rates_->get_domestic_rate() - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, d));
    }

    // calcule reste de la trajectoire 
    for (int k = startIndex+1; k < this->computed_ti_->size; ++k)
    {
        pnl_vect_rng_normal(this->G_, this->size_, rng); // G Vecteur gaussien
        pnl_mat_mult_vect_inplace(this->B_, this->sigma_, this->G_);
        timestep = GET(this->computed_ti_, k) - GET(this->computed_ti_, k-1);
        for (int d = 0; d < this->size_; ++d)
        {
            double sigma_d = GET(this->volatility_, d);
            MLET(path, k, d) = MGET(path, k-1, d) * exp( (this->rates_->get_domestic_rate() - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(this->B_, d));
        }
    }
}

void BlackScholesModel::shift_asset(PnlMat * const shift_path, double t, const PnlMat * const path, int d, double h) const
{
    double startIndex = first_index_gte(this->computed_ti_, t); // TODO: on peut peut-etre opti en faisant un calcul direct
    if(GET(this->computed_ti_, startIndex) == t){ // si on est une date de constatation, on n'a pas besoin de shift le spot
        startIndex += 1;
    }
    if (h>0)
    {
        pnl_mat_clone(shift_path, path);
        for (int k = startIndex; k < path->m; ++k)
        {  
            MLET(shift_path, k, d) = MGET(shift_path, k, d) * (1+h);
        }
    } else {
        for (int k = startIndex; k < path->m; ++k)
        {  
            MLET(shift_path, k, d) = (MGET(shift_path, k, d) / (1-h)) * (1+h);
        }
    }
}