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
    if (newYear <= 0)
        return false;
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
//해당 달에 며칠까지 있는지 반환
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
int Day::getTotalDays() const {
    int result = 0;

    //계산상 편의를 위해 시작일을 0001-01-01이 아닌 0000-00-00으로 잡음
    //년도 일수로 쪼개기
    int tempYear = year;
    //400년
    result += DAY_OF_400YEARS * (tempYear/400);
    tempYear %= 400;
    //100년
    result += DAY_OF_100YEARS * (tempYear/100);
    tempYear %= 100;
    //4년
    result += DAY_OF_4YEARS * (tempYear/4);
    result += 365 * (tempYear%4);

    //달 쪼개기(최대 12번 동작)
    for (int nowMonth = month; nowMonth > 0; nowMonth--) {
        result += getDaysInMonth(year, nowMonth);
    }

    //남은 일수 더해서 리턴
    result += day;
    return result;
}

Day Day::totalDaysToDay(int totalDays) const {

    //0년 0월 0일 시작이 기준, 윤년 위치 고정
    int newYear = 0;
    int newMonth = 0;
    int newDay = 0;

    //년도: 0년 0월 0일 기준이므로 위와 동일하게 윤년 위치 고정
    //400년
    newYear += 400 * (totalDays/DAY_OF_400YEARS);
    totalDays %= DAY_OF_400YEARS;
    //100년
    newYear += 100 * (totalDays/DAY_OF_100YEARS);
    totalDays %= DAY_OF_100YEARS;
    //4년
    newYear += 4 * (totalDays/DAY_OF_4YEARS);
    totalDays %= DAY_OF_4YEARS;
    //1~3년
    newYear += totalDays/365;
    totalDays %= 365;

    //월
    for (;totalDays >= getDaysInMonth(newYear, newMonth + 1); newMonth++) {
        totalDays -= getDaysInMonth(newYear, newMonth + 1);
    }
    if (newMonth == 0) { //0월이면 전년도 12월로 조정
        newYear--;
        newMonth = 12;
    }

    //남은 일수 0일이면 전월 말일로 조정
    if (totalDays == 0) {
        if (newMonth == 1) {
            newYear--;
            newMonth = 12;
        } else
            newMonth--;

        newDay = getDaysInMonth(newYear, newMonth);
    } else {
        newDay += totalDays;
    }

    return Day(newYear, newMonth, newDay);
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
    int temp = getTotalDays();
    return totalDaysToDay(temp + input);
}

//이항-
const Day Day::operator-(int input) const {
    return totalDaysToDay(getTotalDays() - input);
}

//<<
ostream& operator<<(ostream& out, const Day& day) {
    out << setfill ('0') << setw(4) << day.getYear() << "/";
    out << setw(2) << day.getMonth() << "/";
    out << setw(2) << day.getDay();
    return out;
}