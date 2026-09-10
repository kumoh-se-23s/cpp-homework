#include<iostream>

using namespace std;

void printReverseDigit(int val) {
    if (val < 10) {
        cout << val;
        return;
    }
    cout << val % 10 << " ";
    printReverseDigit(val / 10);  
}

void printDigit(int val) {
    if (val < 10) {
        cout << val;
        return;
    }
    printDigit(val / 10);     
    cout << " " << val % 10;
    
    
}

int main() {
    int cnt, val ;
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        cin >> val ;
        cout << "Backward : ";
        printReverseDigit(val) ;
        cout << endl << "Forward  : ";
        printDigit(val) ;
        cout << endl ;
    }
    return 0 ;
}