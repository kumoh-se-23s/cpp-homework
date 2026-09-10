#include<iostream>
#include<cstdint>

using namespace std;

const int MAX_FACTORIAL = 20;
uint64_t iterationFactorial(int val) {
    uint64_t factorialValue = 1;
    for (int number = 1; number <=val; ++number) {
        factorialValue *= number;

    }
    return factorialValue;
}

uint64_t recursiveFactorial(uint64_t val) {
    if (val == 1) return 1;
    return val * recursiveFactorial(val - 1);
}
int main() {
    int testCase;
    cin >> testCase;
    int factorialNumber;
    for (int nowCase = 0; nowCase < testCase; ++nowCase) {
        cin >> factorialNumber;
        if (factorialNumber > MAX_FACTORIAL) {
            cout << "iteration = -1" << endl;
            cout << "recursive = -1" << endl;
        } else {
            cout << "iteration = " << iterationFactorial(factorialNumber) << endl;
            cout << "recursive = " << recursiveFactorial(factorialNumber) << endl;
        }
        
        
    }
    
    /**
     *  12! = 479,001,600
        13!의 경우 int 자료형의 최댓값인 2,147,483,647을 넘겨버려서 오버플로우가 발상하여 이상한 값이 출력되게 된다
        
        해결방안
        1. 메모리가 더 큰 자료형으로 변환하여 계산하면 틀린 값이 나오지 않게 된다 ex) uint64_t
        
     */
}