#pragma once

#include "derivatives/IDerivative.h"

/**
 * @brief Option Quanto sur un actif étranger avec un strike en devise domestique.
 * Utilisé pour tester préalablement le code sur un modèle simple.
 * 
 */
class QuantoOption : public IDerivative {

  public:
    /**
     * @brief Strike en devise domestique de l'option Quanto.
     * 
     */
    double K_;
    /**
     * @brief Taux d'intérêt étranger. On considère ici que c'est un taux constante.
     * 
     */
    double rf_;
    
    /**
     * @brief Construct a new Quanto Option object
     * 
     * @param T 
     * @param size 
     * @param rf Taux d'intérêt étranger constant.
     * @param K 
     */
    QuantoOption(double T, int size, double rf, double K);
    /**
     * @brief Destroy the Quanto Option object
     * 
     */
    ~QuantoOption();
    
    /**
     * @brief Ne fait rien.
     * 
     * @param sigma 
     */
    void adjust_sigma(PnlMat * const sigma) const;
    /**
     * @brief Ne fait rien.
     * 
     * @param past 
     */
    void adjust_past(PnlMat * const past) const;
    /**
     * @brief Ne fait rien.
     * 
     * @param spot 
     * @param t 
     */
    void adjust_spot(PnlVect * const spot, double t) const;

    double get_annee_payoff() const;
    double payoff(const PnlMat *path);
};