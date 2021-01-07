#include "Derivative.h"

Derivative::~Derivative(){
    
}

Derivative::Derivative(double T, int nbTimeSteps, int size){
    this->T_ = T;
    this->nbTimeSteps_ = nbTimeSteps;
    this->size_ = size;
}
