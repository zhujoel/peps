#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"
#include <vector>
#include "pnl/pnl_matrix.h"

void parse_dates_file(std::vector<DateTime*> &dates, const std::string &fileName, int nbDates, char delimiter);
void calcul_indices_dates(PnlVectInt *indices, const std::vector<DateTime*> &all_dates, const std::vector<DateTime*> &dates_subset); // LES DATES DOIVENT ETRE DANS L'ORDRE
int get_indice_from_date(const std::vector<DateTime*> &all_dates, const DateTime *date);
void same_dates(std::vector<DateTime*> &same_dates, const std::vector<DateTime*> &v1, const std::vector<DateTime*> &v2);
void get_prices_from_date(PnlVect *prices, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlVect *allPrices);
void get_subset_path_from_dates(PnlMat *subset_path, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlMat *path);
void from_date_to_date(std::vector<DateTime*> &from_to, const std::vector<DateTime*> &allDates, const DateTime *from, const DateTime *to);
void delete_date_vector(std::vector<DateTime*> &list);