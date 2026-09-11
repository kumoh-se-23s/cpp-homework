#include<iostream>
using namespace std;

int getSrand() {
    srand((unsigned int)time(NULL));
    return rand() % 100 + 1;
}

int getRand() {
    return rand() % 100 + 1;
}
//srand 사용하지 않을 시 랜덤값이 고정됨

int main() {

    cout << "I have specified one natural number less than or equal to 100." << endl;

    int userGuess = -1;
    int goalNumber = getSrand();
    int tryCount;

    for (tryCount = 0; userGuess != goalNumber; ++tryCount) { 
        cout << "Guess the number and enter it : ";

        cin >> userGuess;

        if (userGuess > goalNumber) {
            cout << "The number I specified is a number less than " << userGuess << ".\n\n";

        } else if (userGuess < goalNumber) {
            cout << "The number I specified is a number greater than " << userGuess << ".\n\n" ;
        }
    }
    cout << "Got it!!! You've succeeded in the " << tryCount << "-th times!";
}   

