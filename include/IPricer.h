#ifndef IPRICER_H
#define IPRICER_H

#pragma once

class IPricer{

    public:
        virtual double price() = 0;
};

#endif