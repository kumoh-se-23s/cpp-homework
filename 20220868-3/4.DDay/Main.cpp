#include <chrono>
#include <iostream>
#include "Day.h"


bool stoui(const char *input, int len, int &result) {
    result = 0;
    for (int i = 0; i < len && input[i] != '\0'; ++i) {
        int num = input[i] - '0';
        if (num < 0 || num > 9) return false;
        result *= 10;
        result += num;
    }
    return true;
}

void printErr() {
    using namespace std;
    cout << "*** ERROR" << std::endl;
    //?
}


int main() {
    Day d(2026, 10, 1);
    auto current = std::chrono::system_clock::now();
    for (int i = 0; i < 30000000; ++i) {
        d = d + (d.getYear() * d.getMonth() + d.getDay() ) * (!(i & 1) - (i & 1));

        if ((i & 0x000fffff) == 0) {
            std::cout << i << "-th iteration | " << d << std::endl;
        }
    }
    auto elapsed = std::chrono::system_clock::now() - current;
    // expected 2001/10/07
    std::cout << d << " | " << std::chrono::duration_cast<std::chrono::duration<float> >(elapsed).count() << "sec" <<
            std::endl;


    using namespace std;
    Day day;
    char input[9]{};
    int delta = 0;
    Day targetDay;

    while (input[0] != 'Q' && input[0] != 'q') {
        cout << day << "[D-day:" << (delta >= 0 ? "+" : "") << delta << "]" << targetDay << std::endl;
        cout << "<< Move Date(yyyymmdd, Tomorrow(T/t), Yesterday(Y/y), Set D-day(+/-int), or Quit(Q/q)) : ";
        cin >> input;

        switch (input[0]) {
            case '0':
                targetDay = day;
                delta = 0;
                break;
            case 'y':
            case 'Y':
                --day;
                --targetDay;
                break;
            case 'T':
            case 't':
                ++day;
                ++targetDay;
                break;
            case '+':
            case '-':
                if (!stoui(input + 1, 8, delta)) {
                    printErr();
                    break;
                }
                if (input[0] == '-') delta = -delta;
                targetDay = day + delta;
                break;
            case 'q':
            case 'Q':
                break;
            default:
                int y;
                int m;
                int d;
                if (!stoui(input, 4, y)) {
                    printErr();
                    break;
                }
                if (!stoui(input + 4, 2, m)) {
                    printErr();
                    break;
                }
                if (!stoui(input + 6, 2, d)) {
                    printErr();
                    break;
                }
                if (!Day::isValid(y, m, d)) {
                    printErr();
                    break;
                }

                day = Day(y, m, d);
                targetDay = day + delta;
                break;
        }
    }

    cout << "=== END ===" << endl;
    return 0;
}
