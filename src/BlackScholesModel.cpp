#include "BlackScholesModel.h"
#include <iostream>

BlackScholesModel::BlackScholesModel() : IModel()
{
}

BlackScholesModel::~BlackScholesModel()
{
}

void BlackScholesModel::asset(PnlVect *path, double T, int nbTimeSteps, PnlRng *rng, PnlMat *sigma, double r, int ind)
{
    double timestep = T/nbTimeSteps;

    double size = sigma->n;
    PnlVect *G = pnl_vect_create(size); 
    PnlVect *B = pnl_vect_create(size);    
    PnlVect *row = pnl_vect_create(size);

    for (int k = 1; k <= nbTimeSteps; ++k)
    {
        pnl_vect_rng_normal(G, size, rng);
        pnl_mat_mult_vect_inplace(B, sigma, G);
        pnl_mat_get_row(row, sigma, ind);
        double sigma_d = pnl_vect_norm_two(row);

        LET(path, k) = GET(path, k-1) * exp( (r - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(B, ind));
    }
}
