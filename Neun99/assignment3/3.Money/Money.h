#pragma once
#include <iostream>

using namespace std;

class Money {
public:
    //생성자
    Money();
    Money(int newDollar, int newCent);

    //getter setter
    int getDollar() const;
    int getCent() const;
    void setValue(int newDollar, int newCent);

    //연산자 오버로딩
    const Money operator+(const Money& money2) const;
    const Money operator-() const;
    const Money operator-(const Money& money2) const;
    bool operator==(const Money& money2) const;
    bool operator!=(const Money& money2) const;
    bool operator>=(const Money& money2) const;
    bool operator<=(const Money& money2) const;
    bool operator>(const Money& money2) const;
    bool operator<(const Money& money2) const;

private:
    int dollar;
    int cent;
    void normalize();
};

ostream& operator<<(ostream& out, const Money& money);
istream& operator>>(istream& in, Money& money);