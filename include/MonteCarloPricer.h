#pragma once

#include "IPricer.h"

class MonteCarloPricer : public IPricer{

    public:
        MonteCarloPricer();
        ~MonteCarloPricer();
        double price();
};