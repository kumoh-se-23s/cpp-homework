#include "Fraction.h"
#include <iostream>

Fraction::Fraction(const int32_t a, const int32_t b)
{
    set(a, b);
    
}

void Fraction::set(const int32_t newA, int32_t newB)
{
   
    if(newA == 0){
        newB = 1;
        return;
    }
    if(newB == 0) {
        using namespace std;
        cout << "ERR" << endl;
        newB = 1;
    }



    uint32_t absA = std::abs(newA);
    uint32_t absB = std::abs(newB);
    uint32_t gcdResult = gcd(absA, absB);
    

    int32_t sgn = newA * newB;
    a = sgn < 0 ? -absA / gcdResult : absA / gcdResult;
    b = absB / gcdResult;
}

Fraction Fraction::add(const Fraction other)
{ // 16바이트까진 복사가 빠름
    int64_t ma = a * other.b + b * other.a;
    int64_t mb = b * other.b;
    return Fraction(ma, mb);
}

Fraction Fraction::mul(const Fraction other)
{ // 16바이트까진 복사가 빠름
    int64_t ma = a * other.a;
    int64_t mb = b * other.b;
    return Fraction(ma, mb);
}

void Fraction::print() const
{
    using namespace std;
    cout << a << "/" << b << endl;
}
uint32_t Fraction::gcd(const uint32_t m, const uint32_t n)
{
    uint32_t min = std::min(m, n);
    uint32_t max = std::max(m, n);

    while (min != 0)
    {
        uint32_t r = max % min;
        max = min;
        min = r;
    }
    return max;
}
