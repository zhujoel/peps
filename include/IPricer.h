#pragma once

#include "IModel.h"
#include "Derivative.h"
#include "pnl/pnl_random.h"

class IPricer{
    public:
        IModel *model_; // pointeur vers le modèle utilisé
        Derivative *derivative_; // pointeur sur le produit dérivé qu'on calcule
        PnlRng *rng_; // pointeur sur le rng -- TODO: modifier par un RNGEngine
        double fdStep_; // pas de constatation des produits -- TODO: gérer les dates plus précisément pour Océlia
        int nbSamples_; // nombre de tirages de Monte-Carlo
        PnlMat *path_; // trajectoires des sous-jacents du modele 

        IPricer(IModel *model, Derivative *derivative, PnlRng *rng, double fdStep, int nbSamples);
        ~IPricer();
        /**
         * Calcule le prix d'un produit à la date 0.
         * @param prix[out] Prix estimé par le IPricer.
         * @param std_dev[out] Interval de confiance du prix estimé.
         *
         */
        virtual void price(double &prix, double &std_dev) = 0;
        
        // TODO: implémenter le delta pour hedger
        // void delta(PnlVect *delta, PnlVect *std_dev);

};