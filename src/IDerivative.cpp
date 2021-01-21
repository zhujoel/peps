#include "IDerivative.h"
#include <iostream>

IDerivative::IDerivative(double T, int nbTimeSteps, int size, double rd, IUnderlying **und){
    this->T_ = T;
    this->nbTimeSteps_ = nbTimeSteps;
    this->size_ = size;
    this->rd_ = rd;
    this->underlyings_ = und;

    for(int i = 0; i < this->size_; ++i){
        if(this->underlyings_[i]->nbTimeSteps_ != this->nbTimeSteps_){
            throw std::invalid_argument("Les sous-jacents et le produit dérivé doivent avoir le nbTimeSteps !");
        }
    }

    // TODO: mettre une vérif pour que le size soit le même que le nombre de sous jacents
}

IDerivative::~IDerivative(){
}