#pragma once
#include "Day.h"

class DDay {
    public:
        bool isLeapYear(int year) const;
        bool isValidDate(int year, int month, int day) const;
        Day totalDaysToDate(int totalDays) const;
        int dateToTotalDays(int year, int month, int day) const;
        int getDaysInMonth(int year, int month) const;

    private:
        int yearToTotalDays(int year) const;
        int monthToTotalDays(int year, int month) const;

        int totalDaysToYear(int& totalDays) const;
        int totalDaysToMonth(int year, int& totalDays) const;

        static constexpr int DAYS_IN_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        // static constexpr int leapYearDay[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
};

