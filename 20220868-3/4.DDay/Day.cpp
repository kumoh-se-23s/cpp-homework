#include "Day.h"
#include <cassert>

Day::Day(const int year, const int month, const int day) : year(year), month(month), day(day) {
    normalizeIfNeeded();
};

Day Day::operator++() {
    if (day == getMonthDays(year, month)) {
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
            day = getMonthDays(year, --month);
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

int Day::getMonthDays(const int year, const int month) {
    return (isLeap(year) && (month == 2)) + (MONTH_DAYS_SUM[month] - MONTH_DAYS_SUM[month - 1]);
}


int Day::getTotalDaysFromThisYear(const int y, const int m) {
    return getTotalDaysFromThisYear(isLeap(y), m);
}

int Day::getTotalDaysFromThisYear(const bool leap, const int m) {
    return (leap & (m >= 3)) + MONTH_DAYS_SUM[m - 1];
}


bool Day::isLeap(const int year) {
    return (year % 400 == 0) || (year % 4 == 0) && (year % 100 != 0);
}

void Day::normalizeIfNeeded() {
    if (isValid(year, month, day)) return;
    normalize();
}

int Day::calcTotalDaysFromYear(const int year) {
    // year days = year * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400
    const int ym1 = year - 1;
    return year * 365 + ym1 / 4 - ym1 / 100 + ym1 / 400;
}

void Day::normalize() {
    // O(1) normalization
    if (month < 1 || month > 12) [[unlikely]] { //depending branch predictor
        year += (month - 12) / 12;
        month = ((month - 1) % 12 + 12) % 12 + 1;
    }

    // applying +1 to MONTH_DAYS_SUM :
    // (leap && (monthApprox >= 3)) + MONTH_DAYS_SUM[monthApprox]
    day += getTotalDaysFromThisYear(year, month);

    // minimum date : 0001/01/01
    // additional days : day - 1 (1~365 => 0~364 mapping, applying day-1)
    const int totalDays = std::max(365, calcTotalDaysFromYear(year) + day - 1);

    //146000 + 97 (24 * 4 = 96, 400 is leap year. 96 + 1 = 97)
    //36500 + 24 (100 / 4 = 25, 100 is not leap year. 25 - 1 = 24)
    //1460 + 1(leaps)
    int yearCalcDays = totalDays - 365;
    yearCalcDays -= yearCalcDays / 146097;
    yearCalcDays += yearCalcDays / 36524;
    yearCalcDays -= yearCalcDays / 1461;

    // get year
    year = yearCalcDays / 365 + 1;
    const int totalDaysForYear = calcTotalDaysFromYear(year);
    int currYearDays = totalDays - totalDaysForYear + 1;

    // if day is zero, it must be like LEAP/12/31. solve 366
    year -= currYearDays == 0;
    currYearDays += 366 * (currYearDays == 0);
    const bool leap = isLeap(year);

    // approximate month
    const int monthApprox = (currYearDays >> 5) + 2;
    // get month
    month = monthApprox - (currYearDays <= getTotalDaysFromThisYear(leap, monthApprox));
    // get day
    day = currYearDays - getTotalDaysFromThisYear(leap, month);
}

bool Day::isValid(const int y, const int m, const int d) {
    return y >= 1 && m >= 1 && m <= 12 && d >= 1 && d <= getMonthDays(y, m);
}
