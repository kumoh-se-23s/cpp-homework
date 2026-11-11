#include "IdHashMap.h"

//생성자-----------------------------------
//기본생성자
IdHashMap::IdHashMap() {
    capacity = DEFAULT_CAPACITY;
    arr = new IdHashNode[capacity];
    size = 0;
}

//길이 받는 생성자
IdHashMap::IdHashMap(int minLength) {
    capacity = getEnoughCapacity(minLength);
    arr = new IdHashNode[capacity];
    size = 0;
}

//소멸자
IdHashMap::~IdHashMap() {
    delete[] arr;
}

//기능-------------------------------
//인자로 받은 년도의 head 꺼내기: 없으면 위치맞춰 만들어서 넘겨줌
StudentNode *IdHashMap::getHeadByKey(int year) {
    if (size == 0) { //size 0일 때는 바로 만들고 리턴
        arr[0] = IdHashNode(year);
        size++;
        return arr[0].getHead();
    }

    //size 1 이상인 경우
    int diff = year - getMinYear(); //최소년도와의 차이

    if (diff < 0) { //음수면 최소년도 갱신
        insertFront(year, diff);
        return arr[0].getHead();
    }

    if (diff < size) //범위 안이면 바로 return
        return arr[diff].getHead();

    //size보다 크면 최대년도 갱신
    append(year, diff);
    return arr[size].getHead();
}

//최소년도: 맨앞
int IdHashMap::getMinYear() const {
    return arr[0].getId();
}

//최대년도: 맨 뒤
int IdHashMap::getMaxYear() const {
    return arr[size - 1].getId();
}

//private--------------------------
//resize
void IdHashMap::resize(int minLength) {
    capacity = getEnoughCapacity(minLength);
    IdHashNode* newArr = new IdHashNode[capacity];
    copyArr(newArr, arr, size);
    delete[] arr;
    arr = newArr;
}

//capacity 2배씩 늘리며 인자를 담을 수 있는 capacity 찾기
int IdHashMap::getEnoughCapacity(int minLength) const {
    int result;
    for (result = capacity; result > minLength; result *= 2) {}
    return result;
}

//배열 깊은복사
void IdHashMap::copyArr(IdHashNode *copy, IdHashNode *src, int srcSize) {
    for (int idx = 0; idx < srcSize; idx++) {
        copy[idx] = src[idx];
    }
}

//앞에 추가
void IdHashMap::insertFront(int year, int diff) {
    if (diff < 0) //음수 diff면 양수로 바꾸기
        diff *= -1;

    int newSize = size + diff; //바뀔 사이즈

    //용량체크
    if (newSize > capacity) { //부족한 경우, 복사 횟수 줄이기 위해 resize 호출 x
        IdHashNode* newArr = new IdHashNode[getEnoughCapacity(newSize)];

        int idx = 0;
        //앞부분 넣기
        for (; idx < diff; idx++, year++) {
            newArr[idx] = IdHashNode(year);
        }

        //뒷부분 복사
        for (int srcIdx = 0; srcIdx < size; idx++, srcIdx++) {
            newArr[idx] = arr[srcIdx];
        }

        delete[] arr;
        arr = newArr;

    } else { //넉넉한 경우 그냥 밀고 넣기
        //밀기
        for (int idx = size - 1; idx >= diff; idx--) {
            arr[idx + diff] = arr[idx];
        }

        //넣기
        for (int idx = 0; idx < diff; idx++, year++) {
            arr[idx] = IdHashNode(year);
        }
    }

    size = newSize;
}

//뒤에 추가
void IdHashMap::append(int year, int diff) {
    int newSize = size + diff; //새 사이즈

    if (newSize > capacity) //용량체크
        resize(newSize);

    //넣기
    for (int idx = newSize - 1; idx >= size; idx--, year--) {
        arr[idx] = year;
    }

    size = newSize;
}
