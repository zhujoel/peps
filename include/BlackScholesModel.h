#pragma once

#include "IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        PnlVect *sigma_; /// vecteur de volatilités
        PnlVect *trend_; /// mu

        // TODO: à enlever si pas nécessaire
        PnlMat *L_; /// L Decomposition Cholesky de la matrice de Covariance
        PnlVect *G_; /// Vecteur Gaussien
        PnlVect *B_; // Brownien (produit matriciel L * G);

        // TODO: modifier le rho en matrice
        BlackScholesModel(int size, double rd, double rho, PnlVect *spot, PnlVect *sigma);
        ~BlackScholesModel();
        /**
        * Sets the trend attribute
        * 
        * @param[in] trend mu
        */
        void setTrend(PnlVect *trend);
        
        /**
        * Génère une trajectoire du modèle et la stocke dans path
        *
        * @param[out] path contient une trajectoire du modèle.
        * C'est une matrice de taille (nbTimeSteps+1) x d
        * @param[in] T  maturité
        * @param[in] nbTimeSteps nombre de dates de constatation
        */
        void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);
};