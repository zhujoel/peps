#pragma once

#include "IModel.h"
#include "IDerivative.h"
#include "pnl/pnl_random.h"

class IPricer{
    public:
        IModel *model_; // pointeur vers le modèle utilisé
        IDerivative *derivative_; // pointeur sur le produit dérivé qu'on calcule
        PnlRng *rng_; // pointeur sur le rng -- TODO: modifier par un RNGEngine
        double fdStep_; // pas de constatation des produits -- TODO: gérer les dates plus précisément pour Océlia
        int nbSamples_; // nombre de tirages de Monte-Carlo
        PnlMat *path_; // trajectoires des sous-jacents du modele
        PnlMat *shift_path_; /*! espace mémoire d'une trajectoire shiftée par (1+h) */

        IPricer(IModel *model, IDerivative *derivative, PnlRng *rng, double fdStep, int nbSamples);
        ~IPricer();

        /**
         * Simulate the Monte-Carlo and computes the price, a standard deviation for the price, the delta, and the standard-deviation for the delta at time 0.
         *
         */
        virtual void simulate(QuantoOption *option, double &prix, double &price_std_dev, PnlVect *delta, PnlVect *delta_std_dev) = 0;

        /**
         * Calcule le prix d'un produit à la date 0.
         * @param prix[out] Prix estimé par le IPricer.
         * @param std_dev[out] Interval de confiance du prix estimé.
         *
         */
        virtual void price(double &prix, double &std_dev) = 0;

        /**
        * Calcule le delta de l'option à la date 0
        *
        * @param[out] delta contient le vecteur de delta
        * @param[out] std_dev contient l'écart type de l'estimateur
        */
        virtual void delta(QuantoOption *option, PnlVect *delta, PnlVect *std_dev) = 0;

        /**
         * @brief Computes the discount price and discount standard deviation.
         * 
         * @param t 
         * @param prix 
         * @param std_dev 
         */
        virtual void discount_price(double t, double &prix, double &std_dev) = 0;
        virtual void discount_delta(double t, PnlVect *delta, PnlVect *std_dev) = 0;

};