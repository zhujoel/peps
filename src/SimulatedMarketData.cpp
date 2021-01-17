#include "SimulatedMarketData.h"

SimulatedMarketData::SimulatedMarketData(DateTime ** listeDate, int nombreDate):IMarketData(DateTime ** listeDate, int nombreDate){}

SimulatedMarketData::~SimulatedMarketData(){}
IUnderlying ** SimulatedMarketData::getMarketdata(int nbUnderlying){
    IUnderlying **underlyings = new IUnderlying*[nbUnderlying];
    for(int i=0; i<nbUnderlying; i++){
        underlyings[i]= (Iunderlying*)new ForeignUnderlying(100, 10, nombreDate);
    
    }
return underlyings;
}