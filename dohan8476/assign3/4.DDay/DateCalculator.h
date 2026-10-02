#pragma once
#include "Day.h"

class DateCalculator {
    public:
        static bool isLeapYear(int year);
        static bool isValidDate(int year, int month, int day);
        Day totalDaysToDate(int days);
        int dateToTotalDays(int year, int month, int day);
        static int getDaysInMonth(int year, int month);

    private:
        Day day;
        constexpr static int commonYearDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        constexpr static int leapYearDay[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
};

