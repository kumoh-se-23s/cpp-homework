#include <iostream>
#include <string>
#include <time.h>
using namespace std;

int main() {
    string inStr;
    srand((unsigned int)time(NULL));
    do {
        int randDice = rand() % 6 + 1;
        int randScore = rand() % 201 - 101;
        double randF = (RAND_MAX - rand()) / static_cast<double>(RAND_MAX);
        
        cout << "주사위 : " << randDice << endl;
        cout << "점수 : " << randScore << endl;
        cout << "실수 : " << randF << endl;
        cout << "계속하시겠습니까? (y/n) : ";
        cin >> inStr;

    } while(inStr == "y" || inStr == "Y");

    return 0;
}