#include <iostream>
#include "DateCalculator.h"

using namespace std;

int main() {
    DateCalculator dateCalculator;
    int num = dateCalculator.dateToTotalDays(999999, 1, 1);
    cout << num;
}