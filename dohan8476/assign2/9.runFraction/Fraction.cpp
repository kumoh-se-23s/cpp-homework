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
        numerator = num;
        den = 1;
    }
    if (den < 0) {
        num = -1 * num;
        den = -1 * den;
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
    int g = gcd(numerator, denominator);
    numerator = numerator / g;
    denominator = denominator / g;
}

int Fraction::gcd(int num, int den) {
    if (num < 0) num = -num;
    if (den < 0) den = -den;

    int max, min;
    if (num < den) {
        max = den;
        min = num;
    }else {
        max = num;
        min = den;
    }
    while (min != 0) {
        int res = max % min;
        max = min;
        min = res;
    }
    return max;
}
