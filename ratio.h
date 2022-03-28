#include <string>
#ifndef DEFNS
#define DEFNS
#include "defns.h"
#endif
#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>

class Ratio {
    private:
        earnings* earn;
        int length;
    public:
        Ratio(earnings* earnings, int length); //constructor

        void printRatios(int yearStart, int yearEnd);

        ~Ratio(); //destructor
};