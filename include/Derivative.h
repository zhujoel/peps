#pragma once

#include "pnl/pnl_matrix.h"

// Produit dérivé abstrait
class Derivative {
  public:
      double T_;        // maturité finale du produit
      int nbTimeSteps_; // nb de pas de temps de discrétisation

      Derivative(double T, int nbTimeSteps);
      virtual ~Derivative();
      /**
      * Calcule la valeur du payoff sur la trajectoire
      *
      * @param[in] path est une matrice de taille (N+1) x d
      * contenant une trajectoire du modèle utelle que créée
      * par la fonction asset.
      * @return le payoff du produit
      */
      virtual double payoff(const PnlMat *path) const = 0;
};