#include <iostream>
using namespace std;

void printDigit(int val);
void printReverseDigit(int val);

int main() {
    int cnt, val ;
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; i++) {
        cin >> val ;
        cout << "Backward : " ;
        printReverseDigit(val) ;
        cout << endl << "Forward : " ;
        printDigit(val) ;
        cout << endl ;
    }
    return 0 ;
}

void printDigit(int val) {
    //입력 값은 양의 정수로 가정
    if (val > 0) {
        printDigit(val/10);
        cout << val % 10 << " ";
    }   
}

void printReverseDigit(int val) {
    if (val > 0) {
        cout << val % 10 << " ";
        printReverseDigit(val/10);
    }
}