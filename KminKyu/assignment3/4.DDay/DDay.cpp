#include "DDay.h"

DDay::DDay() {
    startDay = Day();
    endDay = Day();
    dday = 0;
}

void DDay::setDDay(int dday) {
    this->endDay = this->startDay +dday;
    this->dday = dday;
}

bool DDay::setStartDay(int year, int month, int day) {
    if (Day::isCorrectDate(year, month, day)) {
        this->startDay.set(year, month, day);
        setDDay(this->dday);
       return true;
    }

    return false;
}

void DDay::setTomorrow() {
    ++this->startDay;
    ++this->endDay;
}

void DDay::setYesterday() {
    --this->startDay;
    --this->endDay;
}

std::ostream & operator<<(std::ostream &out, const DDay &dday) {
    out << "<< " << dday.getStartDay() << " [D-day:";
    if (dday.getDDay() >= 0) {
        out << "+";
    }
    out << dday.getDDay() << "] " << dday.getEndDay() << "\n";
    return out;

}

const Day& DDay::getStartDay() const {
    return startDay;
}

const Day& DDay::getEndDay() const {
    return endDay;
}

int DDay::getDDay() const {
    return dday;
}
