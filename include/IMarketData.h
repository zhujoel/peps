#pragma once
#include "DateTimeVector.h"

class IMarketData{
    public:
        char* name_; // name of the data
        DateTime *startDate_; // date de début où récupérer les données
        DateTime *endDate_; // date de fin
        DateTimeVector *dates_; // toutes les dates entre start_date_ et end_date_ ayant un prix
        PnlVect *path_; // tous les prix entre start_date_ et end_date_
        IMarketData(char* name, DateTime *startDate, DateTime *endDate);

        virtual ~IMarketData() = 0;
};