#include "models/IModel.h"

IModel::IModel(int size, double rd){
    this->size_ = size;
    this->rd_ = rd;
}

IModel::~IModel(){
}