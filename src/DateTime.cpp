#include "DateTime.h"
#include <iostream>
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

void fill_dates_from_file(DateTime **dates, std::string fileName, int nbDates) {
    std::ifstream inputFileStream(fileName);
    std::string jour, mois, annee;
    std::string line;
    for (int i = 0; i < nbDates; i++)
    {
        getline(inputFileStream, line);
        std::istringstream lineStream(line);
        getline(lineStream, jour, ',');
        getline(lineStream, mois, ',');
        getline(lineStream, annee, ',');
        dates[i] = new DateTime(std::stoi(jour), std::stoi(mois), std::stoi(annee));
    }
}


// void calcul_indices_dates(DateTime **all_dates, DateTime **dates, PnlVectInt *indices)
// {
//     int cnt = 0;
//     for(int i = 0; i < indices->length; ++i){
//         // TODO: à tester (peut y avoir +1 ou -1 dans les indices)
//         if(all_dates[i] == dates[cnt]){
//             LET_INT(indices, cnt++) = i;

//             if(cnt == indices->size) break;
//         }
//     }
// }