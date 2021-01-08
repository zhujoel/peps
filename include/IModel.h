#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

class IModel{
    public:
        int size_; /// nombre d'actifs du modèle
        double rd_; /// taux d'intérêt domestique (domestic rate) // TODO: à changer si on a besoin qu'il ne soit pas constant 
        // double rho_; /// paramètre de corrélation // TODO: modifier en matrice
        PnlMat *cov_; /// Matrice de covariance
        PnlVect *spot_; /// valeurs initiales des sous-jacents

        // IModel(int size, double rd, double rho, PnlVect *spot);
        IModel(int size, double rd, PnlMat *cov, PnlVect *spot);
        ~IModel();
        /**
        * Génère une trajectoire du modèle et la stocke dans path
        *
        * @param[out] path contient une trajectoire du modèle.
        * C'est une matrice de taille (nbTimeSteps+1) x size_. Attention, chaque colonne de la matrice à une signification particulière.
        * @param[in] T  maturité
        * @param[in] nbTimeSteps nombre de dates de constatation
        * @param[in] rng Moteur de rng -- TODO: à changer
        */
        virtual void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) = 0;
};