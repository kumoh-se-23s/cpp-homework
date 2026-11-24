#include "IdHashMap.h"

//생성자-----------------------------------
//기본생성자
IdHashMap::IdHashMap() {
    arr = new IdHashNode[capacity];
    size = 0;
}

//복사생성자
IdHashMap::IdHashMap(const IdHashMap &idHashMap) {
    size = idHashMap.size;
    capacity = getEnoughCapacity(size);
    arr = new IdHashNode[capacity];
    for (int idx = 0; idx < size; idx++) {
        arr[idx] = idHashMap.arr[idx];
    }
}

//대입연산자
IdHashMap &IdHashMap::operator=(const IdHashMap &idHashMap) {
    if (this != &idHashMap) {
        size = idHashMap.size;
        capacity = getEnoughCapacity(size);
        IdHashNode* newArr = new IdHashNode[capacity];
        for (int idx = 0; idx < size; idx++) {
            newArr[idx] = idHashMap.arr[idx];
        }
        delete[] arr;
        arr = newArr;
    }

    return *this;
}

//소멸자
IdHashMap::~IdHashMap() {
    delete[] arr;
}

//getter-------------------------------
//인자로 받은 년도의 노드 꺼내기: 없으면 nullptr
IdHashNode *IdHashMap::operator[](int year) const{
    int diff = year - getMinYear(); //최소년도와의 차이가
    if (diff >= 0 && diff < size) //범위 안이면 바로 return
        return &arr[diff];
    return nullptr; //아니면 nullptr
}

//최소년도: 맨앞
int IdHashMap::getMinYear() const {
    if (size == 0)
        return 0;
    return arr[0].getId();
}

//최대년도: 맨 뒤
int IdHashMap::getMaxYear() const {
    if (size == 0)
        return 0;
    return arr[size - 1].getId();
}

//기능----------------------
//용량 미리 확장
void IdHashMap::presetCapacity(int minLength) {
    if (minLength > capacity) {
        capacity = getEnoughCapacity(minLength);
        IdHashNode* newArr = new IdHashNode[capacity];
        copyArr(newArr, arr, size);
        delete[] arr;
        arr = newArr;
    }
}

IdHashNode *IdHashMap::insert(int year) {
    if (size == 0) { //size 0일 때는 바로 넣고 리턴
        arr[0] = IdHashNode(year);
        size++;
        return &arr[0];
    }

    //size 1 이상인 경우
    int diff = year - getMinYear(); //최소년도와의 차이가

    if (diff < 0) { //음수면 최소년도 갱신
        insertFront(year, diff);
        return &arr[0];
    }

    if (diff < size) //범위 안이면 바로 return
        return &arr[diff];

    //size보다 크면 최대년도 갱신
    append(year, diff);
    return &arr[size - 1];
}

//제거: 맨 앞이나 맨 뒤일 경우만 작동
void IdHashMap::remove(int year) {
    if (size == 0) //비었으면 지울것도 없으므로 작동 x
        return;

    if (year == getMinYear()) { // 맨 앞이면 최소년도 갱신
        // 현재 유의미한 최소년도 탐색
        int cnt = 1;
        for (; cnt < size && arr[cnt].isEmpty(); cnt++) {}

        // 배열 갱신
        for (int idx = 0; idx < size - cnt; idx++) {
            arr[idx] = arr[idx + cnt];
        }

        size -= cnt;
    } else if (year == getMaxYear()) { // 맨 뒤면 최대년도 갱신
        // 현재 유의미한 최대년도 탐색
        int cnt = 1;
        for (; cnt < size && arr[size - cnt - 1].isEmpty(); cnt++) {}
        size -= cnt;

    }
}

//private--------------------------
//capacity 2배씩 늘리며 인자를 담을 수 있는 capacity 찾기
int IdHashMap::getEnoughCapacity(int minLength) const {
    int result;
    for (result = capacity; result < minLength; result *= 2) {}
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
    if (newSize > capacity) { //부족한 경우
        capacity = getEnoughCapacity(newSize);
        IdHashNode* newArr = new IdHashNode[capacity];

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
        for (int idx = size - 1; idx >= 0; idx--) {
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
    int newSize = diff + 1; //새 사이즈

    if (newSize > capacity) //용량체크
        presetCapacity(newSize);

    //넣기
    for (int idx = newSize - 1; idx >= size; idx--, year--) {
        arr[idx] = IdHashNode(year);
    }

    size = newSize;
}
