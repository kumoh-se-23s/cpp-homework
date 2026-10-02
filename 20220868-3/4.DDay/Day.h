#pragma once

#include <iostream>
#include <iomanip>

class Day{
    int year;
    int month;
    int day;
    
    static constexpr int MONTH_DAYS_SUM[13] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
    };

    public:
    explicit Day(int year = 2026, int month = 10, int day = 1);
    
    Day operator++();
    
    Day operator--();

    Day operator+(int d) const;
    
    Day operator-(int d) const;

    [[nodiscard]] static int getMonthDays(int year, int month);

    [[nodiscard]] static bool isLeap(int year);

    [[nodiscard]] int getYear() const {return year;}
    [[nodiscard]] int getMonth() const {return month;}
    [[nodiscard]] int getDay() const {return day;}

    static bool isValid(int y, int m, int d);

private:

    static int getTotalDaysFromThisYear(int y, int m);

    static int getTotalDaysFromThisYear(bool leap, int m);

    void normalizeIfNeeded();

    static int calcTotalDaysFromYear(int year);

    void normalize();
};




inline std::ostream &operator<<(std::ostream &out, const Day & day){
    return out << std::setfill('0') << std::setw(4) << day.getYear() << "/" << std::setw(2) << day.getMonth() << "/" << std::setw(2) << day.getDay();

}