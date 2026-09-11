#include<iostream>

using namespace std;

int main() {
    int testCase;

    cin >> testCase;

    for (int caseNum  = 0; caseNum < testCase; ++caseNum) {
        double value;
        cin >> value;

        int integerValue = static_cast<int>(value);
        double floatingPointValue = value - integerValue;

        cout << integerValue << " + " << floatingPointValue << endl;
    
    }

    return 0;
}