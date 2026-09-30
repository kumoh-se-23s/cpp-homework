#include "Money.h"
#include<iostream>

using namespace std;

Money::Money() = default;

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

int Money::getCent() const {
    return cent;
}

int Money::getDollar() const {
    return dollar;
}

Money Money::add(const Money& money) const{
    int resultCent = this->getCent() + money.getCent();
    int resultDollar = this->getDollar() + money.getDollar();

    Money resultMoney = Money(resultDollar, resultCent);
    return resultMoney;
}
Money Money::minus(const Money& money) const{
    int resultCent = this->getCent() - money.getCent();
    int resultDollar = this->getDollar() - money.getDollar();

    Money resultMoney = Money(resultDollar, resultCent);
    return resultMoney;
}
void Money::set(int dollar, int cent) {
    this->dollar = dollar;
    this->cent = cent;
    this->normalize();
}

const Money Money::operator +(const Money& Money) const {
    return add(Money);
}

const Money Money::operator -(const Money& Money) const {
    return minus(Money);
}

bool Money::operator ==(const Money& money) const {
    return this->getDollar() == money.getDollar() && this->getCent() == money.getCent();
}

bool Money::operator <(const Money& money) const
{
    if (this->getDollar() > money.getDollar()) {
        return false;
    } else if (this->getDollar() == money.getDollar() && this->getCent() >= money.getCent()) {
        return false;
    } else {
        return true;
    }
}

bool Money::operator >(const Money& money) const {
    if (this->getDollar() < money.getDollar()) {
        return false;
    } else if (this->getDollar() == money.getDollar() && this->getCent() <= money.getCent()) {
        return false;
    } else {
        return true;
    }
}
bool Money::operator >=(const Money& money) const {
    if (this->getDollar() < money.getDollar()) {
        return false;
    } else if (this->getDollar() == money.getDollar() && this->getCent() < money.getCent()) {
        return false;
    } else {
        return true;
    }
}

bool Money::operator <=(const Money& money) const {
    if (this->getDollar() > money.getDollar()) {
        return false;
    } else if (this->getDollar() == money.getDollar() && this->getCent() > money.getCent()) {
        return false;
    } else {
        return true;
    }
}
Money& Money::operator =(const Money& money) {
    set(money.getDollar(), money.getCent());
    return *this;
}

string Money::toString() const {
    string result = "";
    if (dollar < 0) {
        result += "-";
    }
    result = result + "$" + to_string(abs(dollar)) + ".";
    if (abs(cent) < 10)
    {
        result += "0";
    }
    result.append(to_string(abs(cent)));
    return result;
}

ostream& operator <<(ostream& out, const Money& money) {
    out << money.toString();
    return out;
}
istream& operator >>(istream& in, Money& money){
    int dollar, cent;
    in >> dollar >> cent;
    money.set(dollar, cent);
    return in;
}
