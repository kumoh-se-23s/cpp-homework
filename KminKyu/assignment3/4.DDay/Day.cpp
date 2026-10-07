#include<iostream>
#include "Day.h"

//기원전 처리 가능
Day::Day(int year, int month, int day) {
    set(year ,month, day);
}

bool Day::isLeapYear(int year) {
    return (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0));
}
bool Day::isCorrectDate(int year, int month, int day) {
    if (month < 0 || MAX_OF_MONTH < month) {
        return false;
    }
    int correctDay = getMaxOfDays(year, month);



    if (day < 0 || correctDay < day) {
        return false;
    }

    return true;
}

int Day::getMaxOfYear(int year) {
    if (isLeapYear(year)) {
        return MAX_OF_LEAF_YEAR;
    } else {
        return MAX_OF_NORMAL_YEAR;
    }
}

int Day::getRemainDays(int year, int month, int day) {
    int remainDay = 0;
    for (int nowMonth = 1; nowMonth < month; ++nowMonth) {
        remainDay += MAX_OF_DAYS[nowMonth];
    }
    if (isLeapYear(year) && month > 2) {
        return remainDay + day + 1;
    } else {
        return remainDay + day;
    }
}

Day Day::calculateShortTermDays(int year, int day) const { //이 코드가 실행된 시점에서 날짜는 XXXX.12.31로 고정된다
    int maxDay = getMaxOfYear(year);
    if (day >= maxDay || day == 0) { //만일의 오류를 위한 코드 -> 테스트 후 삭제 예정
        return Day(this->year, this->month, this->day);
    }

    int remainDay;

    if (day < 0) {
        remainDay = maxDay + day;
    } else {
        remainDay = day;
    }

    int resultMonth = 1;
    int resultDay = 1;

    int lastDay = getMaxOfDays(year, resultMonth);

    while (remainDay >= lastDay) {
        remainDay -= lastDay;
        lastDay = getMaxOfDays(year, ++resultMonth);
    }
    resultDay = remainDay;

    return Day(this->year, resultMonth, resultDay);
}
int Day::getMaxOfDays(int year, int month) {
    if (month == 2 && isLeapYear(year)) {
        return MAX_OF_DAYS[month] + 1;
    }
    return MAX_OF_DAYS[month];
}
Day Day::calculateDays(int day) const {
    char sign;
    int resultYear = this->year;

    if (day < 0) { //day 가 365(366) 보다 큰 마이너스 값인 경우
        sign = -1;
        day *= -1;
        day += getMaxOfYear(resultYear) - getRemainDays(resultYear, this->month, this->day);  //연말까지 남은 일수롤 더해서 xxxx.12.31로 취급
    } else if (day > 0) { //day가 365(366)보다 큰 플러스 값인 경우
        sign = 1;
        day += getRemainDays(resultYear, this->month, this->day); //올해 일수를 전부 더해서 xxxx.12.31로 취급
        if (day < getMaxOfYear(resultYear)) {
            return calculateShortTermDays(resultYear, day);
        }
    } else {
        return Day(this->year, this->getMonth(), this->getDay());
    }
    if (day < getMaxOfYear(resultYear)) {
        return calculateShortTermDays(resultYear, day);
    }

    while (day >= getMaxOfYear(resultYear)) {
            day -= getMaxOfYear(resultYear);
            resultYear += sign;
    }
    return Day(resultYear, this->month, this->day);
}

bool Day::set(int year, int month, int day) {
    if (isCorrectDate(year, month, day)) {
        this->year = year;
        this->month = month;
        this->day = day;
        return true;
    } else {
        return false;
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

Day Day::operator+(int day) const {
    return calculateDays(day);
}

Day Day::operator-(int day) const {
    return calculateDays(-1 * day);
}

Day & Day::operator++() {
    this->day += 1;
    if (day > getMaxOfDays(this->year, this->month)) {
        this->month += 1;
        this->day = 1;
    }
    if (month > MAX_OF_MONTH) {
        this->year += 1;
        this->month = 1;
    }
    return *this;
}

Day & Day::operator--() {
    this->day -= 1;
    if (day < 1) {
        this->month -= 1;
        this->day = getMaxOfDays(this->year, this->month);
    }
    if (month < 1) {
        this->year -= 1;
        this->month = MAX_OF_MONTH;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Day &day) {
    if (day.getYear() < 1) {
        out << "BC" << abs(day.getYear() - 1);
    } else {
        out << day.getYear();
    }

    out << '/' << day.getMonth() << "/" << day.getDay();
    return out;
}


