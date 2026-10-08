//
// Created by leegu on 26. 10. 7..
//

#include "Day.h"

Day::Day()
    : year(2026), month(10), day(1) {
}

Day::Day(char date_text[]) {
    *this = parse_day(date_text);
}

Day::Day(int year, int month, int day)
    : year(year), month(month), day(day) {
}

int Day::get_year() const {
    return this->year;
}

int Day::get_month() const {
    return this->month;
}

int Day::get_day() const {
    return this->day;
}

void Day::set(int year, int month, int day){
    this->year = year;
    this->month = month;
    this->day = day;

    this->normalize();
}

Day Day::operator+(int day) {
    this->add_day(day);
    return *this;
}

Day Day::operator-(int day) {
    this->sub_day(day);
    return *this;
}

void Day::operator++(){
    this->add_day();
}

void Day::operator--(){
    this->sub_day();
}

void Day::add_day(int day_delta) {
    this->day += day_delta;
    this->normalize();
}

void Day::sub_day(int day_delta) {
    this->day -= day_delta;
    this->normalize();
}

void Day::normalize() {
    if (this->day <= this->get_day_in_month()) {
        return;
    }


    for (; this->day > this->get_day_in_month(); this->day -= this->get_day_in_month()) ++this->month;

    int carry_year = this->month / 12;
    int changed_month = this->month % 12;

    this->year += carry_year;
    this->month = changed_month;
}

bool Day::is_leap_year() const {
    return (this->year % 4 == 0) && (this->year % 100 != 0) || (this->year % 400 == 0);
}

int Day::get_day_in_month() const {
    if (this->is_leap_year() && this->month == 2) {
        return 29;
    }

    return DAYS_IN_MONTHS_ON_COMMON_YEAR[this->month - 1];
}

Day Day::parse_day(char date_text[]) {
    Day new_day = Day();

    return new_day;
}

std::ostream& operator<<(std::ostream &output_stream, const Day &day) {
    return output_stream;
}
