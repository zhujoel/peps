#pragma once

#include "models/IModel.h"
#include "derivatives/IDerivative.h"
#include "pnl/pnl_random.h"

class IPricer{
    public:
        IModel *model_; // pointeur vers le modèle utilisé
        IDerivative *derivative_; // pointeur sur le produit dérivé qu'on calcule
        PnlRng *rng_; // pointeur sur le rng
        double fdStep_; // pas de constatation des produits
        int nbSamples_; // nombre de tirages de Monte-Carlo
        PnlMat *path_; // trajectoires des sous-jacents du modele
        PnlMat *shift_path_; /*! espace mémoire d'une trajectoire shiftée par (1+h) */

        IPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples);
        virtual ~IPricer();

        virtual void simulate(const PnlMat *past, double t, const PnlMat *sigma, double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev) = 0;
        virtual void price(double &prix, double &std_dev) = 0;
        virtual void delta(double t, PnlVect *delta, PnlVect *std_dev) = 0;
        virtual void discount_price(double t, double &prix, double &std_dev) = 0;
        virtual void discount_delta(double t, PnlVect *delta, PnlVect *std_dev) = 0;
};