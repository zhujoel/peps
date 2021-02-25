#pragma once

#include "pnl/pnl_matrix.h"

class IDerivative {
  public:
      double T_;        // maturité finale du produit
      int nbTimeSteps_; // nb de pas de temps de discrétisation
      int size_; // nombre de sous-jacents dans le payoff du produit

      IDerivative(double T, int nbTimeSteps, int size);
      virtual ~IDerivative();

      // sert à ajuster des sous-jacents étranger avec le zéro coupon
      virtual void adjust_sigma(PnlMat *sigma) = 0; // adjust computed sigma en fonction du nombre de produit dans la matrix path et de comment ils sont organisés
      virtual void adjust_past(PnlMat *past) = 0; 

      virtual double get_annee_payoff() = 0; // la date de maturité du produit, i.e. quand il paie
      virtual double payoff(const PnlMat *path) = 0;
};