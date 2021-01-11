#include "IDerivative.h"

IDerivative::IDerivative(double T, int nbTimeSteps, int size, IUnderlying **und){
    this->T_ = T;
    this->nbTimeSteps_ = nbTimeSteps;
    this->size_ = size;
    this->underlyings_ = und;
}

IDerivative::~IDerivative(){
    
}