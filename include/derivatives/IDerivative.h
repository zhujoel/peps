#pragma once

#include "pnl/pnl_matrix.h"

/**
 * @brief Gère un produit dérivé
 */
class IDerivative {
  public:
      /**
      * @brief Maturité finale du produit.
      */
      double T_;
      /**
      * @brief Nombre de produits dans la matrice path.
      */
      int size_;

      /**
       * @brief Construct a new IDerivative object
       * 
       * @param T 
       * @param size 
       */
      IDerivative(double T, int size);
      /**
       * @brief Destroy the IDerivative object
       */
      virtual ~IDerivative();

      // sert à ajuster des sous-jacents étranger avec le zéro coupon
      /**
       * @brief Ajuster les valeurs des sigmas du produit pour réaliser les calculs en devise locale.
       * Ne fait rien s'il n'y a pas de sous-jacents étrangers ou s'il n'y a pas besoin de le faire.
       * Les sigmas sont ajustés en fonction du nombre de produits dans le path et de comment ils sont organisé dans la matrice.
       * 
       * @param sigma Les sigmas à ajuster.
       */

      virtual void adjust_sigma(PnlMat * const sigma) const = 0;
      /**
       * @brief Ajuste les valeurs de la matrice des sous-jacents pour réaliser les calculs en devise locale.
       * Capitalise également les valeurs en fonction des taux sans risques étrangers.
       * Ne fait rien s'il n'y a pas de sous-jacents étrangers ou s'il n'y a pas besoin de le faire.
       * Les prix sont ajustés en fonction du nombre de produits dans le path et de comment ils sont organisé dans la matrice.
       * 
       * @param past La matrice de prix past à ajuster.
       */

      virtual void adjust_past(PnlMat * const past) const = 0; 
      /**
       * @brief Ajuste les valeurs des sous-jacents à une date t pour réaliser les calculs en devise locale.
       * Capitalise également les valeurs en fonction des taux sans risques étrangers.
       * Ne fait rien s'il n'y a pas de sous-jacents étrangers ou s'il n'y a pas besoin de le faire.
       * Les prix sont ajustés en fonction du nombre de produits dans le spot et de comment ils sont organisé dans le vecteur.
       * 
       * @param spot Le vecteur spot à ajuster.
       * @param t La date t à laquelle capitaliser.
       */

      virtual void adjust_spot(PnlVect * const spot, double t) const = 0;
      /**
       * @brief Calcule l'année de payoff du produit dérivé.
       * 
       * @return double L'année de payoff du produit.
       */
      virtual double get_annee_payoff() const = 0;
      
      /**
       * @brief Calcule le payoff du produit dérivé.
       * 
       * @param path Matrice des prix des sous-jacents du produit dérivé.
       * @return double Le payoff final du produit dérivé.
       */
      virtual double payoff(const PnlMat *path) = 0;
};