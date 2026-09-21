#include <iostream>
#include "Fraction.h"

using namespace std;

Fraction::Fraction() {
    numerator = 1;
    denominator = 1;
}

Fraction::Fraction(int num, int den) {
    set(num, den);
}

Fraction Fraction::add(Fraction a) {
    int num = (this->numerator * a.denominator) + (a.numerator * this->denominator);
    int den = this->denominator * a.denominator;

    return Fraction(num, den);
}

void Fraction::set(int num, int den) {
    if (den == 0 ) {
        cout << "ERR ";
        den = 1;
    }
    if (den < 0) {
        num = -num;
        den = -den;
    }

    numerator = num;
    denominator = den;

    simplify();
}

void Fraction::print() {
    cout << numerator ;
    if (denominator != 1) {
        cout << "/" << denominator;
    }
}

void Fraction::simplify() {
    int g = calcGcd(numerator, denominator);
    numerator /=  g;
    denominator /= g;
}

int Fraction::calcGcd(int num, int den) {
    num = abs(num);
    den = abs(den);

    int larger = max(num, den);
    int smaller = min(num, den);

    while (smaller != 0) {
        int res = larger % smaller;
        larger = smaller;
        smaller = res;
    }

    return larger;
}

int Fraction::abs(int num) {
    if (num < 0) num = -num;

    return num;
}

int Fraction::max(int a, int b) {
    if (a >= b) return a;

    return b;
}
int Fraction::min(int a, int b) {
    if (a <= b) return a;

    return b;
}
