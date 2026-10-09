#pragma once
#include <iostream>

using namespace std;

class Day {
public:
    //생성자
    Day();
    Day(int newYear, int newMonth, int newDay);

    //getter setter
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    bool setValue(int newYear, int newMonth, int newDay);

    //연산자 오버로딩
    Day operator++();
    Day operator--();
    const Day operator+(int input) const;
    const Day operator-(int input) const;
private:
    int year;
    int month;
    int day;

    int getDaysInMonth(int year, int month) const;
    bool isLeapYear(int year) const;
    int getDaysOfAnYear(int year, int startMonth) const;
};

ostream& operator<<(ostream& out, const Day& day);