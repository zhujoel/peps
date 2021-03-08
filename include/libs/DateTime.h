#pragma once

#include <string>

class DateTime{
    public:
        int dd_; // number of day
        int mm_; // number of month
        int yyyy_; // number of year

        DateTime(int dd_, int mm_, int yyyy_);
        DateTime(const DateTime * const date);
        ~DateTime();
        
        DateTime* copy() const;
        int compare(const DateTime * const dt) const;
        std::string str() const;

};

std::ostream &operator<<(std::ostream &output, const DateTime * const dt);
DateTime* parse_date_string(const std::string &dateString, char delimiter); // format is american (YYYY-MM-DD)
