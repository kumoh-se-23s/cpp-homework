#include <iostream>
using namespace std;

int iteratorGetFactorial(int n);
int recursiveGetFactorial(int n);

int main() {
    int tc;
    cin >> tc;

    for (int i = 0; i < tc; i++) {
        int n;
        cin >> n;
        cout << "iteration = " << iteratorGetFactorial(n) << endl;
        cout << "recursive = " << recursiveGetFactorial(n) << endl;
    }
}
// 13 이상 일 때 터지는 이유는 13!의 값이 int의 범위를 넘어서 오퍼플로우가 발생하기 때문입니다.
// 해결법은 메모리를 최대한 끌어다쓰는 방법으로 배열을 만들어서
// 배열에 데이터를 저장하면 가능할 거 같습니다.
// ex) 배열 [1] = 2, 배열[0] = 4 --> 24(4!)
// 5!이면 배열[1] * 5 = 10. 배열[0] * 5 = 20으로
// 배열[2] = 1 배열[1] = 2 배열[0] = 0의 형태로 저장하는 식의 형태로 저장하면 훨씬 긴 자릿수도 표현 가능 할 거 같습니다.
// 자료구조 다항식 과제에서 n제곱(계수,지수)을 표현하는 방법을 n자릿수(숫자, 자릿수)로 치환하는 형태로도 표현 가능 할 거 같습니다.


int iteratorGetFactorial(int n) {
    int val = 1;

    for (int i = 1; i <= n; i++) {
        val = val * i;
    }

    return val;
}

int recursiveGetFactorial(int n) {
    if (n == 0) {
        return 1;
    }
    return recursiveGetFactorial(n-1) * n;
}