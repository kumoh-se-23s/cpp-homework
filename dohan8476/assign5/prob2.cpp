#include <iostream>
using namespace std;

void print1DArr(const int *arr1, int MAX);
void myCopy(int *, const int *, int size);

int main()
{
    const int MAX = 5;
    int arr1[MAX] = { 1,2,3,4,5 }, arr2[MAX], *p;
    // arr2 = arr1; // arr2가 arr1의 복사본이 되도록 함수 myCopy(int *, int *, size) 작성
    myCopy(arr2, arr1, MAX);
    // p = arr1; // p는 arr1의 복사본이 되도록, 동적 할당 후 myCopy() 호출
    p = new int[MAX];
    myCopy(p, arr1, MAX);
    p[0] = 99; // 위의 수정이 제대로 되면 arr2은 안 바뀌면서 p[0]만 수정

    print1DArr(arr1, MAX); // print1DArr(int *, int)를 추가 구현할 것
    print1DArr(arr2, MAX);
    print1DArr(p, MAX);

    delete[] p;
    p = nullptr;

    return 0;
}

void print1DArr(const int *arr, const int MAX) {
    for (const int *p = arr; p < arr + MAX; p++) {
        cout << *p << " ";
    }
    cout << endl;
}

void myCopy(int *arr1,const int *arr2, const int size) {
    for (int i = 0; i < size; i++) {
        arr1[i] = arr2[i];
    }
}

