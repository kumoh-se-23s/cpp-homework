#include<iostream>
using namespace std;


int main() {
    srand((unsigned int)time(NULL)); 
    //srand 사용하지 않을 시 랜덤값이 42로 고정됨

    cout << "I have specified one natural number less than or equal to 100." << endl;

    int userGuess = -1;
    int goalNumber = rand() % 100 + 1;
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

