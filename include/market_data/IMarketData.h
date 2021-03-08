#pragma once
#include "libs/DateTimeVector.h"
#include "pnl/pnl_matrix.h"

class IMarketData{
    public:
        std::string name_; // name of the data
        DateTime *startDate_; // date de début où récupérer les données
        DateTime *endDate_; // date de fin
        std::vector<DateTime*> dates_; // toutes les dates entre start_date_ et end_date_ ayant un prix
        PnlMat *path_; // tous les prix entre start_date_ et end_date_
        PnlMat *interest_path_; // taux d'intérets des devises utilisées
        
        IMarketData(const std::string &name, const DateTime * const startDate, const DateTime * const endDate);
        virtual ~IMarketData();

        virtual void set_data() = 0;
};