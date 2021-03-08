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

        IPricer(IModel * const model, IDerivative * const derivative, PnlRng * const rng, double fdStep, int nbSamples);
        virtual ~IPricer();
        
        virtual void price_and_delta(const PnlMat * const past, double t, const PnlMat * const sigma, double &prix, double &price_std_dev, PnlVect * const delta, PnlVect * const delta_std_dev) = 0;
        virtual void price(const PnlMat * const past, double t, const PnlMat * const sigma, double &prix, double &price_std_dev) = 0;
        virtual void add_price(double t, double &prix, double &std_dev) = 0;
        virtual void add_delta(double t, int pastSize, PnlVect * const delta, PnlVect * const std_dev) = 0;
};