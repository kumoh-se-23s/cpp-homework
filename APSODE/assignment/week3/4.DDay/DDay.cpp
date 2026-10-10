//
// Created by leegu on 26. 10. 7..
//

#include "DDay.h"
#include "Day.h"


DDay::DDay()
    : day(Day()), day_delta(0) {
}

DDay::DDay(const Day manual_day)
    : day(manual_day), day_delta(0) {
}

DDay::DDay(const char date_text[])
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

void DDay::set_new_day(const Day new_day) {
    this->day = new_day;
}

std::ostream &operator<<(std::ostream &output_stream, const DDay &dday) {
    output_stream << dday.get_day();
    output_stream << " [D-day:";
    if (dday.get_day_delta() >= 0) {
        output_stream << '+';
    }
    output_stream << dday.get_day_delta() << "] ";
    output_stream << dday.calc_dday();

    return output_stream;
}
