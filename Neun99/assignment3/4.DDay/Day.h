#pragma once
#include <iostream>
#include <string>

using namespace std;

class Day {
public:
    //생성자
    Day();
    Day(int, int, int);

    //getter setter
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    bool setValue(int, int, int);

    //연산자 오버로딩
    Day operator++();
    Day operator--();
    const Day operator+(int) const;
    const Day operator-(int) const;
private:
    int year;
    int month;
    int day;

    //totalDays 계산용 상수들
    static constexpr int DAY_OF_400YEARS = 146097;
    static constexpr int DAY_OF_100YEARS = 36524;
    static constexpr int DAY_OF_4YEARS = 1461;

    int getDaysInMonth(int, int) const;
    bool isLeapYear(int) const;
    int getTotalDays() const;
    Day totalDaysToDay(int) const;
};

ostream& operator<<(ostream&, const Day&);