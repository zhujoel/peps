#pragma once
#include "libs/DateTimeVector.h"
#include "pnl/pnl_matrix.h"

/**
 * @brief Gère des données de marché.
 */
class IMarketData{
    public:
        /**
         * @brief Nom des données de marché.
         */
        std::string name_;
        /**
         * @brief Date de début où récupérer les données.
         */
        DateTime *startDate_; 
        /**
         * @brief Date de fin où arrêter de récupérer les données.
         * 
         */
        DateTime *endDate_;
        /**
         * @brief Toutes les dates entre startDate_ et endDate_ ayant un prix.
         */
        std::vector<DateTime*> dates_;
        /**
         * @brief Tous les prix de tous les produits des données de marché pour name_ entre startDate_ et endDate_.
         */
        PnlMat *path_;
        /**
         * @brief Taux d'intérêts des devises utilisées (si étrangers) entre startDate_ et endDate_.
         */
        PnlMat *interest_path_; 
        
        /**
         * @brief Construct a new IMarketData object
         * 
         * @param name Nom des données de marché (e.g. Océlia)
         * @param startDate Date de début des données.
         * @param endDate Date de fin des données.
         */
        IMarketData(const std::string &name, const DateTime * const startDate, const DateTime * const endDate);
        /**
         * @brief Destroy the IMarketData object
         */
        virtual ~IMarketData();

        /**
         * @brief Récupère les données de marché correspondant à name_.
         */
        virtual void set_data() = 0;
};