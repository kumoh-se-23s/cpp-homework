#pragma once

#include "Day.h"
#include "DateCalculator.h"

class DDay {
    public:
        DDay();
        void run();

    private:
        Day currentDate;
        int dDayValue = 0;
        DateCalculator calc;

        void printCurrent();
        void printMenu();
        void printError();
};
