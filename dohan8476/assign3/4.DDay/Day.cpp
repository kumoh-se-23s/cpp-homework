#include "Day.h"

#include <iomanip>

//default : 2026/10/01
Day::Day() {
    year = 2026;
    month = 10;
    day = 1;
}

Day::Day(int year, int month, int day) {
    this->year = year;
    this->month = month;
    this->day = day;
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

void Day::setYear(int year) {
    this->year = year;
}
void Day::setMonth(int month) {
    this->month = month;
}
void Day::setDay(int day) {
    this->day = day;
}

//일단 00nn년/0n월/0n일만 출력
std::ostream& operator<<(std:: ostream& out, const Day& day) {

    out << std::setfill('0') << std::setw(4) << day.getYear() << "/";
    out << std::setfill('0') << std::setw(2) << day.getMonth() << "/";
    out << std::setfill('0') << std::setw(2) << day.getDay();

    return out;
}