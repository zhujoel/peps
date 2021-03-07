#include "models/IModel.h"

IModel::IModel(int size, int nbTimeSteps, double rd){
    this->size_ = size;
    this->rd_ = rd;
    this->nbTimeSteps_ = nbTimeSteps;
}

IModel::~IModel(){
}