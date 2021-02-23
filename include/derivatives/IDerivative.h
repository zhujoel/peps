#pragma once

#include "pnl/pnl_matrix.h"

// Produit dérivé abstrait
class IDerivative {
  public:
      // double T_;        // maturité finale du produit
      // int nbTimeSteps_; // nb de pas de temps de discrétisation
      // int size_; // nombre de sous-jacents dans le payoff du produit

      // IDerivative(double T, int nbTimeSteps, int size);
      // virtual ~IDerivative();

      // // SERT A CALCULER LE SIGMA AVEC LES ZERO COUPONS 
      // virtual void adjust_sigma(PnlMat *sigma) = 0; // adjust computed sigma en fonction du nombre de produit dans la matrix path et de comment ils sont organisés
      // virtual void adjust_spot(PnlVect *spot) = 0; // same as above

      // virtual double getMaturity() = 0;
      // /**
      // * Calcule la valeur du payoff sur la trajectoire
      // *
      // * @param[in] path est une matrice de taille (N+1) x d
      // * contenant une trajectoire du modèle telle que créée
      // * par la fonction asset.
      // * @return le payoff du produit
      // */
      // virtual double payoff(const PnlMat *path) = 0;
};