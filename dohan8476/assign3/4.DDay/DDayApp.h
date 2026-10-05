#pragma once

#include "Day.h"
#include "DDay.h"

class DDayApp {
    public:
        DDayApp();
        void run();

    private:
        DDay calc;
        Day currentDate;
        int dDayValue = 0;

        void printCurrent();
        void printMenu();
        void printError();
};
