#include <chrono>
#include <iostream>
#include "DateCalculator.h"
#include "DDay.h"
#include "Day.h"

using namespace std;

int main() {
    Day d(43095, 5, 10);
    // Day d(2003, 12, 31);
    // Day d2(2004, 1, 1);
    // Day d3(2004, 12, 31);
    // Day d4(2005, 1, 1);
    // Day d5(2005, 5, 1);
    auto current = std::chrono::system_clock::now();
    // d = d + 0 ;
    // d2 = d2 + 0 ;
    // d3 = d3 + 0 ;
    // d4 = d4 + 0 ;
    // d5 = d5 + 0 ;
    //
    // cout << d << endl;
    // cout << d2 << endl;
    // cout << d3 << endl;
    // cout << d4 << endl;
    // cout << d5 << endl;

    for (int i = 1; i < 30000000; ++i) {
        d = d + i * (!(i & 1) - (i & 1));

        if ((i & 0x000fffff) == 1) {
            std::cout << i << "-th iteration | " << d << std::endl;
        }
    }
    auto elapsed = std::chrono::system_clock::now() - current;
    // expected 2026/10/01
    std::cout << d << " | " << std::chrono::duration_cast<std::chrono::duration<float> >(elapsed).count() << "sec" <<
            std::endl;
}