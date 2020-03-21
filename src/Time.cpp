//
// Created by Antoine on 22/02/2020.
//

#include "Time.h"

// Retourne le temps acutel depuis l'UTC
long getCurrentTime(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}