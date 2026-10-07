#pragma once
#include <iostream>

using namespace std;

class Money {
public:
    //생성자
    Money();
    Money(int, int);

    //getter setter
    int getDollar() const;
    int getCent() const;
    void setValue(int, int);

    //연산자 오버로딩
    const Money operator+(const Money&) const;
    const Money operator-() const;
    const Money operator-(const Money&) const;
    bool operator==(const Money&) const;
    bool operator!=(const Money&) const;
    bool operator>=(const Money&) const;
    bool operator<=(const Money&) const;
    bool operator>(const Money&) const;
    bool operator<(const Money&) const;

private:
    int dollar;
    int cent;
    void normalize();
};

ostream& operator<<(ostream&, const Money&);
istream& operator>>(istream&, Money&);