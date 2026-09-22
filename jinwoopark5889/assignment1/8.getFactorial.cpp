#include <iostream>

using namespace std;

int getFactorialIteration(int);
int getFactorialRecursive(int);

// 팩토리얼에서 13! 이상에서 오류가 발생하는 이유 : int의 값이 21억이 초과하게 될 경우 오버플로우가 발생하기 때문입니다.
// 해결책 : BigInteger클래스를 활용한 이 방식은 숫자를 일의 자리수로 낮은 자리수부터 높은자리수 순으로 배열에 저장시키는 방식입니다.
// 문제가 발생하는 13!의 값의 상황에서 BigInteger방식은 13! 계산 시 12!의 값이 저장되어 있는 배열을 참조하여 일의 자리수부터 저장된 배열에
// 13을 곱하여 올림 연산을 수행하여 배열에 값을 저장하게 되면 13!의 올바른 값이 배열에 저장됩니다.
int main()
{
    int repeat;
    cin >> repeat;

    for (int i = 0; i < repeat; i++)
    {
        int num;
        cin >> num;

        cout << "Iteration = " << getFactorialIteration(num) << endl
             << "Recursive = " << getFactorialRecursive(num) << endl;
    }

    return 0;
}

int getFactorialIteration(int num)
{
    int result = 1;
    for (int i = 1; i <= num; i++)
    {
        result *= i;
    }
    return result;
}

int getFactorialRecursive(int num)
{
    if (num == 1)
    {
        return 1;
    }
    return num * getFactorialRecursive(num - 1);
}
