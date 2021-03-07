#include "derivatives/IDerivative.h"

IDerivative::IDerivative(double T, int size){
    this->T_ = T;
    this->size_ = size;
}

IDerivative::~IDerivative(){
    
}