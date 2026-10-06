#include "Day.h"

int main() {
    Day day1(2023, 3, 1);
    Day day2(2023, 3, 1);
    Day day3(2023, 3, 1);
    Day day4(2026, 1, 31);

    int plus = 366;
    int minus = plus;

    cout << "++: ";
    for (int idx = 0; idx < plus; idx++) {
        ++day1;
    }
    cout << day1 << endl;

    cout << "+ : ";
    day2 = day2 + plus;
    cout << day2 << endl;

    cout << "--: ";
    for (int idx = 0; idx < minus; idx++) {
        --day3;
    }
    cout << day3 << endl;

    cout << "- : ";
    day4 = day4 - minus;
    cout << day4;

    return 0;
}

