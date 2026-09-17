#include <iostream>

using namespace std;

int main() {
    int tc;
    cin >> tc;

    for (int i = 0; i < tc; i++) {
        double num;
        cin >> num;

        cout << (int)num << " + " << num - (int)num << endl;
    }

    return 0;
}