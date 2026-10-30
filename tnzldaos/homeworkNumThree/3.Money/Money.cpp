#include "Money.h"
#include <string>
using namespace std;

Money::Money() {
    dollar = 0;
    cent = 0;
}

Money::Money(int inputDollar, int inputCent) {
    dollar = inputDollar;
    cent = inputCent;
    calculateDollarCent();
}

int Money::getDollar() const {
    return dollar;
}

void Money::setDollar(int inputDollar) {
    dollar = inputDollar;
}

int Money::getCent() const {
    return cent;
}

void Money::setCent(int inputCent) {
    cent = inputCent;
}

void Money::calculateDollarCent() {
    int total = (dollar * 100) + cent; 
    dollar = total / 100;
    cent = total % 100;
}

const Money Money::operator+(const Money& money) {
    return Money(dollar + money.getDollar(), cent + money.getCent());
}

const Money Money::operator-(const Money& money) {
    return Money(dollar - money.getDollar(), cent - money.getCent());
}

bool Money::operator==(const Money& money) {
    return (dollar * 100 + cent) == money.getDollar() * 100 + money.getCent();
}

bool Money::operator!=(const Money& money) {
    return !((dollar * 100 + cent) == money.getDollar() * 100 + money.getCent());
}
    
//4.1 3.2
bool Money::operator>=(const Money& money) {
    if(dollar > money.getDollar()) {
        return true;
    } 
    else if (dollar < money.getDollar()) {
        return false;
    } 
    else {
        if(cent >= money.getCent()) {
            return true;
        } else {
            return false;
        }
    }
}

bool Money::operator<=(const Money& money) {
    if(dollar < money.getDollar()) {
        return true;
    } 
    else if(dollar > money.getDollar()) {
        return false;
    } 
    else {
        if(cent <= money.getCent()) {
            return true;
        } else {
            return false;
        }
    }
}

bool Money::operator>(const Money& money) {
    if(dollar > money.getDollar()) {
        return true;
    } 
    else if (dollar < money.getDollar()) {
        return false;
    } 
    else {
        if(cent > money.getCent()) {
            return true;
        } else {
            return false;
        }
    }
}

bool Money::operator<(const Money& money) {
    if(dollar < money.getDollar()) {
        return true;
    } 
    else if(dollar > money.getDollar()) {
        return false;
    } 
    else {
        if(cent < money.getCent()) {
            return true;
        } else {
            return false;
        }
    }
}

ostream& operator<<(ostream& outputStream, const Money& money) {

    if(money.getDollar() < 0 || money.getCent() < 0) {
        outputStream << "-$" << -(money.getDollar()) << ".";
    } else {
        outputStream << "$" << (money.getDollar()) << ".";
    }

    if(money.getCent() < 0) {
        outputStream << -money.getCent() / 10 << -money.getCent() % 10;
    } else {
        outputStream << money.getCent() / 10 << money.getCent() % 10;
    }

    return outputStream;
}

istream& operator>>(istream& inputStream, Money& money) {
    int dollar{0};
    int cent{0};

    inputStream >> dollar;
    inputStream >> cent;

    money.setDollar(dollar);
    money.setCent(cent);
    money.calculateDollarCent();

    return inputStream;
}
