#include <iostream>
using namespace std;

int main() {
    int num;
    cout << "몇단을 출력할까요?";
    cin >> num;
    for (int i = 1; i < 10; ++i) {
        cout << num << "+" << i << "=" << num*i << endl;
        
    }
    return 0;
}