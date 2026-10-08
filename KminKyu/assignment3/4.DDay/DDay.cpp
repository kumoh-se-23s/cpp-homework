#include "DDay.h"

DDay::DDay() {
    startDay = Day();
    endDay = Day();
    dday = 0;
}

void DDay::setDDay(int dday) {
    this->endDay = this->startDay + dday;
    this->dday = dday;
}

bool DDay::setStartDay(int dayInt) {
    int year, month, day;
    year = dayInt / 10000;
    dayInt %= 10000;
    month = dayInt / 100;
    day = dayInt % 100;
    if (Day::isCorrectDate(year, month, day)) {
        this->startDay.set(year, month, day);
        this->endDay = this->startDay + dday;
        return true;
    } else {
        return false;
    }
}

void DDay::setTomarrow() {
    ++this->startDay;
    ++this->endDay;
}

void DDay::setYesterDay() {
    --this->startDay;
    --this->endDay;
}

std::ostream & operator<<(std::ostream &out, const DDay &dday) {
    out << "<< " << dday.getStartDay() << " [D-day:";
    if (dday.getDDay() >= 0) {
        out << "+";
    }
    out << dday.getDDay() << "] " << dday.getEndDay();

}

const Day DDay::getStartDay() const {
    return startDay;
}

const Day DDay::getEndDay() const {
    return endDay;
}

int DDay::getDDay() const {
    return dday;
}
