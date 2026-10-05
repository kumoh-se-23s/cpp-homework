#include "DDay.h"

Day DDay::totalDaysToDate(int totalDays) {
    int year = 1, month = 1;

    year += totalDays / 146097 * 400;
    totalDays %= 146097;

    for (;totalDays - 36524 > 0; totalDays -= 36524, year += 100) { }

    year += totalDays / 1461 * 4;
    totalDays %= 1461;

    for (;totalDays - 365 > 0 ; totalDays -= 365, ++year) {}

    //윤년 계산(모듈러 연산이 딱 0으로 나누어 떨어지면 버그 방지 로직)
    if (totalDays == 0 && isLeapYear(year - 1)) {
        year -= 1;
        totalDays = 366;
        // 이게 더 나으려나
        // return Day(year - 1, 12, 31);
    }

    for (int i = 0; i < 12; i++) {
        month = i + 1;
        if (isLeapYear(year)) {
            if (totalDays - leapYearDay[i] <= 0) {
                break;
            }
            totalDays -= leapYearDay[i];
        }
        else {
            if (totalDays - commonYearDay[i] <= 0) {
                break;
            }
            totalDays -= commonYearDay[i];
        }
    }
    int day = totalDays;

    return Day(year, month, day);
}

bool DDay::isLeapYear(int year) {
    return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

int DDay::dateToTotalDays(int year, int month, int day) {
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

bool DDay::isValidDate(int year, int month, int day) {
    return year > 0 && month > 0 && month < 13 && day <= getDaysInMonth(year, month) && day > 0;
}

int DDay::getDaysInMonth(int year, int month) {
    if (isLeapYear(year)) {
        return leapYearDay[month - 1];
    }
    return commonYearDay[month - 1];
}
