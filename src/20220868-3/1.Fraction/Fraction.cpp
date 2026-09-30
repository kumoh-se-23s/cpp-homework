#include "Fraction.h"
#include <format>

Fraction::Fraction(const int32_t a, const int32_t b)
{
    set(a, b);
}

void Fraction::set(const int32_t newA, int32_t newB)
{
   
    if(newA == 0){
        a = 0;
        b = 1;
        return;
    }
    if(newB == 0) {
        using namespace std;
        cout << "ERR" << endl;
        newB = 1;
    }



    int32_t absA = std::abs(newA);
    int32_t absB = std::abs(newB);
    int32_t gcdResult = gcd(absA, absB);


    bool positive = (newA > 0) == (newB > 0);
    a = positive ? absA / gcdResult : -absA / gcdResult;
    b = absB / gcdResult;
}

Fraction &Fraction::operator=(const Fraction other){
    a = other.a;
    b = other.b;
    return *this;
}

Fraction Fraction::operator+(const Fraction other) const
{ // 16바이트까진 복사가 빠름
    int64_t ma = static_cast<int64_t>(a) * other.b + static_cast<int64_t>(b) * other.a;
    int64_t mb = static_cast<int64_t>(b) * other.b;
    return Fraction(ma, mb);
}

Fraction Fraction::operator*(const Fraction other) const
{ // 16바이트까진 복사가 빠름
    int64_t ma = static_cast<int64_t>(a) * other.a;
    int64_t mb = static_cast<int64_t>(b) * other.b;
    return Fraction(ma, mb);
}

std::string Fraction::toString() const
{
    using namespace std;
    if(b == 1){
        return std::to_string(a);
    }

    return std::format("{}/{}", a, b);
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



// int main()
// {
//     using namespace std;
//     Fraction f2(-53172,53214), f3 ;
//     Fraction f1(42, 53214);


//     f3 = f1 + f2;
//     cout << f1 <<  "+ " << f2 ;
//     cout << " = " << f3 << endl ;
//     return 0 ;
// }