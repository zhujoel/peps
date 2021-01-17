#include "IMarketData.h"

IMarketData::IMarketData(DateTime ** listeDate, int nombreDate){
    this->listeDate_ = listeDate;
    this->nombreDate_ = nombreDate;

}

//IUnderlying ** IMarketData::getMarketdata(int nbUnderlying){

//}

IMarketData::~IMarketData(){}

