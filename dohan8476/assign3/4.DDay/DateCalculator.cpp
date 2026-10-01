#include "DateCalculator.h"

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

Day DateCalculator::totalDaysToDate(int days) {
    int year = 1, month = 0;

    --days;

    year += days / 146097 * 400;
    days %= 146097;

    for (;days - 36524 > 0; days -= 36524, year += 100)

    year += days / 1461 * 4;
    days %= 1461;

    for (;days - 365 > 0; days -= 365, ++year)

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

bool DateCalculator::isValidDate(int year, int month, int day) {
    return year > 0 && month > 0 && month < 13 && day < getDaysInMonth(year, month);
}

int DateCalculator::getDaysInMonth(int year, int month) {
    if (isLeapYear(year)) {
        return leapYearDay[month - 1];
    }
    return commonYearDay[month - 1];
}
