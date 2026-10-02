#include <chrono>
#include <iostream>
#include "DateCalculator.h"
#include "DDay.h"
#include "Day.h"

using namespace std;

int main() {
    Day d(43095, 5, 10);
    auto current = std::chrono::system_clock::now();
    for (int i = 0; i < 30000000; ++i) {
        d = d + i * (!(i & 1) - (i & 1));

        // if ((i & 0x000fffff) == 1) {
            if (!DateCalculator::isValidDate(d.getYear(), d.getMonth(), d.getDay())) {
                std::cout << i << "-th iteration | " << d << std::endl;
            // }
        }
    }
        auto elapsed = std::chrono::system_clock::now() - current;
        // expected 2026/10/01
        std::cout << d << " | " << std::chrono::duration_cast<std::chrono::duration<float> >(elapsed).count() << "sec" <<
                std::endl;
    }
