#include "Fraction.h"

#include <iostream>
using namespace std;

Fraction::Fraction()
{
    denominator = 1;
    numerator = 1;
}
Fraction::Fraction(int n, int d)
{
    set(n, d);
}
int Fraction::getDenominator()
{
    return denominator;
}
int Fraction::getNumerator()
{
    return numerator;
}
void Fraction::set(int n, int d)
{
    numerator = n;
    if (d == 0)
    {
        cout << "ERR" << endl;
        denominator = 1;
    }
    else
        denominator = d;

    // 분자 음수, 분모 음수 || 분자 양수, 분모 음수
    if (denominator < 0)
    {
        numerator = -numerator;
        denominator = -denominator;
    }
    
    simplify();
}

// 유클리드 호제법 알고리즘
int Fraction::getGcd(int a, int b)
{
    while (b != 0)
    {
        int remain = a % b;
        a = b;
        b = remain;
    }
    return a;
}

// 약분
void Fraction::simplify()
{
    int gcd = getGcd(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
}


void Fraction::print() const
{
    if (denominator == 1 || numerator == 0)
    {
        cout << numerator;
    }
    else
    {
        cout << numerator << "/" << denominator;
    }
}

Fraction Fraction::add(Fraction otherFraction)
{
    Fraction tempFraction = {this->numerator * otherFraction.getDenominator() +
                                 otherFraction.getNumerator() * this->denominator,
                             this->denominator * otherFraction.getDenominator()};
    return tempFraction;
}

//     void Fraction::set(int n, int d)
// {
//     numerator = n;
//     if (d == 0)
//     {
//         cout << "ERR" << endl;
//         denominator = 1;
//     }
//     else
//         denominator = d;

//     // 분자 음수, 분모 음수 || 분자 양수, 분모 음수
//     bool comingNegative = false;
//     if (denominator < 0)
//     {
//         numerator = -numerator;
//         denominator = -denominator;
//     }

//     if (numerator < 0 && denominator > 0)
//     {
//         numerator = -numerator;
//         comingNegative = true;
//     }

//     // 분자 > 분모
//     if (numerator > denominator && numerator != 0)
//     {
//         if (numerator % denominator == 0)
//         {
//             numerator /= denominator;
//             denominator = 1;
//         }
//         else
//         {
//             // 둘다 i로 모듈러 연산 결과가 0이라면 나눗셈 수행
//             for (int i = denominator; i >= 2; i--)
//             {
//                 if (denominator % i == 0 && numerator % i == 0)
//                 {
//                     denominator /= i;
//                     numerator /= i;
//                 }
//             }
//         }
//     }

//     // 분자 < 분모
//     else if (numerator < denominator && numerator != 0)
//     {
//         if (denominator % numerator == 0)
//         {
//             denominator /= numerator;
//             numerator = 1;
//         }
//         else
//         {
//             for (int i = numerator; i >= 2; i--)
//             {
//                 if (denominator % i == 0 && numerator % i == 0)
//                 {
//                     denominator /= i;
//                     numerator /= i;
//                 }
//             }
//         }
//     }

//     if (comingNegative)
//         numerator = -numerator;

//     // 분자 == 분모
//     else if (numerator == denominator)
//     {
//         numerator = 1;
//         denominator = 1;
//     }
// }