#include <ostream>
#include "Utils.h"

Utils::Utils() {

}

bool Utils::isDDay(char inputCharArr[], int length) {
        for(int i = 0 ; i < length ; i++) {
        if(inputCharArr[i] == '+' || inputCharArr[i] == '-') {
            return true;
        } 
    }
    return false;
}

bool Utils::isValidInput(char inputCharArr[]) {
    bool isValid = true;
    int index{0};

    while(inputCharArr[index] != '\0') {
        if(index == 0 && inputCharArr[index + 1] == '\0' && (toupper(inputCharArr[index]) == 'T' || toupper(inputCharArr[index]) == 'Y' ||  toupper(inputCharArr[index]) == 'Q')) {
            return true;
        }
        else if(inputCharArr[index] - '0' >= 0 && inputCharArr[index] - '9' <= 0) {
            index = index + 1;
            isValid = true;
        }
        else if(index == 0 && (inputCharArr[index] == '+' || inputCharArr[index] == '-' ) && (inputCharArr[index + 1] - '0' >= 0 && inputCharArr[index + 1] - '9' <= 0)) {
            index = index + 1;
            isValid = true;
        } 
        else {
            return false;
        }
    }
    return isValid;
}

int Utils::charArrParser(char inputCharArr[], int arrLength) {
    int digitIndex{1};
    int result{0};

    for(int index = arrLength - 1 ; index >= 0 ; index--) {
        if(inputCharArr[index] == '-') {
            return -result;
        } else if (inputCharArr[index] == '+') {
            return +result;
        }

        if(inputCharArr[index] != '\0') {
            result = result + (inputCharArr[index] - '0') * digitIndex;
            digitIndex = digitIndex * 10;
        }
    }

    return result;
}

ostream& operator<<(ostream& outputStream, const Day& day) {
    outputStream << day.getYear() << "/" ;
    if(day.getMonth() < 10) {
        outputStream << "0" << day.getMonth() << "/"; 
    }
    else {
        outputStream << day.getMonth() << "/"; 
    }
    
    if(day.getDay() < 10) {
        outputStream << "0" << day.getDay();
    }
    else {
        outputStream << day.getDay();
    }
    
    
    return outputStream;
}

ostream& operator<<(ostream& outputStream, const DDay& dday) {
    if(dday.getDDay() >= 0) {
        outputStream << " [D-day:+" << dday.getDDay() << "] ";
    }
    else {
        outputStream << " [D-day:" << dday.getDDay() << "] ";
    }

    
    return outputStream;  
}