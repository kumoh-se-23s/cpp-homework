#include <iostream>

int recursive_factorial(int num) {
    return num > 1 ? num * recursive_factorial(num - 1) : 1;
}

int iterative_factorial(int num) {
    int result = 1;
    while (num > 1) {
        result *= num--;
    }

    return result;
}


/*
 * 13! 이후에서 재귀/반복의 factorial의 값이 이상한 이유는 signed int의 범위가 -2147483648 ~ 2147483647의 값을 가지기 때문이다.
 * 따라서 이 문제를 해결하기 위해서는 결과가 저장되는 변수가 signed int 이상의 범위를 가지는 변수로 정의하면 해결할 수 있다.
 * 이는 여러가지 방법이 있다.
 *
 * 1. unsigned int와 같은 더 넓은 범위의 타입 사용
 * 2. 자바의 BigInteger와 같은 문자열 기반 커스텀 구현 클래스 사용
 * 3. 메모리 동적할당을 이용한 64bit/128bit의 정수형 공간 할당
 *
 * 이러한 방법 중에 2번의 경우 실제로 직접 구현또한 가능하다
 * 이 경우, 문자열을 이용하여 signed int 이상의 범위를 가지는 정수를 저장하고,
 * 이에 대한 각각의 연산(ex 연산 중의 올림수)을 직접 정의하여 구현하는 것으로 구현이 가능하다고 생각된다.
 */
int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for(int loop = 0; loop < loop_amount; ++loop) {
        int input_number;
        std::cin >> input_number;

        printf("iteration = %d\n", iterative_factorial(input_number));
        printf("recursive = %d\n", recursive_factorial(input_number));

        malloc(10);
    }

    return 0;
}
