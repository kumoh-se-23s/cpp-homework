#include <iostream>

using namespace std;

void randGuess();
void srandGuess();

int main () {
    cout << "I have specified one natural number less than or equal to 100." << endl;
    // randGuess();
    srandGuess();
    //rand() 와 srand()이 차이는 rand()는 실행 때 randVal의 값이 고정이였지만, (저의 경우 42 였습니다.)
    //srand()의 경우 매번 randVal의 값이 달라져서 통상적으로 생각하는 랜덤의 값의 형태를 보였습니다.
}

void randGuess() {
    int val;
    int randVal = rand() % 100 + 1;

    int cnt = 0;

    bool isRun = true;
    while (isRun) {
        cout << "Guess the number and enter it : ";
        cin >> val;
        if (randVal < val) {
            cout << "The number I specified is a number less than " << val << "." << endl;
            ++cnt;
        }
        else if (randVal > val) {
            cout << "The number I specified is a number greater than " << val << "." << endl;
            ++cnt;
        }
        else {
            cout << "Got it!!! You've succeeded in the " << ++cnt << "-th times!" << endl;
            isRun = false;
        }
    }
}

void srandGuess() {
    srand(time(0));
    int val;
    int randVal = rand() % 100 + 1;
    int cnt = 0;

    bool isRun = true;
    while (isRun) {
        cout << "Guess the number and enter it : ";
        cin >> val;
        if (randVal < val) {
            cout << "The number I specified is a number less than " << val << "." << endl;
            ++cnt;
        }
        else if (randVal > val) {
            cout << "The number I specified is a number greater than " << val << "." << endl;
            ++cnt;
        }
        else {
            cout << "Got it!!! You've succeeded in the " << ++cnt << "-th times!" << endl;
            isRun = false;
        }
    }
}