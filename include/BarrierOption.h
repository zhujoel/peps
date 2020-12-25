#pragma once

#include "IDerivative.h"

class BarrierOption : public IDerivative{

    public:
        BarrierOption();
        ~BarrierOption();
};