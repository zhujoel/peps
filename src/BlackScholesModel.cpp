#include "BlackScholesModel.h"
#include <iostream>

BlackScholesModel::BlackScholesModel() : IModel()
{
}

BlackScholesModel::~BlackScholesModel()
{
}

void BlackScholesModel::asset(PnlVect *path, double T, int nbTimeSteps, PnlRng *rng, PnlMat *sigma, double spot, double r, int size, int ind)
{
//     double timestep = T/nbTimeSteps;
//     LET(path, 0) = spot;

//     PnlVect *G = pnl_vect_create(this->size_); 
//     PnlVect *B = pnl_vect_create(this->size_);

//     for (int k = 1; k <= nbTimeSteps; ++k)
//     {
//         pnl_vect_rng_normal(G, size, rng); // G Vecteur gaussien
//         pnl_mat_mult_vect_inplace(B, sigma, G);

//         double sigma_d = pnl_vect_norm_two(sigma);
//         LET(path, k) = GET(path, k-1) * exp( (r - (sigma_d*sigma_d)/2 ) * timestep + sqrt(timestep) * GET(B, ind));
//     }
}
