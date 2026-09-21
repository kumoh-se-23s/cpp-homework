
#include <iostream>

void numToArr(int num, int *result, int size)
{

    for (int i = 0; i < size; ++i)
    {
        result[i] = num % 10;
        num /= 10;
    }
}

void printArr(int *arr, int size)
{
    using namespace std;
    bool zero = true;
    for (int i = size - 1; i >= 0; --i)
    {
        if (arr[i] != 0)
            zero = false;
        if (!zero)
            cout << arr[i];
    }
    if(zero) cout << "0";
    cout << endl;
}


void inPlaceAdd(int *v, int *op, int opSize)
{
    int carry = 0;
    for(int i = 0; i < opSize; ++i){
        int add = carry + v[i] + op[i];
        v[i] = add % 10;
        carry = add / 10;
    }
    v[opSize] += carry;
}


void mul(int *lhs, int rhs, int *result, int lhsSize)
{
    int carry = 0;
    for (int i = 0; i < lhsSize; ++i)
    {
        int mul = carry + lhs[i] * rhs;
        result[i] = mul % 10;
        carry = mul / 10;
    }
    result[lhsSize] = carry;
}

void multiplyWithPrint(int lhs, int rhs)
{

    using namespace std;

    constexpr int OPERAND_SIZE = 3;
    constexpr int OPERATION_SIZE = OPERAND_SIZE + 1;
    constexpr int RESULT_SIZE = OPERAND_SIZE * 2;

    int uArr[OPERAND_SIZE]{0};
    int dArr[OPERAND_SIZE]{0};

    int resultArr[RESULT_SIZE]{0};
    int *resultPtr = resultArr;

    numToArr(lhs, uArr, OPERAND_SIZE);
    numToArr(rhs, dArr, OPERAND_SIZE);

    for (int i = 0; i < OPERAND_SIZE; ++i)
    {
        int tArr[OPERATION_SIZE]{0};
        mul(uArr, dArr[i], tArr, OPERAND_SIZE);
        printArr(tArr, OPERATION_SIZE);
        inPlaceAdd(resultPtr + i, tArr, OPERATION_SIZE);
    }
    printArr(resultArr, RESULT_SIZE);
}

int main()
{
    using namespace std;

    int lhs;
    int rhs;

    cin >> lhs;
    cin >> rhs;

    multiplyWithPrint(lhs, rhs);

    return 0;
}