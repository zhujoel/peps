#pragma once

#include "pnl/pnl_matrix.h"
#include "libs/DateTime.h"
#include <vector>

/**
 * @brief Gère les taux d'intérêt.
 * 
 */
class InterestRate{
    public:
        /**
         * @brief Toutes les dates avec un taux d'intérêt.
         * 
         */
        std::vector<DateTime*> all_dates_;
        /**
         * @brief Les taux d'intérêts.
         * 
         */
        PnlMat *interest_path_;
        /**
         * @brief Taux d'intérêt à une date t.
         * 
         */
        PnlVect *rates_;

        /**
         * @brief Construct a new Interest Rate object
         * 
         * @param current_date Date à laquelle on souhaite initialiser rates_.
         * @param all_dates 
         * @param interest_path 
         */
        InterestRate(DateTime * const current_date, const std::vector<DateTime*> &all_dates, PnlMat * const interest_path);
        /**
         * @brief Destroy the Interest Rate object
         * 
         */
        ~InterestRate();

        /**
         * @brief Récupère le taux domestique à la date t.
         * 
         * @return double Le taux domestique.
         */
        double get_domestic_rate();
        /**
         * @brief Récupère le taux étranger à la date t pour un sous-jacent.
         * 
         * @param index Indice du sous-jacent qu'on souhaite récupérer la valeur.
         * @return double La valeur du sous-jacent.
         */
        double get_foreign_rate(int index);
        /**
         * @brief Récupère le taux domestique à la date t capitalisé. 
         * t0 et t1 utilisées pour déterminer le t à laquelle capitaliser.
         * 
         * @param t0 t0 < t1.
         * @param t1 
         * @return double Le taux domestique capitalisé.
         */
        double compute_domestic_risk_free_asset(double t0, double t1);
        /**
         * @brief Récupère le taux étranger à la date t capitalisé.
         * t0 et t1 utilisées pour déterminer le t à laquelle capitaliser.
         *
         * @param t0 t0 < t1.
         * @param t1 
         * @param index Indice du sous-jacent qu'on souhaite récupérer la valeur.
         * @return double La valeur du sous-jacent capitalisé.
         */
        double compute_foreign_risk_free_asset(double t0, double t1, int index);
};