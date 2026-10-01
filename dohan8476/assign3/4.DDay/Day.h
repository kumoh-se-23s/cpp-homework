#pragma once

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

        Day operator+(int day) const;
        Day operator-(int day) const;
        Day operator++();
        Day operator--();

    private:
        int year;
        int month;
        int day;
};