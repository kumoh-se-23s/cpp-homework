#pragma once
#include<iostream>

using namespace std;
class Fraction {
public:
    Fraction(int n = 1, int d = 1);
    Fraction add(const Fraction& fraction) const;
    void set(int n, int d);
    int getNumerator() const;
    int getDenominator() const;
    Fraction operator +(const Fraction& fraction) const;
    Fraction& operator =(const Fraction& fraction);
private: 
    int numerator = 1;
    int denominator = 1;
    void organizeFraction();
    static void swap(int&, int&);
    static int getGCD(int n, int d);
};

ostream& operator <<(ostream& outputStream, const Fraction& fraction);