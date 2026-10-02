#include "DateCalculator.h"

#include <iomanip>
#include <ostream>

namespace Calendar {
Day DateCalculator::totalDaysToDate(int days) {
    int year = 1, month = 0;

    --days;

    year += days / 146097 * 400;
    days %= 146097;

    for (int i = 0 ; i < 3 && days - 36524 > 0; i++) {
        days -= 36524;
        year += 100;
    }

    year += days / 1461 * 4;
    days %= 1461;

    for (int i = 0 ; i < 3 && days - 365 > 0; i++) {
        days -= 365;
        ++year;
    }

    for (int i = 0; i < 12; i++) {
        month = i + 1;
        if (isLeapYear(year)) {
            if (days - leapYearDay[i] <= 0) {
                break;
            }
            days -= leapYearDay[i];
        }
        else {
            if (days - commonYearDay[i] <= 0) {
                break;
            }
            days -= commonYearDay[i];
        }
    }
    int day = days;

    return Day(year, month, day);
}

bool DateCalculator::isLeapYear(int year) {
    return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

int DateCalculator::dateToTotalDays(int year, int month, int day) {
    int totalDays = 0;

    totalDays += day;

    --month;
    if (isLeapYear(year)) {
        for (int i = 0; i < month; i++) {
            totalDays += leapYearDay[i];
        }
    }
    else {
        for (int i = 0; i < month; i++) {
            totalDays += commonYearDay[i];
        }
    }

    --year;
    totalDays += (year / 400) * 146097;
    year %= 400;

    totalDays += (year / 100) * 36524;
    year %= 100;

    totalDays += (year / 4) * 1461;
    year %= 4;

    totalDays += year * 365;

    return totalDays;
}

bool DateCalculator::isValidDate(int year, int month, int day) {
    return year > 0 && month > 0 && month < 13 && day < getDaysInMonth(year, month);
}

int DateCalculator::getDaysInMonth(int year, int month) {
    if (isLeapYear(year)) {
        return leapYearDay[month - 1];
    }
    return commonYearDay[month - 1];
}

Day& operator++(Day& tomorrow) {
    DateCalculator calc;

    int year = tomorrow.getYear();
    int month = tomorrow.getMonth();
    int day = tomorrow.getDay() + 1;

    if (day > calc.getDaysInMonth(year, month)) {
        day = 1;
        ++month;

        if (month > 12) {
            month = 1;
            ++year;
        }
    }

    tomorrow.setYear(year);
    tomorrow.setMonth(month);
    tomorrow.setDay(day);

    return tomorrow;
}
Day& operator--(Day& yesterday) {
    DateCalculator calc;

    int year = yesterday.getYear();
    int month = yesterday.getMonth();
    int day = yesterday.getDay() - 1;

    if (day == 0) {
        --month;

        if (month == 0) {
            month = 12;
            --year;
        }
        //마지막 날로 세팅
        day = calc.getDaysInMonth(year, month);
    }

    yesterday.setYear(year);
    yesterday.setMonth(month);
    yesterday.setDay(day);

    return yesterday;
}

Day operator+(const Day& day, int addDays){
    DateCalculator calc;
    int totalDays = calc.dateToTotalDays(day.getYear(), day.getMonth(), day.getDay());

    return calc.totalDaysToDate(totalDays + addDays);
}

Day operator-(const Day& day, int subDays){
    DateCalculator calc;
    int totalDays = calc.dateToTotalDays(day.getYear(), day.getMonth(), day.getDay());

    return calc.totalDaysToDate(totalDays - subDays);
}

}
