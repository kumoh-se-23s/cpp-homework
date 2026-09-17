#include<iostream>
#include "Fraction.h"

using namespace std;

int getGCD(int n, int d) {
    if (d > n) {
        swap(d, n);
    }
    int temp;
    while (n > 0) {
        temp = n;
        n = d % n;
        d = temp;
    }
    return d;
}

void swap(int* a, int* b) {
    int* temp = a;
    a = b;
    b = temp;
}

Fraction::Fraction(int n, int d)
    : numerator(n), denominator(d) {
    organizeFraction();
}

void Fraction::organizeFraction() {
    if (denominator < 0) {
        denominator *= -1;
        numerator *= -1;
    } else if (denominator == 0) {
        denominator = 1;
        cout << "ERR";
    }
    int gcd = getGCD(numerator, denominator);
    if (gcd != 1) {
        numerator /= gcd;
        denominator /= gcd;
    }

}

int Fraction::getNumerator() const {
    return numerator;
}

int Fraction::getDenominator() const {
    return denominator;
}
void Fraction::print() const {
    cout << numerator;
    if (denominator != 1) {
        cout << "/" << denominator;
    }
}

Fraction Fraction::add(Fraction fraction) {
    int resultNumerator, resultDenominator;
    int otherNumerator, otherDenominator;

    otherNumerator = fraction.getNumerator();
    otherDenominator = fraction.getDenominator();

    int divNumDenominator = getGCD(otherNumerator, denominator);
    int divNumNumerator = getGCD(numerator, otherDenominator);

    resultNumerator = numerator / divNumNumerator + otherNumerator / divNumDenominator;
    resultDenominator = denominator / divNumDenominator + otherDenominator / divNumNumerator;

    return Fraction(resultNumerator, resultDenominator);
}
void Fraction::set(int n, int d) {
    numerator = n;
    denominator = d;
    this->organizeFraction();
}
