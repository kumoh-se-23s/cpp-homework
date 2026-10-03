#pragma once
#include "Day.h"

class DateCalculator {
    public:
        bool isLeapYear(int year);
        bool isValidDate(int year, int month, int day);
        Day totalDaysToDate(int days);
        int dateToTotalDays(int year, int month, int day);
        int getDaysInMonth(int year, int month);

    private:
        Day day;
        int commonYearDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int leapYearDay[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
};

