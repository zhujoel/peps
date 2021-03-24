#pragma once

#include "market_data/IMarketData.h"

/**
 * @brief Récupère des données de marché historique.
 */
class HistoricalMarketData : public IMarketData{
    public:
        /**
         * @brief Prix du produit dérivé entre startDate_ et endDate_.
         */
        PnlMat *derivative_path_;

        /**
         * @brief Construct a new IMarketData object
         * 
         * @param name Nom des données de marché (e.g. Océlia)
         * @param startDate Date de début des données.
         * @param endDate Date de fin des données.
         */
        HistoricalMarketData(const std::string &name, const DateTime * const startDate, const DateTime * const endDate);
        /**
         * @brief Destroy the Historical Market Data object
         */
        ~HistoricalMarketData();
        
        void set_data();
        /**
         * @brief Récupère les données de marché liées à Océlia.
         */
        void set_Ocelia_data();
};