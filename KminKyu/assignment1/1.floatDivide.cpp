#include<iostream>

using namespace std;

int main() {
    int testCase;

    cin >> testCase;
    double value;
    int integerValue;
    double floatingPointValue;
    
    for (int caseNum  = 0; caseNum < testCase; ++caseNum) {
        cin >> value;
        integerValue = int(value);
        floatingPointValue = value - integerValue;

        cout << integerValue << " + " << floatingPointValue << endl;
    
    }

    return 0;
}