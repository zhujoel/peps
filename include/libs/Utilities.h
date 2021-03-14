#pragma once

#include <string>
#include "pnl/pnl_vector.h"

void split(std::string * const output, const std::string &line, char delimiter);
void trunc(PnlVect * const vect, int n); // arrondir à n decimals
bool are_all_positive(const PnlVect * const vect); // détermine si tous les performances sont positives
bool has_no_index_value_null(const std::string * const array, const PnlVectInt * const indexes);
int first_index_gte(const PnlVect * const vect, double value);
std::ostream& operator<<(std::ostream &stm, const PnlVect * const vect);