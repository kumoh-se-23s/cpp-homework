#include "Day.h"

const short Day::dayOfMonth[2][13] = {
            {0,31,28,31,30,31,30,31,31,30,31,30,31}, //평년
            {0,31,29,31,30,31,30,31,31,30,31,30,31}, //윤년
        };

Day::Day() {
    year = 2026;
    month = 10;
    day = 1;
}

Day::Day(int inputYear, int inputMonth, int inputDay) {
    year = inputYear;
    month = inputMonth;
    day = inputDay;
}

int Day::getYear() const {
    return year;
}

int Day::getMonth() const {
    return month;
}


int Day::getDay() const {
    return day;
}

bool Day::isLeapYear(int inputYear) const {
    return !((inputYear % 4 != 0) | ((inputYear % 400 != 0) ^ (inputYear % 100 != 0)));
}

bool Day::isValid(int inputYear, int inputMonth, int inputDay) const {
    if(inputMonth >= 1 && inputMonth <= 12 && dayOfMonth[isLeapYear(inputYear)][inputMonth] >= inputDay && inputDay >= 1 ) {
        return true;
    } 
    else {
        return false;
    }
}

bool Day::setDay(int inputYear, int inputMonth, int inputDay) {
    if(isValid(inputYear, inputMonth, inputDay)) {
        year = inputYear;
        month = inputMonth;
        day = inputDay;

        return true;
    } 
    return false;
}

Day Day::operator+(int inputDDay) const {
    int leafYearNum = inputDDay / 146097;

    int totalDay{inputDDay + day - (leafYearNum * 146097)};

    for(int index = 1 ; index < month ; index++) {
        totalDay = totalDay + dayOfMonth[isLeapYear(year)][index];
    }
    
    int resultYear{year + leafYearNum * 400};
    int resultMonth{1};
    int resultDay{0};

    while(totalDay > 0) {
        if(totalDay > (365 + isLeapYear(resultYear))) {
            totalDay = totalDay - 365 - isLeapYear(resultYear);
            resultYear = resultYear + 1;
        } 
        else {
            if(totalDay > dayOfMonth[isLeapYear(resultYear)][resultMonth]) {
                totalDay = totalDay - dayOfMonth[isLeapYear(resultYear)][resultMonth];
                resultYear = resultYear + resultMonth / 12;
                resultMonth = resultMonth % 12 + 1;
            }
            else {
                resultDay = totalDay;
                totalDay = 0;
            }
        }
    }

    return Day(resultYear, resultMonth, resultDay);
}

Day Day::operator-(int inputDDay) const {
    inputDDay = inputDDay < 0 ? -inputDDay : inputDDay;

    int leafYearNum = inputDDay / 146097;

    int totalDay{inputDDay - day - (leafYearNum * 146097)};

    for(int index = month ; index <= 12 ; index++) {
        totalDay = totalDay + dayOfMonth[isLeapYear(year)][index];
    }
    
    int resultYear{year - leafYearNum * 400};
    int resultMonth{12};
    int resultDay{31};

    while(totalDay > 0) {
        if(totalDay > (365 + isLeapYear(resultYear))) {
            totalDay = totalDay - 365 - isLeapYear(resultYear);
            resultYear = resultYear - 1;
        } 
        else {
            if(totalDay > dayOfMonth[isLeapYear(resultYear)][resultMonth]) {
                totalDay = totalDay - dayOfMonth[isLeapYear(resultYear)][resultMonth];
                resultYear = resultYear - (resultMonth == 0);
                resultMonth = resultMonth == 0 ? 12 : resultMonth - 1;
            }
            else {

                if(totalDay == dayOfMonth[isLeapYear(resultYear)][resultMonth] && resultMonth == 1) {
                    resultYear = resultYear - 1;
                    resultMonth = 12;
                    resultDay = dayOfMonth[isLeapYear(resultYear)][resultMonth];
                }
                else if(totalDay == dayOfMonth[isLeapYear(resultYear)][resultMonth]) {
                    resultMonth = resultMonth - 1;
                    resultDay = dayOfMonth[isLeapYear(resultYear)][resultMonth];
                }
                else {
                    resultDay = dayOfMonth[isLeapYear(resultYear)][resultMonth] - totalDay;
                }
                totalDay = 0;
            }
        }
    }

    return Day(resultYear, resultMonth, resultDay);
}

//분기를 줄이기 위해 밑과 같은 방식으로 작성

void Day::operator++() {
    int tempMonth = month;
        
    year = year + (month == 12 && dayOfMonth[isLeapYear(year)][month] == day);
    month = (month - 1 + (dayOfMonth[isLeapYear(year)][month] == day)) % 12 + 1;
    day = day % dayOfMonth[isLeapYear(year)][tempMonth] + 1; 
}

void Day::operator--() {
        year = year - (month == 1 && day == 1);
        month = month - (day == 1) + (month == 1 && day == 1) * 12; 
        day = (day - 1) + (day == 1) * dayOfMonth[isLeapYear(year)][month];
}