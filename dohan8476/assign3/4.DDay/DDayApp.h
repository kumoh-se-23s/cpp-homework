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

        bool processCommand(const char input[], int length);
        void handleSetDDay(const char input[], int length);
        void handleMoveDate(const char input[], int length);

        void printCurrent();
        void printMenu();
        void printError();

        bool isDigit(char input);
};
