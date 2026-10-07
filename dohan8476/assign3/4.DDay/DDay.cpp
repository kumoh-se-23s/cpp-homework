#include "DDay.h"

Day DDay::totalDaysToDate(int totalDays) {
    int year = totalDaysToYear(totalDays);
    //윤년(모듈러 == 0)이면 윤년의 마지막날임
    // if (totalDays == 0 && isLeapYear(year - 1)) {
    if (!totalDays && isLeapYear(year - 1) ){
        return Day(year - 1, 12, 31);
    }
    int month = totalDaysToMonth(year, totalDays);
    int day = totalDays;

    return Day(year, month, day);
}

int DDay::dateToTotalDays(int year, int month, int day) {
    return day + monthToTotalDays(year, month) + yearToTotalDays(year);
}

bool DDay::isLeapYear(int year) {
    return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

bool DDay::isValidDate(int year, int month, int day) {
    return year > 0 && month > 0 && month < 13 && day <= getDaysInMonth(year, month) && day > 0;
}

int DDay::getDaysInMonth(int year, int month) {
    return isLeapYear(year) ? leapYearDay[--month] : commonYearDay[--month];
}

int DDay::totalDaysToYear(int &totalDays) {
    int year = 1;

    year += totalDays / 146097 * 400;
    totalDays %= 146097;

    for (;totalDays - 36524 > 0; totalDays -= 36524, year += 100) { }

    year += totalDays / 1461 * 4;
    totalDays %= 1461;

    for (;totalDays - 365 > 0 ; totalDays -= 365, ++year) {}

    return year;
}

int DDay::totalDaysToMonth(int year, int &totalDays) {
    int month = 1;

    for (int i = 0; i < 12; i++) {
        int daysInMonth = isLeapYear(year) ? leapYearDay[i] : commonYearDay[i];

        if (totalDays <= daysInMonth) {
            break;
        }
        totalDays -= daysInMonth;
        month++;
    }

    return month;
}

int DDay::yearToTotalDays(int year) {
    int totalDays = 0;

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

int DDay::monthToTotalDays(int year, int month) {
    int totalDays = 0;
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
    return totalDays;
}


