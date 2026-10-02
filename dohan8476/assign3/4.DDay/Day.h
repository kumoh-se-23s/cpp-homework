#pragma once
#include <iosfwd>

//Day 내부엔 year, month, day만 가지고 싶어서
//namespace로 묶어서 DateCalculator에 오버로딩을 전역?함수 느낌으로 구현했는데
//너무 억지같기도하고 근데 Day에서 계산을 가질거면 굳이 DateCalculator 분리할 이유 없을거같고
//흠,,,,,,,,,
namespace Calendar {

    class Day {
    public:
        Day();
        Day(int year, int month, int day);
        int getYear() const;
        int getMonth() const;
        int getDay() const;

        void setYear(int year);
        void setMonth(int month);
        void setDay(int day);

    private:
        int year;
        int month;
        int day;
    };

    std::ostream& operator<<(std::ostream& os, const Day& day);
}