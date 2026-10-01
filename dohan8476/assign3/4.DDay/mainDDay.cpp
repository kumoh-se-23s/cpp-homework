#include <iostream>
#include "DateCalculator.h"

using namespace std;

int main() {
    DateCalculator dateCalculator;
    int num = dateCalculator.dateToTotalDays(1999, 12, 31);
    cout << num;
}