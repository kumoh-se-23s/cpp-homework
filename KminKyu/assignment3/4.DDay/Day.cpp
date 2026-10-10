#include<iostream>
#include "Day.h"

#include <iomanip>

Day::Day(int year, int month, int day) {
    set(year ,month, day);
}

bool Day::isLeapYear(int year) {
    return (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0));
}
bool Day::isCorrectDate(int year, int month, int day) {
    if (month < 1 || MAX_OF_MONTH < month) {
        return false;
    }
    int correctDay = getMaxOfDays(year, month);

    if (day < 1 || correctDay < day) {
        return false;
    }

    return true;
}

int Day::getMaxOfYear(int year) {
    if (isLeapYear(year)) {
        return MAX_OF_NORMAL_YEAR + 1;
    }
    return MAX_OF_NORMAL_YEAR;
}

int Day::getDayOfYear(int year, int month, int day) {
    int remainDay = 0;
    for (int nowMonth = 1; nowMonth < month; ++nowMonth) {
        remainDay += MAX_OF_DAYS[nowMonth];
    }
    if (isLeapYear(year) && month > 2) {
        return remainDay + day + 1;
    }
    return remainDay + day;

}

int Day::getMaxOfDays(int year, int month) {
    if (month == 2 && isLeapYear(year)) {
        return MAX_OF_DAYS[month] + 1;
    }
    return MAX_OF_DAYS[month];
}

Day Day::calculateShortDays(int year, int dday) { //이 코드가 실행된 시점에서 날짜는 XXXX.12.31로 고정된다
    int maxDay = getMaxOfYear(year);

    int remainDay = dday;

    if (dday < 0) {
        remainDay += maxDay;      //day가 음수인 경우 365(366) - day 변환
    }

    int resultMonth = 1;
    int resultDay = 1;

    int lastDay = getMaxOfDays(year, resultMonth);

    while (remainDay > lastDay) {
        remainDay -= lastDay;
        lastDay = getMaxOfDays(year, ++resultMonth);
    }
    resultDay = remainDay;

    return Day(year, resultMonth, resultDay);
}

Day Day::calculateDays(int dday) const {
    char sign;
    int resultYear = this->year;
    int resultMonth, resultDay;

    if (dday < 0) {
        //올해 남은 일수롤 더해서 xxxx.12.31로 취급
        sign = -1;
        dday *= -1;
        dday += getMaxOfYear(resultYear) - getDayOfYear(resultYear, this->month, this->day);
    } else if (dday > 0) {
        //현재까지 일수를 전부 더해서 xxxx-1.12.31로 취급, 계산은 xxxx년으로 해야 하므로 year -1 처리 x
        sign = 1;
        dday += getDayOfYear(resultYear, this->month, this->day);
    } else {
        return Day(this->year, this->getMonth(), this->getDay());
    }
    resultMonth = MAX_OF_MONTH, resultDay = MAX_OF_DAYS[resultMonth];

    while (dday >= getMaxOfYear(resultYear)) {
        dday -= getMaxOfYear(resultYear);
        resultYear += sign;
    }

    if (dday > 0) {
        return calculateShortDays(resultYear, sign * dday);
    }
    if (sign == 1) {
        resultYear -= 1; //실제론 year -1을 해야 하지만 계산을 위해 year -1을 하지 않았으므로 연산 후 year -1;
    }
    return Day(resultYear, resultMonth, resultDay);
}

void Day::set(int year, int month, int day) {
    if (isCorrectDate(year, month, day)) {
        this->year = year;
        this->month = month;
        this->day = day;
    }
}

int Day::getYear() const {
    return year;
}

int Day::getMonth() const {
    return month;
}

int Day::getDay() const {
    return day;
}

Day Day::operator+(int dday) const {
    return calculateDays(dday);
}

Day Day::operator-(int dday) const {
    return calculateDays(-1 * dday);
}

Day &Day::operator++() {
    this->day += 1;
    if (day > getMaxOfDays(this->year, this->month)) {
        this->month += 1;
        if (month > MAX_OF_MONTH) {
            this->year += 1;
            this->month = 1;
        }
        this->day = 1;
    }

    return *this;
}

Day &Day::operator--() {
    this->day -= 1;
    if (day < 1) {
        this->month -= 1;
        if (month < 1) {
            this->year -= 1;
            this->month = MAX_OF_MONTH;
        }
        this->day = getMaxOfDays(this->year, this->month);
    }

    return *this;
}

std::ostream &operator<<(std::ostream &out, const Day &day) {
    if (day.getYear() < 1) {
        out << "[BC]" << std::setfill('0') << std::setw(4) << abs(day.getYear() - 1);
    } else {
        out << std::setfill('0') << std::setw(4) << day.getYear();
    }
    out << '/' << std::setw(2) << day.getMonth();
    out << "/" << std::setw(2) << day.getDay();
    return out;
}


