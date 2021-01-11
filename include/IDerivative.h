#pragma once

#include "pnl/pnl_matrix.h"
#include "IUnderlying.h"

// Produit dérivé abstrait
class IDerivative {
  public:
      double T_;        // maturité finale du produit
      int nbTimeSteps_; // nb de pas de temps de discrétisation
      int size_; // nombre de sous-jacents dans le payoff du produit
      IUnderlying **underlyings_;

      IDerivative(double T, int nbTimeSteps, int size, IUnderlying** und);
      virtual ~IDerivative();
      
      /**
      * Calcule la valeur du payoff sur la trajectoire
      *
      * @param[in] path est une matrice de taille (N+1) x d
      * contenant une trajectoire du modèle telle que créée
      * par la fonction asset.
      * @return le payoff du produit
      */
      virtual double payoff() const = 0;
};