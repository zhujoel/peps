#include "libs/DateTime.h"
#include "libs/Utilities.h"
#include <iostream>

DateTime::DateTime(int dd, int mm, int yyyy)
{
    this->dd_ = dd;
    this->mm_ = mm;
    this->yyyy_ = yyyy;
}

DateTime::~DateTime(){

}

int DateTime::compare(const DateTime *dt) const{
    if(this->yyyy_ < dt->yyyy_){
        return -1;
    }
    else if(this->yyyy_ > dt->yyyy_){
        return 1;
    }
    else{
        if(this->mm_ < dt->mm_){
            return -1;
        }
        else if(this->mm_ > dt->mm_){
            return 1;
        }
        else{
            if(this->dd_ < dt->dd_){
                return -1;
            }
            else if(this->dd_ > dt->dd_){
                return 1;
            }
        }
    }
    return 0;
}

std::ostream &operator<<(std::ostream &output, const DateTime *dt){
    output << dt->dd_ << "/" << dt->mm_ << "/" << dt->yyyy_;
    return output;
}


DateTime* parseDateString(std::string dateString, char delimiter){
    std::string dateParsed[3];
    split(dateString, delimiter, dateParsed);
    return new DateTime(std::stoi(dateParsed[2].c_str()), std::stoi(dateParsed[1].c_str()), std::stoi(dateParsed[0].c_str())); // c_str(): needs a char* to use atof
}