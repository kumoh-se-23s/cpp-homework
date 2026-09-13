#include <iostream>
#include <string>
#include "Date.h"

using namespace std;

Date::Date(int y, int m, int d) {
    setYear(y);
    setMonth(m);
    setDay(d);
    setDayOfWeek();
}

void Date::setYear(int y) {
    if (y < 1) y = 1;
    year = y;
}

void Date::setMonth(int m) {
    if (m < 1 || 12 < m) m = 1;
    month = m;
}

void Date::setDay(int d) {
    static const int MAX_DAY_OF_MONTH[MAX_MONTH] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (d < 1 || MAX_DAY_OF_MONTH[month - 1] < d || (month == 2 && !(year % 4 == 0 && year % 100 != 0 || year % 400 == 0) && d == 29)) d = 1;
    day = d;
}
void Date::read() {
    int y,m,d;
    cout << "년 월 일 입력 : ";
    cin >> y >> m >> d;
    setYear(y);
    setMonth(m);
    setDay(d);
    setDayOfWeek();
}

void Date::setDayOfWeek() {
    int y = year, m = month, d = day;
    if (m < 3) {
        m += 12;
        y--;
    }
    dayOfWeek = (y + y / 4 - y / 100 + y / 400 + (13 * m + 8) / 5 + d) % 7;
}

string Date::getString() const {
    static const string DAY_OF_WEEK[7] = {"일", "월", "화", "수", "목", "금", "토"};
    return to_string(year) + "년 " + to_string(month) + "월 " + to_string(day) + "일 (" + DAY_OF_WEEK[dayOfWeek] + ")";
}