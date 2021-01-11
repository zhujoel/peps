#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "IDerivative.h"

class IModel{
    public:
        IDerivative *derivative_; // derivative to simulate
        PnlVect *rd_; /// taux d'intérêt domestique
        PnlMat *sigma_; // matrice de covariance
        double nbTimeSteps_;
        PnlRng *rng_;

        IModel(IDerivative *derivative, PnlVect *rd, PnlMat *sigma, double nbTimeSteps, PnlRng *rng);
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
        virtual void asset(PnlVect *path, int ind) = 0;

        virtual void price_all() = 0;
        
        /**
        * Shift d'une trajectoire du sous-jacent
        *
        * @param[in]  path contient en input la trajectoire
        * du sous-jacent
        * @param[out] shift_path contient la trajectoire path
        * dont la composante d a été shiftée par (1+h)
        * à partir de la date t.
        * @param[in] t date à partir de laquelle on shift
        * @param[in] h pas de différences finies
        * @param[in] d indice du sous-jacent à shifter
        * @param[in] timestep pas de constatation du sous-jacent
        */
        // virtual void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep) = 0;

};