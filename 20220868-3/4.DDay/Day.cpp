#include "Day.h"
#include <cassert>

Day::Day(const int year, const int month, const int day) : year(year), month(month), day(day) {
    normalizeIfNeeded();
};

Day Day::operator++() {
    if (day == getDays(year, month)) {
        if (month == 12) {
            ++year;
            month = 1;
            day = 1;
        } else {
            ++month;
            day = 1;
        }
    } else {
        ++day;
    }
    return *this;
}


Day Day::operator--() {
    if (day == 1) {
        if (month == 1) {
            --year;
            month = 12;
            day = 31;
        } else {
            day = getDays(year, --month);
        }
    } else {
        --day;
    }
    return *this;
}

Day Day::operator+(const int d) const {
    return Day(year, month, day + d);
}

Day Day::operator-(const int d) const {
    return Day(year, month, day - d);
}

int Day::getDays(const int year, const int month) {
    return (isLeap(year) & (month == 2)) + (MONTH_DAYS_SUM[month] - MONTH_DAYS_SUM[month - 1]);
}

bool Day::isLeap(const int year) {
    return (year % 400 == 0) | (year % 4 == 0) & (year % 100 != 0);
}

void Day::normalizeIfNeeded() {
    if (isValid(year, month, day)) return;
    normalize();
}

void Day::normalize() {
    // O(1) normalization
    year += (month - 12) / 12;
    month = ((month - 1) % 12 + 12) % 12 + 1;
    day += (isLeap(year) & (month >= 3)) + MONTH_DAYS_SUM[month - 1];

    // minimum date : 0001/01/01
    // year days = year * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400
    // additional days : day - 1 (1~365 => 0~364 mapping, applying day-1)

    // n/100 : n * 2^32/100 >> 32 = 42949673*n >> 32
    const int totalDays = std::max(365, year * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 + day - 1);

    // 366 or 365, unknown => execute both 365 and 366

    //146000 + 97 (24 * 4 = 96, 400 is leap year. 96 + 1 = 97)
    //36500 + 24 (100 / 4 = 25, 100 is not leap year. 25 - 1 = 24)
    //1460 + 1(leaps)
    int yearCalcDays = totalDays - 365;
    yearCalcDays -= yearCalcDays / 146097;
    yearCalcDays += yearCalcDays / 36524;
    yearCalcDays -= yearCalcDays / 1461;

    int leapYearCalcDays = totalDays - 366; //re-calculation for leap years
    leapYearCalcDays -= leapYearCalcDays / 146097;
    leapYearCalcDays += leapYearCalcDays / 36524;
    leapYearCalcDays -= leapYearCalcDays / 1461;

    const int year2 = leapYearCalcDays / 365 + 1;
    year = yearCalcDays / 365 + 1;
    year = (yearCalcDays - (isLeap(year2) & (year != year2))) / 365 + 1; //solve 366

    // (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400
    const int totalDaysForYear = year * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
    const int remainDays = totalDays - totalDaysForYear + 1;
    const bool leap = isLeap(year);

    // approximate month
    const int monthApprox = (remainDays >> 5) + 1;
    month = monthApprox + 1 - (remainDays <= (leap & (monthApprox >= 2)) + MONTH_DAYS_SUM[monthApprox]);
    day = remainDays - (leap & (month >= 3)) - MONTH_DAYS_SUM[month - 1];
}

bool Day::isValid(const int y, const int m, const int d) {
    return y >= 1 && m >= 1 && m <= 12 && d >= 1 && d <= getDays(y, m);
}
