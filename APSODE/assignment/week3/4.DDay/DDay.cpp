//
// Created by leegu on 26. 10. 7..
//

#include "DDay.h"
#include "Day.h"


DDay::DDay()
    : day(Day()), day_delta(0) {
}

DDay::DDay(Day manual_day)
    : day(manual_day), day_delta(0) {
}

DDay::DDay(char date_text[])
    : day(Day(date_text)), day_delta(0) {
}

Day DDay::get_day() const {
    return this->day;
}

int DDay::get_day_delta() const {
    return this->day_delta;
}

void DDay::tomorrow() {
    ++this->day;
}

void DDay::yesterday() {
    --this->day;
}

void DDay::set_dday(int day_delta) {
    this->day_delta = day_delta;
}

Day DDay::calc_dday() const {
    return this->day + this->day_delta;
}

std::ostream &operator<<(std::ostream &output_stream, const DDay &dday) {
    char dday_prefix = (dday.get_day_delta() >= 0) ? '+' : '\0';

    output_stream << dday.get_day();
    output_stream << " [D-day:" << dday_prefix << dday.get_day_delta() << "] ";
    output_stream << dday.calc_dday();

    return output_stream;
}
