#include "IModel.h"

IModel::IModel(int size, double rd, PnlMat *sigma, PnlVect *spot){
    this->size_ = size;
    this->rd_ = rd;
    this->sigma_ = sigma;
    this->spot_ = spot;
}

IModel::~IModel(){

}