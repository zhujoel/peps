#pragma once

#include "IModel.h"
#include "pnl/pnl_vector.h"

class BlackScholesModel : public IModel{
    public:
        PnlVect *trend_; /// mu
        PnlMat *sigma_; /// L Decomposition Cholesky de la matrice de Covariance

        // on l'avait pour l'espace mémoire -- TODO: à enlever si pas nécessaire
        PnlVect *G_; /// Vecteur Gaussien
        PnlVect *B_; // Brownien (produit matriciel L * G);

        BlackScholesModel(int size, double rd, PnlMat *cov, PnlVect *spot);
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