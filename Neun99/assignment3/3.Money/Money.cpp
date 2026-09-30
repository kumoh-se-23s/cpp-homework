#include <iostream>
#include <string>
#include <sstream>
#include "Money.h"

using namespace std;

//생성자---------------
Money::Money() {
    dollar = 0;
    cent = 0;
}

Money::Money(int newDollar, int newCent): dollar(newDollar), cent(newCent) {
    normalize(dollar, cent);
}

//getter-----------------
int Money::getDollar() const {
    return dollar;
}
int Money::getCent() const {
    return cent;
}

//setter-----------------
int Money::setDollar(int newDollar) {
    dollar = newDollar;
}

void Money::setCent(int newCent) {
    cent = newCent
}

//연산자 오버로딩-----------------
//+
const Money Money::operator+(const Money& money) const {
    return Money(dollar + money.getDollar(), cent + money.getCent());
}

//단항-
const Money Money::operator-() const{
    return Money(-dollar, -cent);
}

//이항-
const Money Money::operator-(const Money& money) const {
    return Money(dollar - money.getDollar(), cent - money.getCent());
}

//==
bool Money::operator==(const Money& money) const {
    return dollar == money.getDollar() && cent == money.getCent();
}

//!=
bool Money::operator!=(const Money& money) const {
    return dollar != money.getDollar() || cent != money.getCent();
}

//<=
bool Money::operator<=(const Money& money) const {
    if (dollar <= money.getDollar())
        return true;
    return cent <= money.getCent();
}

//>=
bool Money::operator>=(const Money& money) const {
    if (dollar >= money.getDollar())
        return true;
    return cent >= money.getCent();
}

//<
bool Money::operator<(const Money& money) const {
    if (dollar < money.getDollar())
        return true;
    return cent < money.getCent();
}

//>
bool Money::operator>(const Money& money) const {
    if (dollar > money.getDollar())
        return true;
    return cent > money.getCent();
}

//toString
string Money::toString() const{
    ostringstream result;
    if (dollar < 0)
        result << "-";
    result << "$" << abs(dollar) << cent;

    return result.str();
}

//in
istream& operator>>(istream& in, Money& money) {
    int dollar, cent;
    in >> dollar;
    in >> cent;

    money.setDollar(dollar);
    money.setCent(cent);

    return in;
}

//out
ostream& operator<<(ostream& out, const Money& money) {
    out << money.toString();
}

void Money::normalize(int& dollar, int& cent) {
    dollar += cent / 100;
    cent %= 100;

    //cent 음수면 양수화
    if (cent < 0) {
        --dollar;
        cent += 100;
    }
}