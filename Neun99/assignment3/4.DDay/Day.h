#pragma once
#include <iostream>
#include <string>

using namespace std;

class Day {
public:
    //생성자
    Day();

    //getter
    int getYear() const;
    int getMonth() const;
    int getDay() const;

    string toString() const;
    bool setValue(int, int, int);

    //연산자 오버로딩
    Day operator++();
    Day operator--();
    // const Day operator+(int) const;
    // const Day operator-(int) const;
private:
    int year;
    int month;
    int day;

    int getDaysInMonth(int, int) const;
    bool isLeapYear(int) const;
};

ostream& operator<<(ostream&, const Day&);