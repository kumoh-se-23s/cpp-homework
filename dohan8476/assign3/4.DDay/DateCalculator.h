#pragma once
#include "Day.h"

namespace Calendar {

    class DateCalculator {
    public:
        bool isLeapYear(int year);
        bool isValidDate(int year, int month, int day);
        Day totalDaysToDate(int days);
        int dateToTotalDays(int year, int month, int day);
        int getDaysInMonth(int year, int month);

    private:
        int commonYearDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int leapYearDay[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    };

    //Day 내부에서 정의가 아니니 모든 인자를 명시적으로 받아야함
    Day operator+(const Day& day, int addDays);
    Day operator-(const Day& day, int subDays);
    //원본을 터치해야하니 Day&
    Day& operator++(Day& day);
    Day& operator--(Day& day);
}
