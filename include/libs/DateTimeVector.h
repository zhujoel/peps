#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"
#include <vector>

std::vector<DateTime*> parseDatesFile(std::string fileName, int nbDates, char delimiter);
void calcul_indices_dates(std::vector<DateTime*> all_dates, std::vector<DateTime*> dates, PnlVectInt *indices); // LES DATES DOIVENT ETRE DANS L'ORDRE
std::vector<DateTime*> sameDates(std::vector<DateTime*> v1, std::vector<DateTime*> v2);
void getPricesFromDate(std::vector<DateTime*> allDates, std::vector<DateTime*> relevantDates, PnlVect *allPrices, PnlVect *result);
std::vector<DateTime*> fromDateToDate(std::vector<DateTime*> allDates, DateTime *from, DateTime *to);