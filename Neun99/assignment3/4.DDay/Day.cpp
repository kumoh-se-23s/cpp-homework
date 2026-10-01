#include <iostream>
#include <string>
#include "Day.h"
#include <sstream>

using namespace std;

//생성자--------------------
Day::Day() {
    year = 2026;
    month = 10;
    day = 1;
}
//getter-------------------
int Day::getYear() const{
    return year;
}

int Day::getMonth() const {
    return month;
}

int Day::getDay() const {
    return day;
}

//기능---------------------
string Day::toString() const{
    ostringstream result;
    result << year << "/";
    if (month < 10) //한자리수면 0 붙여주기
        result << "0";
    result << month << "/";
    if (day < 10)
        result << "0";
    result << day;

    return result.str();
}

//유효하지 않은 값 들어오면 변경하지 않고 false 리턴
bool Day::setValue(int newYear, int newMonth, int newDay) {
    year = newYear;

    if (newMonth < 1 || newMonth > 12)
        return false;
    month = newMonth;

    if (newDay < 1 || newDay > getDaysInMonth(year, month))
        return false;
    day = newDay;
}

//해당 달에 며칠까지 있는지 반환
int Day::getDaysInMonth(int year, int month) const {
    switch (month) {
        case 2:
            if (isLeapYear(year))
                return 29;
            return 28;
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        default:
            return 30;
    }
}

//윤년인지 판단
bool Day::isLeapYear(int year) const{
    if (year % 400 == 0)
        return true;
    if (year % 100 == 0)
        return false;
    if (year % 4 == 0)
        return true;
    return false;
}

//연산자 오버로딩----------------------------
Day Day::operator++() {
    if (day == getDaysInMonth(year, month)) { //일수 꽉 찼으면 month 올리고 day = 1
        if (month == 12) { //달수 꽉 찼으면 year 올리고 month = 1;
            year++;
            month = 1;
        } else {
            month++;
        }
        day = 1;
    } else {
        day++;
    }
    return *this;
}


Day Day::operator--() {
    if (day == 1) {
        if (month == 1) {
            year--;
            month = 12;
        } else {
            month--;
        }
        day = getDaysInMonth(year, month) - 1;
    } else {
        day--;
    }
    return *this;
}

ostream& operator<<(ostream& out, const Day& day) {
    out << day.toString();
    return out;
}