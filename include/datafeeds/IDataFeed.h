#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_matrix.h"
#include <string>
#include <vector>

/**
 * @brief Parse et récupère les données à partir d'un fichier
 */
class IDataFeed{
    public:
        /**
         * @brief Nom du fichier à parser.
         */
        std::string filepath_;
        /**
         * @brief Dates ayant un prix contenues dans le fichier.
         */
        std::vector<DateTime*> dates_;
        /**
         * @brief Prix contenus dans le fichier.
         */
        PnlMat *prices_;
        /**
         * @brief Les indices des colonnes dont on souhaite récupérer les données.
         */
        PnlVectInt *relevant_columns_;
        /**
         * @brief Indice de la colonne contenant les dates.
         */
        unsigned int date_column_;
        /**
         * @brief Nombre de colonnes totaux du fichier.
         */
        unsigned int nb_columns_;

        /**
         * @brief Construct a new IDataFeed object
         * 
         * @param relevant_columns 
         * @param date_column 
         * @param nb_columns 
         * @param filepath 
         */
        IDataFeed(PnlVectInt * const relevant_columns, unsigned int date_column, unsigned int nb_columns, const std::string &filepath);
        /**
         * @brief Destroy the IDataFeed object
         */
        virtual ~IDataFeed();
        
        /**
         * @brief Détermine le nombre de donneés valides (i.e. prix non-null) du fichier.
         * 
         * @return int Le nombre de données valides.
         */
        virtual int get_number_valid_data() const = 0;
        /**
         * @brief Set the data object
         */
        virtual void set_data() = 0;
};