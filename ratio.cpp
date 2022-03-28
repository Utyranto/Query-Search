#include "ratio.h"

Ratio::Ratio(earnings* earnings, int length){
    this->earn = earnings;
    this->length = length;
}

void Ratio::printRatios(int yearStart, int yearEnd){
    for(int i = yearStart; i <= yearEnd; i++){
        earnings earnYear = earn[2019 - i];
        double ratio = ((double) earnYear.female_earnings) / ((double) earnYear.male_earnings);
        double percent = (ratio * 100) - 0.05;
        // << std::setprecision(2) << std::fixed 
        std::cout << "\t" << i << ": " << std::setprecision(1) << std::fixed << percent << "%" << std::endl;
    }
}
