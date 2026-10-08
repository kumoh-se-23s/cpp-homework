//
// Created by leegu on 26. 10. 7..
//

#ifndef CPP_HOMEWORK_DAY_H
#define CPP_HOMEWORK_DAY_H

#include <iostream>

class Day {
    public:
        Day();

        Day(char date_text[]);

        Day(int year, int month, int day);

        int get_year() const;

        int get_month() const;

        int get_day() const;

        void set(int year, int month, int day);

        Day operator+(int day);

        Day operator-(int day);

        void operator++();

        void operator--();


    private:
        constexpr static int DAYS_IN_MONTHS_ON_COMMON_YEAR[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        int year;
        int month;
        int day;

        void normalize();

        void add_day(int day_delta = 1);

        void sub_day(int day_delta = 1);

        bool is_leap_year() const;

        int get_day_in_month() const;

        static Day parse_day(char date_text[]);
};

std::ostream &operator <<(std::ostream &output_stream, const Day &day);

#endif //CPP_HOMEWORK_DAY_H
