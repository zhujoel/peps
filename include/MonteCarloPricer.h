#ifndef MONTECARLOPRICER_H
#define MONTECARLOPRICER_H

#include "IPricer.h"

class MonteCarloPricer : public IPricer{

    public:
        MonteCarloPricer();
        ~MonteCarloPricer();
        double price();
};

#endif