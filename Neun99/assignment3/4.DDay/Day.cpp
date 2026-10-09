#include <iostream>
#include "Day.h"
#include <iomanip>

using namespace std;

//생성자--------------------
Day::Day() {
    year = 2026;
    month = 10;
    day = 1;
}

Day::Day(int newYear, int newMonth, int newDay) {
    year = newYear;
    month = newMonth;
    day = newDay;
}

//getter setter-------------------
//년 반환
int Day::getYear() const{
    return year;
}

//월 반환
int Day::getMonth() const {
    return month;
}

//일 반환
int Day::getDay() const {
    return day;
}

//유효하지 않은 값 들어오면 변경하지 않고 false 리턴
bool Day::setValue(int newYear, int newMonth, int newDay) {
    if (newMonth < 1 || newMonth > 12)
        return false;
    if (newDay < 1 || newDay > getDaysInMonth(newYear, newMonth))
        return false;

    year = newYear;
    month = newMonth;
    day = newDay;

    return true;
}

//기능---------------------
//해당 달에 며칠까지 있는지 반환 (static?)
int Day::getDaysInMonth(int year, int month) const {
    static constexpr int DAYS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year) && month == 2)
        return 29;
    return DAYS[month - 1];
}

//윤년인지 반환
bool Day::isLeapYear(int year) const{
    if (year % 400 == 0)
        return true;
    if (year % 100 == 0)
        return false;
    if (year % 4 == 0)
        return true;
    return false;
}

//인자로부터 앞으로 1년이 며칠인지 반환
int Day::getDaysOfAnYear(int year, int startMonth) const {
    //올해가 윤년인데 2월이 안 지남 or 2월 이후부터인데 내년이 윤년임 -> 366일
    if ((isLeapYear(year) && startMonth <= 2) || (startMonth >= 3 && isLeapYear(year + 1)))
        return 366;
    return 365; //그 외
}

//연산자 오버로딩----------------------------
//전위++
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

//전위--
Day Day::operator--() {
    if (day == 1) { //1일이면 전월 말일로
        if (month == 1) { //1월이면 전년도 12월로
            year--;
            month = 12;
        } else {
            month--;
        }
        day = getDaysInMonth(year, month);
    } else {
        day--;
    }
    return *this;
}

//이항+
const Day Day::operator+(int input) const {
    int newYear = year;
    int newMonth = month;
    int newDay = day;

    //400년단위 선처리
    constexpr int DAYS_OF_400YEAR = 146097; //400년 일수
    newYear += 400 * (input / DAYS_OF_400YEAR);
    input %= DAYS_OF_400YEAR;

    //년 단위 처리 (최대 400번 동작)
    for (; input >= getDaysOfAnYear(newYear, newMonth); newYear++) {
        input -= getDaysOfAnYear(newYear, newMonth)   ;
    }

    //달 단위 처리(최대 12번 동작)
    while (input >= getDaysInMonth(newYear, newMonth)) {
        input -= getDaysInMonth(newYear, newMonth);

        //input -=가 변동 전 year와 month로 이뤄져야 하며, 변동에는 if문 로직이 들어가므로 while 사용
        if (newMonth == 12) {
            newYear++;
            newMonth = 1;
        } else {
            newMonth++;
        }
    }

    //일 단위 처리(최대 2번 동작)
    newDay += input;
    while (newDay > getDaysInMonth(newYear, newMonth)) {
        newDay -= getDaysInMonth(newYear, newMonth);

        if (newMonth == 12) {
            newYear++;
            newMonth = 1;
        } else {
            newMonth++;
        }
    }

    return Day(newYear, newMonth, newDay);
}

//이항-
const Day Day::operator-(int input) const {
    int newYear = year;
    int newMonth = month;
    int newDay = day;

    //400년단위 선처리
    constexpr int DAYS_OF_400YEAR = 146097; //
    newYear -= 400 * (input / DAYS_OF_400YEAR);
    input %= DAYS_OF_400YEAR;

    //년 단위 처리 (최대 400번 동작)
    for (; input >= getDaysOfAnYear(newYear - 1, newMonth); newYear--) {
        input -= getDaysOfAnYear(newYear - 1, newMonth);
    }

    //달 단위 처리(최대 12번 동작)
    while (input >= getDaysInMonth(newYear, newMonth == 1 ? 12 : newMonth - 1)) {
        if (newMonth == 1) {
            newYear--;
            newMonth = 12;
        } else {
            newMonth--;
        }
        input -= getDaysInMonth(newYear, newMonth);
    }

    //일 단위 처리(최대 2번 동작)
    newDay = day - input;
    while (newDay <= 0) {
        if (newMonth == 1) {
            newYear--;
            newMonth = 12;
        } else {
            newMonth--;
        }
        newDay += getDaysInMonth(newYear, newMonth);
    }

    return Day(newYear, newMonth, newDay);
}

//<<
ostream& operator<<(ostream& out, const Day& day) {
    out << setfill ('0') << setw(4) << day.getYear() << "/";
    out << setw(2) << day.getMonth() << "/";
    out << setw(2) << day.getDay();
    return out;
}