#include "DDay.h"

DDay::DDay(int inputDDay) {
    dday = inputDDay;
}

int DDay::getDDay() const {
    return dday;
}

void DDay::setDDay(int inputDDay) {
    dday = inputDDay;
}

Day DDay::getDate() const {
    return currentDay;
}

bool DDay::setDate(int inputYear, int inputMonth, int inputDay) {
    return currentDay.setDay(inputYear, inputMonth, inputDay);
}

Day DDay::plusDDay() const {
    Day resultDay = currentDay + dday;
    return resultDay;
}

Day DDay::minusDDay() const {
    Day resultDay = currentDay - dday;
    return resultDay;
}

void DDay::moveTomorrow() {
    ++currentDay;
}

void DDay::moveYesterday() {
    --currentDay;
}