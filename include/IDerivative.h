#pragma once

#include "pnl/pnl_matrix.h"
#include "IUnderlying.h"

// Produit dérivé abstrait
class IDerivative {
  public:
      double T_;        // maturité finale du produit
      int nbTimeSteps_; // nb de pas de temps de discrétisation
      int size_; // nombre de sous-jacents dans le payoff du produit
      double rd_; // taux d'intéret domestique // TODO: faire en sorte que ça soit pas constant
      IUnderlying **underlyings_;

      IDerivative(double T, int nbTimeSteps, int size, double rd, IUnderlying** underlying);
      virtual ~IDerivative();
      
      /**
      * Calcule la valeur du payoff sur la trajectoire
      *
      * @param[in] path est une matrice de taille (N+1) x d
      * contenant une trajectoire du modèle telle que créée
      * par la fonction asset.
      * @return le payoff du produit
      */
      virtual double payoff() = 0;
      virtual double shifted_payoff() const = 0;
};