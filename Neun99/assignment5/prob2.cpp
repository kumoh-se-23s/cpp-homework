#include <iostream>

using namespace std;

static void myCopy(int* copy, const int* src, int size);
static void print1DArr(int* arr, int size);

int main() {
    const int MAX = 5;
    int arr1[MAX] = {1,2,3,4,5}, arr2[MAX], *p;

    // arr2 = arr1; 고정된 배열 이름에는 대입 불가, 컴파일 에러가 발생합니다
    myCopy(arr2, arr1, MAX);

    // p = arr1; 같은 위치를 함께 쳐다보는 얕은 복사로 동작, 둘 중 하나를 수정하면 둘 다 영향 받습니다
    p = new int[MAX];
    myCopy(p, arr1, MAX);

    p[0] = 99;
    print1DArr(arr1, MAX);
    print1DArr(arr2, MAX);
    print1DArr(p, MAX);

    //소멸 및 nullptr 배정
    delete[] p;
    p = nullptr;
    return 0;
}

static void myCopy(int* copy, const int* src, int size) {
    for (int idx = 0; idx < size; idx++) {
        copy[idx] = src[idx];
    }
}

static void print1DArr(int* arr, int size) {
    for (int* cur = arr; cur < arr + size; cur++) {
        cout << *cur << " ";
    }
    cout << endl;
}