#include "DateTime.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

DateTime::DateTime(int dd, int mm, int yyyy)
{
    // TODO: faire les vérifs
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