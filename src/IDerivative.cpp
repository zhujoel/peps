#include "IDerivative.h"

IDerivative::IDerivative(double T, int nbTimeSteps, int size){
    this->T_ = T;
    this->nbTimeSteps_ = nbTimeSteps;
    this->size_ = size;
}

IDerivative::~IDerivative(){
    
}