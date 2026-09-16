#include<iostream>

using namespace std;

int main() {
    const int maxLength = 3;
    int baseNum, multiNumber;
    cin >> baseNum;
    cin >> multiNumber;

    int multiNumberArray[maxLength];

    for (int cnt = 0; cnt < maxLength; ++cnt) {
        multiNumberArray[cnt] = multiNumber % 10;
        multiNumber /= 10; 
    }

    int result = 0, positionValue = 1;
    int resultNumberArray[maxLength];

    for (int arrayIndex = 0; arrayIndex < maxLength; ++arrayIndex) {
        resultNumberArray[arrayIndex]= multiNumberArray[arrayIndex] * baseNum ;
        result += positionValue * resultNumberArray[arrayIndex];
        positionValue *= 10;
    }
    
    for (int arrayIndex = 0; arrayIndex < maxLength; ++arrayIndex) {
        cout << resultNumberArray[arrayIndex] << endl;
    }
    cout << result << endl;

    return 0;
}