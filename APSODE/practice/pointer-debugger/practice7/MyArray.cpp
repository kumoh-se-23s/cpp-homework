//
// Created by apsode on 26. 10. 7..
//
#include <iostream>
using namespace std;

class MyArray {
public :
	MyArray(int c = 8, int initValue = 0);
	MyArray(const MyArray& arr2);
	int getItem(int idx) const;
	void appendItem(int val);
	MyArray& operator =(const MyArray& arr2);
	MyArray operator +(const MyArray& arr2) const;
		// C++11이후 const return 하지 않음
	void print() const;
	~MyArray();
private :
	void resize(int newCapacity);
	int capacity ;
	int itemCnt ;
	int* arr;
	const int RESIZE_RATIO = 2;
};

MyArray::MyArray(int c, int initValue) : capacity(c), itemCnt(0){
	arr = new int[capacity];
	for (int i = 0; i < capacity; i++)
		arr[i] = initValue;
	if (initValue != 0) // 초기값이 0이 아니면 개수만큼 저장하는 것으로 간주
		itemCnt = capacity ;
}

MyArray::MyArray(const MyArray& arr2)
	: capacity(arr2.capacity), itemCnt(arr2.itemCnt) {
	cout << "복사 생성자 구동" << endl;
	for (int i = 0; i < itemCnt; i++)
		arr[i] = arr2.arr[i];
}

int MyArray::getItem(int idx) const {
	if (0 <= idx && idx < itemCnt )
		return arr[idx];
	else
		throw out_of_range("배열 인덱스의 범위 초과");
		// 또는 return 0 ;
}

void MyArray::appendItem(int val)  {
	if (itemCnt == capacity - 1) {
		resize(capacity * RESIZE_RATIO);
	}
	arr[itemCnt++] = val;
}

void MyArray::resize(int newCapacity) {
	int newItemCnt = itemCnt ;
	if (newCapacity == capacity) return;
	else if (newCapacity < capacity)
		// resize 배열이 기존 배열보다 작은 경우
		newItemCnt = newCapacity;

	int* srcArr = arr; // 기존 배열 주소 기억
	capacity = newCapacity;
	itemCnt = newItemCnt;
	arr = new int[newCapacity];	// 새 공간 할당
	for (int i = 0; i < newItemCnt; i++)
		arr[i] = srcArr[i];
	delete[] srcArr; // 기존 배열 delete
}

MyArray& MyArray::operator =(const MyArray& arr2){
	cout << "대입 연산자 구동" << endl;
	if (this == &arr2) return *this;

	if (capacity != arr2.capacity) {
		delete[] arr;
		arr = new int[arr2.capacity];
		capacity = arr2.capacity;
	}
	itemCnt = arr2.itemCnt;
	for (int i = 0; i < itemCnt; i++)
		arr[i] = arr2.arr[i];
}

MyArray MyArray::operator +(const MyArray& arr2) const {
	int newItemCnt = itemCnt + arr2.itemCnt;
	MyArray resultArr(newItemCnt) ; // 공간의 크기가 8의 배수가 되지 않음

	for (int i = 0; i < itemCnt; i++)
		resultArr.appendItem(arr[i]);
	for (int i = 0 ; i < arr2.itemCnt ; i++)
		resultArr.appendItem(arr2.arr[i]);

	return resultArr;
}

void MyArray::print() const {
	cout << "[";
	if (itemCnt > 0) cout << arr[0];
	for (int i = 1; i < itemCnt; i++)
		cout << ", " << arr[i];
	cout << "]";
}

MyArray::~MyArray() {
	cout << "파괴자 구동" << endl ;
	delete[] arr;
}