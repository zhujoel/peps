#include "IMarketData.h"

IMarketData::IMarketData(DateTime ** listeDate, int nombreDate){
    this->listeDate_ = listeDate;
    this->nombredate_ = nombreDate;

}

IMarketData::~IMarketData(){}

