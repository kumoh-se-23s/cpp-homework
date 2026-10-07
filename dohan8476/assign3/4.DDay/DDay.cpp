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

//해당 월의 일수 구하는 메소드
int DDay::getDaysInMonth(int year, int month) {
    if (isLeapYear(year) && month == 2) {
        return 29;
    }
    return DAYS_IN_MONTH[--month];
}

//토탈 데이 -> 연
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

//토탈데이 -> 월
int DDay::totalDaysToMonth(int year, int &totalDays) {
    int month = 0;

    for (int i = 1; i < 13; i++) {
        int daysInMonth = getDaysInMonth(year, i);

        if (totalDays <= daysInMonth) {
            break;
        }
        totalDays -= daysInMonth;
        month++;
    }

    return month;
}


// 연 -> 토탈데이
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

// 월 -> 토탈데이
int DDay::monthToTotalDays(int year, int month) {
    int totalDays = 0;

    for (int i = 1 ; i < month + 1 ; i++) {
        totalDays += getDaysInMonth(year, i);
    }

    return totalDays;
}


