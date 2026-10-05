#include "Money.h"

#include <sstream>

using namespace std;

Money::Money() {
    dollar = 0;
    cent = 0;
}

Money::Money(int dollar, int cent) {
    set(dollar, cent);
}

int Money::getDollar() const {
    return dollar;
}
int Money::getCent() const {
    return cent;
}

void Money::set(int dollar, int cent) {
    this->dollar = dollar;
    this->cent = cent;
    normalize();
}

void Money::normalize() {
    dollar += cent / 100;
    cent %= 100;

    if (cent < 0) {
        --dollar;
        cent += 100;
    }
}

//-----------연산자 오버로딩----------

const Money Money::operator+(const Money& m) const {
    return Money(dollar + m.dollar, cent + m.cent);
}

const Money Money::operator-(const Money& m) const {
    return Money(dollar - m.dollar, cent - m.cent);
}

bool Money::operator!=(const Money &m) {
    return dollar != m.dollar || cent != m.cent;
}

bool Money::operator==(const Money &m) const {
    return dollar == m.dollar && cent == m.cent;
}

bool Money::operator<(const Money &m) const {
    if (dollar < m.dollar)
        return true;
    return cent < m.cent;
}

bool Money::operator>(const Money &m) const {
    if (dollar > m.dollar)
        return true;
    return cent > m.cent;
}

bool Money::operator<=(const Money &m) const {
    if (dollar <= m.dollar)
        return true;
    return cent <= m.cent;
}
bool Money::operator>=(const Money &m) const {
    if (dollar >= m.dollar)
        return true;
    return cent >= m.cent;
}

string Money::toString() const {
    ostringstream out;
    if (dollar < 0) {
        out << "-";
    }
    out << "$" << abs(dollar) << "." << cent;

    return out.str();
}

int Money::abs(int num) const{
    if (num < 0) num = -num;

    return num;
}

istream& operator>>(istream& in, Money& m) {
    int dollar, cent;
    in >> dollar;
    in >> cent;

    m.set(dollar, cent);

    return in;
}
ostream& operator<<(ostream& out, const Money& m) {
    out << m.toString();
    return out;
}