#pragma once
#include <iosfwd>

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

std::ostream &operator<<(std::ostream &os, const Day &day);
