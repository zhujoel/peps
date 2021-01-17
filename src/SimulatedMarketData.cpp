#include "SimulatedMarketData.h"
#include "ForeignUnderlying.h"
SimulatedMarketData::SimulatedMarketData(DateTime ** listeDate, int nombreDate):IMarketData(listeDate, nombreDate){}

SimulatedMarketData::~SimulatedMarketData(){}
IUnderlying ** SimulatedMarketData::getMarketdata(int nbUnderlying){
    IUnderlying **underlyings = new IUnderlying*[nbUnderlying];
    for(int i=0; i<nbUnderlying; i++){
        underlyings[i]= new ForeignUnderlying(100, 10, this->nombreDate_);
    
    }
return underlyings;
}