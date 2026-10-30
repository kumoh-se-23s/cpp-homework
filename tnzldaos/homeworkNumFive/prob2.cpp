#include <iostream>
using namespace std;

void myCopy(int* arr1, int* arr2, int size) {
    int arr2Index = 0;

    for(int* index = &arr1[0] ; index < &arr1[size] ; index++) {
        arr2[arr2Index] = *index;
        arr2Index++; 
    }
}

void print1DArr(int* arr, int size) {
    for(int* index = &arr[0] ; index < &arr[size] ; index++) {
        cout << *index << " ";
    }
    cout << endl;
}

int main()
{
    const int MAX = 5;
    int arr1[MAX] = {1,2,3,4,5}, arr2[MAX], *p;

    // arr2 = arr1;
    // arr1과 arr2는 정적배열이라서 =으로 값을 대입하는게 불가능하다
    // 만약 arr1과 arr2가 동적 배열을 가르키는 포인터라면 arr1 = arr2의 결과로 arr1 값에 arr2의 주소값이 들어간다.

    // p = arr1;
    // p변수는 주소값을 가지는 변수이다. p = arr1을 실행시키게 되면 arr1의 주소 값을 가지게 되므로 잘못된 출력이 발생한다.
    
    myCopy(arr1,arr2,MAX);
    p = new int[MAX];
    myCopy(arr1, p , MAX);

    p[0] = 99;
    print1DArr(arr1,MAX);
    print1DArr(arr2,MAX);
    print1DArr(p,MAX);

    delete[] p;
    return 0;
}
