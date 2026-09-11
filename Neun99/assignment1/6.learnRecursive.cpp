#include <iostream>

using namespace std;

int main() {
    void printReverseDigit(int val);
    void printDigit(int val);

    int cnt, val ;
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        cin >> val ;
        cout << "Backward : ";
        printReverseDigit(val);
        cout << endl << "Forward : ";
        printDigit(val) ;
        cout << endl ;
    }
    return 0 ;
}

void printReverseDigit(int val) {
    cout << val % 10 << " ";
    if(val / 10 >= 10)
        printReverseDigit(val / 10);
    else
        cout << val / 10;
}

void printDigit(int val) {
    if (val >= 10)
        printDigit(val / 10);
    cout << val % 10 << " ";
}