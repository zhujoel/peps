#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"
#include <vector>

std::vector<DateTime*> parseDatesFile(std::string fileName, int nbDates, char delimiter);
PnlVectInt* calcul_indices_dates(std::vector<DateTime*> all_dates, std::vector<DateTime*> dates); // LES DATES DOIVENT ETRE DANS L'ORDRE
std::vector<DateTime*> sameDates(std::vector<DateTime*> v1, std::vector<DateTime*> v2);
PnlVect* getPricesFromDate(std::vector<DateTime*> allDates, std::vector<DateTime*> subset, PnlVect *allPrices);
std::vector<DateTime*> fromDateToDate(std::vector<DateTime*> allDates, DateTime *from, DateTime *to);
void delete_date_vector(std::vector<DateTime*> list);