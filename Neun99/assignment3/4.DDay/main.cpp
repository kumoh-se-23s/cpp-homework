#include "Day.h"

int main() {
    Day day;

    cout << day << endl;

    if (day.setValue(2030, 100, 30))
        cout << day;
    else
        cout << "shit";
    cout << endl;

    ++day;
    cout << day << endl;
    --day;
    --day;
    --day;
    cout << day << endl;

    return 0;
}

