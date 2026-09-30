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
    
    static constexpr int LEAP_MONTH_DAYS_SUM[13] = {
        0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
    };

    public:
    explicit Day(int year = 2026, int month = 10, int day = 1);
    
    Day operator++();
    
    Day operator--();

    Day operator+(int d);
    
    Day operator-(int d);

    static int getDays(int year, int month);

    static bool isLeap(int year);

    int getYear() const {return year;}
    int getMonth() const {return month;}
    int getDay() const {return day;}

    static bool isValid(int y, int m, int d);

private:
    void normalize();
};




inline std::ostream &operator<<(std::ostream &out, const Day & day){
    return out << std::setfill('0') << std::setw(4) << day.getYear() << "/" << std::setw(2) << day.getMonth() << "/" << std::setw(2) << day.getDay();

}