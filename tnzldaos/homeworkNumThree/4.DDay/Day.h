#pragma once
#include <ostream>
using namespace std;

// if문 최대한 덜 쓰기 + constexpr 사용 -> 속도 증가

class Day {
    public:

    private:
        static const short dayOfMonth[2][13];

        int year;
        int month;
        int day;

    public:
        Day();
        Day(int year, int month, int day);

        int getYear() const;
        int getMonth() const;
        int getDay() const;

        bool setDay(int inputYear, int inputMonth, int inputDay);

        bool isLeapYear(int inputYear) const;
        bool isValid(int inputYear, int inputMonth, int inputDay) const;

        Day operator+(int inputDDay) const;
        Day operator-(int inputDDay) const;
        void operator++();
        void operator--();

    private:
};