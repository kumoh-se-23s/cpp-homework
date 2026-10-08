//
// Created by leegu on 26. 10. 7..
//

#include "Day.h"
#include "CharacterUtils.h"

Day::Day()
    : year(2026), month(10), day(1) {
}

Day::Day(const char date_text[]) {
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

Day Day::operator+(int day_delta) const {
    Day result_day = Day(
        this->year,
        this->month,
        this->day
    );

    result_day.add_day(day_delta);

    return result_day;
}

Day Day::operator-(int day_delta) const {
    Day result_day = Day(
        this->year,
        this->month,
        this->day
    );

    result_day.sub_day(day_delta);

    return result_day;
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
    // 유레카!
    // 0 & Negative 일수에 대한 보정 작업
    // 0 & Negative의 경우 달(필요할 경우 년도)을 감소시켜가며 일수를 양수화를 진행
    while (this->day < 1) {
        --this->month;
        if (this->month < 1) {
            this->month = 12;
            --this->year;
        }
        this->day += this->get_day_in_month();
    }

    while (this->day > this->get_day_in_month()) {
        this->day -= this->get_day_in_month();
        ++this->month;
        if (this->month > 12) {
            this->month = 1;
            ++this->year;
        }
    }
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

Day Day::parse_day(const char date_text[]) {
    int date_text_length = char_utils::get_char_array_length(date_text);

    if (date_text_length != 8) {
        return Day();
    }

    return Day(
        char_utils::to_positive_integer(date_text, 0, 3, date_text_length),
        char_utils::to_positive_integer(date_text, 4, 5, date_text_length),
        char_utils::to_positive_integer(date_text, 6, 7, date_text_length)
    );
}

std::ostream& operator<<(std::ostream &output_stream, const Day &day) {
    return output_stream << day.get_year() << "/" << day.get_month() << "/" << day.get_day();
}
