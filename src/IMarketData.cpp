#include "IMarketData.h"

IMarketData::IMarketData(DateTimeVector *dates){
    this->dates_ = dates;
}

//IUnderlying ** IMarketData::getMarketdata(int nbUnderlying){

//}

IMarketData::~IMarketData(){}

