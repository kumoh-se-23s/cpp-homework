#include <iostream>
#include "Money.h"
#include <iomanip>

using namespace std;

//생성자---------------
Money::Money() {
    dollar = 0;
    cent = 0;
}

Money::Money(int newDollar, int newCent) {
    dollar = newDollar;
    cent = newCent;
    normalize();
}

//getter-----------------
int Money::getDollar() const {
    return dollar;
}
int Money::getCent() const {
    return cent;
}

//setter-----------------
void Money::setValue(int newDollar, int newCent) {
    dollar = newDollar;
    cent = newCent;
    normalize();
}

//연산자 오버로딩-----------------
//+
const Money Money::operator+(const Money& money2) const {
    return Money(dollar + money2.dollar, cent + money2.cent);
}

//단항-
const Money Money::operator-() const{
    return Money(-dollar, -cent);
}

//이항-
const Money Money::operator-(const Money& money2) const {
    return Money(dollar - money2.dollar, cent - money2.cent);
}

//==
bool Money::operator==(const Money& money2) const {
    return dollar == money2.dollar && cent == money2.cent;
}

//!=
bool Money::operator!=(const Money& money2) const {
    return !(dollar == money2.dollar && cent == money2.cent);
}

//<=
bool Money::operator<=(const Money& money2) const {
    if (dollar == money2.dollar)
        return cent <= money2.cent;
    return dollar < money2.dollar;
}

//>=
bool Money::operator>=(const Money& money2) const {
    if (dollar == money2.dollar)
        return cent >= money2.cent;
    return dollar > money2.dollar;
}

//<
bool Money::operator<(const Money& money2) const {
    if (dollar == money2.dollar)
        return cent < money2.cent;
    return dollar < money2.dollar;
}

//>
bool Money::operator>(const Money& money2) const {
    if (dollar == money2.dollar)
        return cent > money2.cent;
    return dollar > money2.dollar;
}

//in
istream& operator>>(istream& in, Money& money) {
    int dollar, cent;
    in >> dollar;
    in >> cent;

    money.setValue(dollar, cent);

    return in;
}

//out
ostream& operator<<(ostream& out, const Money& money) {
    if (money.getDollar() < 0 || (money.getDollar() == 0 && money.getCent() < 0 )) //달러가 음수거나, 달러가 0인데 센트가 음수
        out << "-";
    out << "$" << abs(money.getDollar()) << ".";

    out << setfill('0') << setw(2) << abs(money.getCent()) << setfill(' ');
    return out;
}

void Money::normalize() {
    dollar += cent / 100;
    cent %= 100;

    //한쪽만 음수면 달러에 맞춰 통일
    if (dollar > 0 && cent < 0) { //달러 양수, 센트 음수
        --dollar;
        cent += 100;
    } else if (dollar < 0 && cent > 0) { //달러 음수, 센트 양수
        ++dollar;
        cent = -(100 - cent);
    }
}