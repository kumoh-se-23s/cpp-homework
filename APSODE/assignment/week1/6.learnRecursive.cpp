#include <iostream>
using namespace std;

int print_reverse_digit(int value, int digit_base = 10) {
    if (value == 0) return 0;
    printf(" %d", value % digit_base);
    return print_reverse_digit(value / digit_base);
}

int print_digit(int value, int digit_base = 10) {
    if (value == 0) return 0;
    int digit = print_digit(value / digit_base, digit_base);
    printf(" %d", value % digit_base);
    return digit;
}


int main() {
    int cnt, val;
    cin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        cin >> val;
        cout << "Backward : " ;
        print_reverse_digit(val);
        
        cout << endl
             << "Forward : " ;
        print_digit(val);
        cout << endl;
        
    }
    return 0;
}