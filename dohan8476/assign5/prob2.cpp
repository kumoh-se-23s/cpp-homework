#include <iostream>
using namespace std;

void print1DArr(const int *arr1, int MAX);
void myCopy(int *, const int *, int size);

int main()
{
    const int MAX = 5;
    int arr1[MAX] = { 1,2,3,4,5 }, arr2[MAX], *p;
    // 아래 arr2 = arr1이 실행되지 않는 이유는 arr2는 이미 Max에 대한 주소 시작값을 가지고 있고 arr1또한 마찬가지로 주솟값을 가지고 있다.
    // 그럼 컴파일러는 0x2000 = 0x1000과 같은 수식으로 이해를 하는데 수학에서 3 = 5라고 쓸 수 없는거 처럼 논리적으로 안된다고 이해하며
    // 아래와 같은 수정은 수정이 불가능한 고정된 주소라고 판단해 에러를 띄우는 것이다.
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

