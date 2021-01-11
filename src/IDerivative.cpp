#include "IDerivative.h"
#include <iostream>

IDerivative::IDerivative(double T, int nbTimeSteps, int size, double rd, IUnderlying **und){
    this->T_ = T;
    this->nbTimeSteps_ = nbTimeSteps;
    this->size_ = size;
    this->rd_ = rd;
    this->underlyings_ = und;
}

IDerivative::~IDerivative(){
    
}