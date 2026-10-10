#pragma once

#include<iostream>

class Day {
public:
    Day(int y = 2026, int m = 10, int d = 1);

    static bool isCorrectDate(int year, int month, int day);

    void set(int year, int month, int day);

    int getYear() const;

    int getMonth() const;

    int getDay() const;

    Day operator+(int dday) const;

    Day operator-(int dday) const;

    Day& operator++();

    Day& operator--();

private:
    int year;

    int month;

    int day;

    Day calculateDays(int dday) const;

    static constexpr int MAX_OF_DAYS[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    static constexpr int MAX_OF_MONTH = 12;

    static constexpr int MAX_OF_NORMAL_YEAR = 365;

    static Day calculateShortDays(int year, int dday);

    static int getDayOfYear(int year, int month, int day);

    static int getMaxOfYear(int year);

    static bool isLeapYear(int year);

    static int getMaxOfDays(int year, int month);
};

std::ostream &operator<<(std::ostream& out, const Day& day);
