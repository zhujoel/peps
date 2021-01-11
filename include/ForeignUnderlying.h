#pragma once

#include "IUnderlying.h"

class ForeignUnderlying : public IUnderlying {
  public:
    ForeignUnderlying(double und_spot, double zc_spot, int nbTimeSteps);
    ~ForeignUnderlying();
};