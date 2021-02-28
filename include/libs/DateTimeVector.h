#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"
#include <vector>
#include "pnl/pnl_matrix.h"

std::vector<DateTime*> parse_dates_file(std::string fileName, int nbDates, char delimiter);
PnlVectInt* calcul_indices_dates(std::vector<DateTime*> all_dates, std::vector<DateTime*> dates); // LES DATES DOIVENT ETRE DANS L'ORDRE
int get_indice_from_date(std::vector<DateTime*> all_dates, DateTime* date);
std::vector<DateTime*> same_dates(std::vector<DateTime*> v1, std::vector<DateTime*> v2);
PnlVect* get_prices_from_date(std::vector<DateTime*> allDates, std::vector<DateTime*> subset, PnlVect *allPrices);
PnlMat* get_path_from_dates(std::vector<DateTime*> allDates, std::vector<DateTime*> subset, PnlMat *path);
std::vector<DateTime*> from_date_to_date(std::vector<DateTime*> allDates, DateTime *from, DateTime *to);
void delete_date_vector(std::vector<DateTime*> list);