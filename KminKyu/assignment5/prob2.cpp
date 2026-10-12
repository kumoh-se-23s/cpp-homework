#include<iostream>

using namespace std;

void print1DArr(const int *, int);
void myCopy(int *, const int *, int size);

int main() {
    const int MAX = 5;
    int arr1[MAX] = { 1,2,3,4,5 }, arr2[MAX], * p;
    // arr2 = arr1
    // p = arr1;
    /**
     * 1. arr2 = arr1
     * arr1과 arr2의 값은 배열값이 아닌 배열의 주소를 가지고 있다.
     * 고정된 주소(정적배열)를 가지고있는 arr2의 값에 arr1을 넣으니 컴파일 에러가 발생한다.
     *
     * 2. p = arr1;
     * p는 포인터 변수로 선언되어 주소값을 저장할 수 있다
     * p에 arr1을 넣게 되면 arr1의 주소값을 넣게 되어 p가 arr1의 첫번째 값을 가리키게 되고
     * p가 arr1의 값을 복사하지 않는다.
    **/
    p = new int[MAX];
    myCopy(arr2, arr1, MAX);
    myCopy(p, arr1, MAX);
    p[0] = 99;
    print1DArr(arr1, MAX);
    print1DArr(arr2, MAX);
    print1DArr(p, MAX);
    return 0;
}

void myCopy(int *arr1st, const int *arr2nd, int size) {
    for (int index = 0; index < size; ++index) {
        arr1st[index] = arr2nd[index];
    }
}

void print1DArr(const int *arr, int size) {
    for (int index = 0; index < size; ++index) {
        cout << arr[index] << " ";
    }
    cout << endl;
}
