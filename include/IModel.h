#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "QuantoOption.h"

class IModel{
    public:
        int size_; /// nombre d'actifs du modèle
        double rd_; /// taux d'intérêt domestique (domestic rate) // TODO: à changer si on a besoin qu'il ne soit pas constant 
        PnlMat *sigma_; /// Matrice de volatilité
        PnlVect *spot_; /// valeurs initiales des sous-jacents

        IModel(int size, double rd, PnlMat *sigma, PnlVect *spot);
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
        virtual void asset2(QuantoOption *derivative, double T, int nbTimeSteps, PnlRng *rng) = 0;

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
        virtual void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep) = 0;
        virtual void shiftAsset2(QuantoOption *derivative, int d, double h, double t, double timestep) = 0;
};