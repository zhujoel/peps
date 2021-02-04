#pragma once

#include <string>

class DateTime{
    public:
        int dd_; // number of day
        int mm_; // number of month
        int yyyy_; // number of year

        DateTime(int dd_, int mm_, int yyyy_);
        ~DateTime();
        
        int compare(const DateTime *dt) const;

};

std::ostream &operator<<(std::ostream &output, const DateTime *dt);
DateTime* parseDateString(std::string dateString, char delimiter); // format is american (YYYY-MM-DD)
