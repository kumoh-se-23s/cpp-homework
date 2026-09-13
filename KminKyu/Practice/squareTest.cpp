#include <iostream>
#include <cmath>
#include <cassert>
#define NDBUG
#define kmk
using namespace std;
float getSquareRoot(float t);

int main() {
    cout << getSquareRoot(-9) << endl;


float getSquareRoot(float t) {
    assert(t >= 0);

#ifdef kmk {
    if (t < 0) {
        cout << "입력값에 문제 발생" << endl;
        return 0;
    }
}
#endif kmk