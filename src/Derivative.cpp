#include "Derivative.h"

Derivative::~Derivative(){
    
}

Derivative::Derivative(double T, int nbTimeSteps){
    this->T_ = T;
    this->nbTimeSteps_ = nbTimeSteps;
}
