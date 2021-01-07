#pragma once

#include "pnl/pnl_matrix.h"

// Produit dérivé abstrait
class Derivative {
  public:
      double T_;        // maturité finale du produit
      int nbTimeSteps_; // nb de pas de temps de discrétisation
      int size_; // nombre de colonnes dans le path

      Derivative(double T, int nbTimeSteps, int size);
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