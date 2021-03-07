#pragma once

#include <string>
#include "pnl/pnl_vector.h"

void split(std::string *output, const std::string &line, char delimiter);
void trunc(PnlVect *vect, int n); // arrondir à n decimals
bool are_all_positive(const PnlVect *vect); // détermine si tous les performances sont positives
bool has_no_index_value_null(const std::string *array, const PnlVectInt *indexes);