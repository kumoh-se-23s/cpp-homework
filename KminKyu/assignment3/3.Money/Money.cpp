#include "Money.h"

#include <iomanip>
#include<iostream>

using namespace std;

Money::Money() : dollar(0), cent(0) {}

Money::Money(int dollar, int cent) {
    set(dollar, cent);
}

void Money::normalize() {
    dollar += cent / 100;
    cent %= 100;
    if (dollar * cent < 0) {
        int regrouping = dollar / abs(dollar) * 100;
        dollar -= regrouping / 100;
        cent += regrouping;
    }
}

Money Money::add(const Money& money) const{
    int resultCent = this->cent + money.cent;
    int resultDollar = this->dollar + money.dollar;

    Money resultMoney = Money(resultDollar, resultCent);
    return resultMoney;
}
Money Money::minus(const Money& money) const{
    int resultCent = this->cent - money.cent;
    int resultDollar = this->dollar - money.dollar;

    Money resultMoney = Money(resultDollar, resultCent);
    return resultMoney;
}
void Money::set(int dollar, int cent) {
    this->dollar = dollar;
    this->cent = cent;
    this->normalize();
}

int Money::getDollar() const {
    return this->dollar;
}

int Money::getCent() const {
    return this->cent;
}

Money Money::operator +(const Money& Money) const {
    return add(Money);
}

Money Money::operator -(const Money& Money) const {
    return minus(Money);
}

bool Money::operator ==(const Money& money) const {
    return this->dollar == money.dollar && this->cent == money.cent;
}

bool Money::operator <(const Money& money) const
{
    if (this->dollar > money.dollar) {
        return false;
    } else if (this->dollar == money.dollar && this->cent >= money.cent) {
        return false;
    } else {
        return true;
    }
}

bool Money::operator >(const Money& money) const {
    if (this->dollar < money.dollar) {
        return false;
    } else if (this->dollar == money.dollar && this->cent <= money.cent) {
        return false;
    } else {
        return true;
    }
}
bool Money::operator >=(const Money& money) const {
    if (this->dollar < money.dollar) {
        return false;
    } else if (this->dollar == money.dollar && this->cent < money.cent) {
        return false;
    } else {
        return true;
    }
}

bool Money::operator <=(const Money& money) const {
    if (this->dollar > money.dollar) {
        return false;
    } else if (this->dollar == money.dollar && this->cent > money.cent) {
        return false;
    } else {
        return true;
    }
}
Money& Money::operator =(const Money& money) {
    set(money.dollar, money.cent);
    return *this;
}


ostream& operator <<(ostream& out, const Money& money) {
    if (money.getDollar() < 0) {
        out << "-";
    }
    out << "$" << abs(money.getDollar()) << "." << std::setfill('0') << abs(money.getCent());
    return out;
}
istream& operator >>(istream& in, Money& money){
    int dollar, cent;
    in >> dollar >> cent;
    money.set(dollar, cent);
    return in;
}
