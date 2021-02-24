#pragma once

#include <string>
#include "pnl/pnl_vector.h"

void split(std::string line, char delimiter, std::string *output);
void trunc(PnlVect *vect, int n); // arrondir à n decimals
bool are_all_positive(PnlVect *vect); // détermine si tous les performances sont positives