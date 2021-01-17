#pragma once

#include "pnl/pnl_vector.h"

class IUnderlying {

  public:
    PnlVect *price_; // prix dans la monnaie étrangère
    PnlVect *price_converted_; // prix converti dans la monnaie domestique
    PnlVect *shifted_price_; 
    double spot_;

    PnlVect *zc_; // zéro coupon de la place du sous jacent
    double zc_spot_; 
    
    // TODO: voir ce qu'on en fait de ces attributs
    PnlVect *ex_rate_; // taux de change entre le marché du sous jacent et le marché domestique
    PnlVect *rf_; // taux sans risque de la place du sous jacent

    IUnderlying(double spot, double zc_spot, int nbTimeSteps);
    ~IUnderlying();
};