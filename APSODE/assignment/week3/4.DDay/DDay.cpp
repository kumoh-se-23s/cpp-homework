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

void DDay::tomorrow() {
    ++this->day;
}

void DDay::yesterday() {
    --this->day;
}

void DDay::set_dday(int day_delta) {
    this->day_delta = day_delta;
}

Day DDay::calc_dday() {
    Day result = this->day;
    return result + this->day_delta;
}
