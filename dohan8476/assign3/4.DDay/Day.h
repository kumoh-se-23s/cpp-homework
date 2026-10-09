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

        Day operator+(int addDays) const;
        Day operator-(int subDays) const;
        Day& operator++();
        Day& operator--();


    private:
        int year;
        int month;
        int day;
};

std::ostream &operator<<(std::ostream &os, const Day &day);
