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

Day::Day(const int year, const int month, const int day)
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

void Day::set(const int year, const int month, const int day){
    this->year = year;
    this->month = month;
    this->day = day;

    this->normalize();
}

Day Day::operator+(const int day_delta) const {
    Day result_day = Day(
        this->year,
        this->month,
        this->day
    );

    result_day.add_day(day_delta);

    return result_day;
}

Day Day::operator-(const int day_delta) const {
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

void Day::add_day(const int day_delta) {
    this->day += day_delta;
    this->normalize();
}

void Day::sub_day(const int day_delta) {
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
        this->day += get_day_in_month(*this);
    }

    while (this->day > get_day_in_month(*this)) {
        this->day -= get_day_in_month(*this);
        ++this->month;
        if (this->month > 12) {
            this->month = 1;
            ++this->year;
        }
    }
}

bool Day::is_valid_day(const Day &maybe_valid_day) {
    const bool is_correct_year_bound = 1 <= maybe_valid_day.year;
    const bool is_correct_month_bound = 1 <= maybe_valid_day.month && maybe_valid_day.month <= 12;
    const bool is_correct_day_bound = 1 <= maybe_valid_day.day && maybe_valid_day.day <= get_day_in_month(maybe_valid_day);

    return is_correct_year_bound && is_correct_month_bound && is_correct_day_bound;
}

bool Day::is_leap_year(const Day &maybe_leap_year) {
    return (maybe_leap_year.year % 4 == 0) && (maybe_leap_year.year % 100 != 0) || (maybe_leap_year.year % 400 == 0);
}

int Day::get_day_in_month(const Day &day) {
    if (is_leap_year(day) && day.month == 2) {
        return 29;
    }

    return DAYS_IN_MONTHS_ON_COMMON_YEAR[day.month - 1];
}

Day Day::parse_day(const char date_text[]) {
    const int date_text_length = char_utils::get_char_array_length(date_text);

    if (date_text_length != 8) {
        return Day();
    }

    return Day(
        char_utils::to_ranged_integer(date_text, 0, 3, date_text_length),
        char_utils::to_ranged_integer(date_text, 4, 5, date_text_length),
        char_utils::to_ranged_integer(date_text, 6, 7, date_text_length)
    );
}

std::ostream &operator<<(std::ostream &output_stream, const Day &day) {
    // year의 경우 큰수의 day_delta 입력시, 계속해서 늘어나도록 의도하였기에,
    // 만약에 1000이하로 줄어들어도 0으로 padding을 하지 않는 것이 맞는것 같음.
    if (day.get_year() <= 0) {
        output_stream << "BC - " << std::abs(day.get_year() - 1);
    } else {
        output_stream << day.get_year();
    }
    output_stream << "/";


    if (day.get_month() < 10) {
        output_stream << "0";
    }
    output_stream << day.get_month() << "/";

    if (day.get_day() < 10) {
        output_stream << "0";
    }
    output_stream << day.get_day();

    return output_stream;
}
