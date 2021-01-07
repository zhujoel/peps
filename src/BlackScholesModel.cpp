#include "BlackScholesModel.h"

BlackScholesModel::BlackScholesModel(int size, double rd, double rho, PnlVect *spot, PnlVect *sigma) :
IModel(size, rd, rho, spot)
{
    this->sigma_ = sigma;
    this->L_ = pnl_mat_create_from_scalar(this->size_, this->size_, this->rho_);
    pnl_mat_set_diag(this->L_, 1, 0);
    pnl_mat_chol(this->L_);
    this->G_ = pnl_vect_create(this->size_); 
    this->B_ = pnl_vect_create(this->size_);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&this->spot_);
    pnl_vect_free(&this->sigma_);
    pnl_mat_free(&this->L_);
    pnl_vect_free(&this->G_);
    pnl_vect_free(&this->B_);
}

void BlackScholesModel::setTrend(PnlVect *trend){
    this->trend_ = trend;
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
{
    double timestep = T/nbTimeSteps;
    pnl_mat_set_row(path, this->spot_, 0);

    for (int k = 1; k <= nbTimeSteps; ++k)
    {
        pnl_vect_rng_normal(this->G_, this->size_, rng); // G Vecteur gaussien
        pnl_mat_mult_vect_inplace(this->B_, this->L_, this->G_);
        for (int d = 0; d < this->size_; ++d)
        {
            double sigma_d = 0.0;
            // TODO: faire ça propre, le sigma est différent en fonction de si c'est un actif risqué ou sans risque
            // car l'actif sans risque devient risqué
            // et l'actif risqué à son risque + le tx de change
            if(d == 0){
                sigma_d = GET(this->sigma_, d);
            }
            else if(d == 1){
                sigma_d = GET(this->sigma_, d) + GET(this->sigma_, d-1);
            }
            MLET(path, k, d) = MGET(path, k-1, d) * exp( (this->rd_ - (sigma_d*sigma_d)/2 ) * timestep + sigma_d * sqrt(timestep) * GET(this->B_, d));
        }
    }
}
