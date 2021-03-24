#pragma once

#include <string>

/**
 * @brief Gère les dates.
 * 
 */
class DateTime{
    public:
        /**
         * @brief Gère les jours. Format DD (e.g. 18, 5).
         * 
         */
        int dd_;
        /**
         * @brief Gère les mois. Format MM (e.g 3, 10).
         * 
         */
        int mm_;
        /**
         * @brief Gère les années. Format YYYY (e.g. 1, 105, 1999).
         * 
         */
        int yyyy_;

        /**
         * @brief Construct a new DateTime object
         * 
         * @param dd_ 
         * @param mm_ 
         * @param yyyy_ 
         */
        DateTime(int dd_, int mm_, int yyyy_);
        /**
         * @brief Constructeur par copie profonde.
         * 
         * @param date DateTime à copier.
         */
        DateTime(const DateTime * const date);
        /**
         * @brief Destroy the DateTime object
         * 
         */
        ~DateTime();
        
        /**
         * @brief Créer une copie de l'instance de DateTime.
         * 
         * @return DateTime* Pointeur de la nouvelle DateTime.
         */
        DateTime* copy() const;
        /**
         * @brief Compare deux dates. 
         * 
         * @param dt Date avec laquelle comparer.
         * @return int Retourne -1 si this est avant dt. 0 si c'est la même date. 1 si this est après dt.
         */
        int compare(const DateTime * const dt) const;
        /**
         * @brief Converti DateTime en string.
         * 
         * @return std::string DateTime converti en string.
         */
        std::string str() const;

};

/**
 * @brief Redéfini l'opérateur << pour pouvoir print un DateTime facilement.
 * 
 * @param output Stream dans lequel écrire.
 * @param dt Date à écrire.
 * @return std::ostream& output.
 */
std::ostream &operator<<(std::ostream &output, const DateTime * const dt);
/**
 * @brief Parse un string sous format américain (e.g. YYYY-MM-DD) et renvoie un DateTime.
 * 
 * @param dateString String à parser.
 * @param delimiter Délimiteur entre les nombres. Dans l'exemple, c'est '-', mais n'importe quel autre délimiteur est utilisable.
 * @return DateTime* DateTime parsé.
 */
DateTime* parse_date_string(const std::string &dateString, char delimiter); // format is american (YYYY-MM-DD)
