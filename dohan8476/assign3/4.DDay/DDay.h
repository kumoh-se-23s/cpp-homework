#pragma once
#include "Day.h"

class DDay {
    public:
        bool isLeapYear(int year);
        bool isValidDate(int year, int month, int day);
        Day totalDaysToDate(int totalDays);
        int dateToTotalDays(int year, int month, int day);
        int getDaysInMonth(int year, int month);

    private:
        int yearToTotalDays(int year);
        int monthToTotalDays(int year, int month);
        int totalDaysToYear(int& totalDays);
        int totalDaysToMonth(int year, int& totalDays);

        static constexpr int DAYS_IN_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        // static constexpr int leapYearDay[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
};

