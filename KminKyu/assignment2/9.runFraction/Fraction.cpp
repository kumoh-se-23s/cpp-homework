#include<iostream>
#include "Fraction.h"

using namespace std;

int getGCD(int n, int d) {
    if (d < n) {
        swap(d, n);
    }
    int temp;
    while (n != 0) {
        temp = n;
        n = d % n;
        d = temp;
    }
    return d;
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

Fraction::Fraction(int n, int d)
    : numerator(n), denominator(d) {
    organizeFraction();
}

void Fraction::organizeFraction() {
    if (denominator == 0) {
        denominator = 1;
        cout << "ERR";
    }
    int gcd = getGCD(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;

    if (denominator < 0) {
        denominator *= -1;
        numerator *= -1;
        return;
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
    cout << endl;
}

Fraction Fraction::add(Fraction fraction) {
    int firstNumerator = this->getNumerator() * fraction.getDenominator();
    int secondNumerator = fraction.getNumerator() * this->getDenominator();

    int resultDenominator = fraction.getDenominator() * this->getDenominator();

    int resultNumerator = firstNumerator + secondNumerator;

    return Fraction(resultNumerator, resultDenominator);
}
void Fraction::set(int n, int d) {
    numerator = n;
    denominator = d;
    this->organizeFraction();
}
