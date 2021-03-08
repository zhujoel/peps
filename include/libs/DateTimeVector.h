#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"
#include <vector>
#include "pnl/pnl_matrix.h"

void parse_dates_file(std::vector<DateTime*> &dates, const std::string &fileName, int nbDates, char delimiter);
void calcul_indices_dates(PnlVectInt * const indices, const std::vector<DateTime*> &all_dates, const std::vector<DateTime*> &dates_subset); // LES DATES DOIVENT ETRE DANS L'ORDRE
int get_indice_from_date(const std::vector<DateTime*> &all_dates, const DateTime * const date);
std::vector<DateTime*> same_dates(const std::vector<DateTime*> &v1, const std::vector<DateTime*> &v2);
void get_prices_from_date(PnlVect * const prices, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlVect * const allPrices);
void get_prices_from_date(PnlMat * const prices, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlMat * const allPrices);
void get_subset_path_from_dates(PnlMat * const subset_path, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlMat * const path);
void from_date_to_date(std::vector<DateTime*> &from_to, const std::vector<DateTime*> &allDates, const DateTime * const from, const DateTime * const to);
void delete_date_vector(std::vector<DateTime*> &list);