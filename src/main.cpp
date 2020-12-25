#include <iostream>
#include "MonteCarloPricer.h"
#include "pnl/pnl_vector.h"

int main(){
    PnlVect* v = pnl_vect_create(3);
    pnl_vect_print(v);
    MonteCarloPricer* mc = new MonteCarloPricer();
    std::cout << mc->price() << std::endl;
}