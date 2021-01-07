#include "IModel.h"

IModel::IModel(int size, double rd, PnlMat *rho, PnlVect *spot){
    // TODO: rajouter le check pour vérifier que c'est positif. cela ce fonctionne plus car on a modifié 
    // l'attribut qui était un double en vecteur
    // if ( (-1./(size - 1) >= rho) || (rho >= 1) ) 
    // {
    //     throw invalid_argument("The correlation matrix should be a positive definite matrix!");
    // }

    this->size_ = size;
    this->rd_ = rd;
    this->rho_ = rho;
    this->spot_ = spot;
}

IModel::~IModel(){

}