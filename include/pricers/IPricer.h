#pragma once

#include "models/IModel.h"
#include "derivatives/IDerivative.h"
#include "pnl/pnl_random.h"

/**
 * @brief Pricer Monte-Carlo
 * 
 */
class IPricer{
    public:
        /**
         * @brief Modèle utilisé.
         * 
         */
        IModel *model_; 
        /**
         * @brief Produit dérivé qu'on calcule.
         * 
         */
        IDerivative *derivative_;
        /**
         * @brief Moteur RNG.
         * 
         */
        PnlRng *rng_;
        /**
         * @brief Pas de constatation des produits.
         * 
         */
        double fdStep_;
        /**
         * @brief Nombre de tirages de Monte-Carlo.
         * 
         */
        int nbSamples_;
        /**
         * @brief Stocke les trajectoires des sous-jacents du modèle.
         * 
         */
        PnlMat *path_;
        /**
         * @brief Stocke les trajectoires shiftées.
         * 
         */
        PnlMat *shift_path_;

        /**
         * @brief Construct a new IPricer object
         * 
         * @param model 
         * @param derivative 
         * @param rng 
         * @param fdStep 
         * @param nbSamples 
         */
        IPricer(IModel * const model, IDerivative * const derivative, PnlRng * const rng, double fdStep, int nbSamples);
        /**
         * @brief Destroy the IPricer object
         * 
         */
        virtual ~IPricer();
        
        /**
         * @brief Calcule le prix et delta d'un produit à une date t.
         * 
         * @param spot Prix auxquelles on commence le Monte-Carlo.
         * @param t Date à partir duquelle on calcule.
         * @param prix[out] Prix calculé.
         * @param price_std_dev[out] Écart-type du prix.
         * @param delta[out] Delta calculé.
         * @param delta_std_dev[out] Écart-type du delta.
         */
        virtual void price_and_delta(const PnlVect * const spot, double t, double &prix, double &price_std_dev, PnlVect * const delta, PnlVect * const delta_std_dev) = 0;
        /**
         * @brief Calcule seulement le price d'un produit à une date t.
         * 
         * @param spot Prix auxquelles on commence le Monte-Carlo.
         * @param t Date à partir duquelle on calcule.
         * @param prix[out] Prix calculé.
         * @param price_std_dev[out] Écart-type du prix.
         */
        virtual void price(const PnlVect * const spot, double t, double &prix, double &price_std_dev) = 0;
        /**
         * @brief Incrémente et actualise les prix.
         * 
         * @param t Date t à actualiser.
         * @param prix[out] Prix.
         * @param std_dev[out] Écart-type du prix.
         */
        virtual void add_price(double t, double &prix, double &std_dev) = 0;
        /**
         * @brief Shift et incrémente le delta.
         * 
         * @param t Date t à actualiser.
         * @param delta[out] Delta.
         * @param std_dev[out] Écart-type du delta.
         */
        virtual void add_delta(double t, PnlVect * const delta, PnlVect * const std_dev) = 0;
};