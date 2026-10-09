#include "Day.h"
#include <iomanip>
#include "DDay.h"

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

//000nn년/0n월/0n일 형식으로 출력
std::ostream& operator<<(std:: ostream& out, const Day& day) {

    // out << std::setfill('0') << std::setw(4) << day.getYear() << "/";
    // out << std::setfill('0') << std::setw(2) << day.getMonth() << "/";
    // out << std::setfill('0') << std::setw(2) << day.getDay();

    out << std::setfill('0') << std::setw(4) << day.getYear() << '/';
    out << std::setw(2) << day.getMonth() << '/';
    out << std::setw(2) << day.getDay();

    return out;
}

//연산자 오버로딩
Day& Day::operator++() {
    DDay calc;

    int year = this->getYear();
    int month = this->getMonth();
    int day = this->getDay() + 1;

    if (day > calc.getDaysInMonth(year, month)) {
        day = 1;
        ++month;

        if (month > 12) {
            month = 1;
            ++year;
        }
    }

    this->setYear(year);
    this->setMonth(month);
    this->setDay(day);

    return *this;
}

Day& Day::operator--() {
    DDay calc;

    int year = this->getYear();
    int month = this->getMonth();
    int day = this->getDay() - 1;

    if (day == 0) {
        --month;

        if (month == 0) {
            month = 12;
            --year;
        }
        //마지막 날로 세팅
        day = calc.getDaysInMonth(year, month);
    }

    this->setYear(year);
    this->setMonth(month);
    this->setDay(day);

    return *this;
}

Day Day::operator+(int addDays) const{
    DDay calc;
    int totalDays = calc.dateToTotalDays(getYear(), getMonth(), getDay());

    return calc.totalDaysToDate(totalDays + addDays);
}

Day Day::operator-(int subDays) const{
    DDay calc;
    int totalDays = calc.dateToTotalDays(getYear(), getMonth(), getDay());

    return calc.totalDaysToDate(totalDays - subDays);
}
