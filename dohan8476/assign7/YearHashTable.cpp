// #include "YearHashTable.h"
// #include <iostream>
//
// using namespace std;
//
// YearHashTable::YearHashTable()
//     : buckets(nullptr), baseYear(0), minYear(EMPTY_MIN_YEAR), maxYear(EMPTY_MAX_YEAR), capacity(0) {}
//
// YearHashTable::~YearHashTable() {
//     clearAndDeleteAllNodes(); // 내부 StudentNode들을 먼저 모두 delete
//     if (buckets != nullptr) {
//         // cout << "[Delete YearHashTable Buckets Array] " << buckets << endl;
//         delete[] buckets;
//         buckets = nullptr;
//     }
// }
//
// void YearHashTable::init(int minY, int maxY) {
//     baseYear = minY;
//     minYear = minY;
//     maxYear = maxY;
//     // capacity = max - min + 1, 오류(min > max)면 0으로 할당X
//     capacity = (maxYear >= minYear) ? (maxYear - minYear + 1) : 0;
//
//     if (capacity > 0) {
//         buckets = new SinglyLinkedList<StudentNode>[capacity];
//         // cout << "[New YearHashTable Buckets Array] " << buckets << " (Cap: " << capacity << ")" << endl;
//     }
// }
//
// void YearHashTable::resize(int newYear) {
//     //newCap 보정
//     int newBase = (capacity == 0 || newYear < baseYear) ? newYear : baseYear;
//     int currentEnd = capacity == 0 ? newYear : baseYear + capacity - 1;
//     int newEnd = newYear > currentEnd ? newYear : currentEnd;
//     int newCap = newEnd - newBase + 1;
//
//     SinglyLinkedList<StudentNode>* newBuckets = new SinglyLinkedList<StudentNode>[newCap];
//     // cout << "[New resize YearBuckets Array] " << newBuckets << endl;
//
//     if (buckets != nullptr) {
//         for (int i = 0; i < capacity; ++i) {
//             int year = baseYear + i;
//             newBuckets[year - newBase].moveFrom(buckets[i]);
//         }
//         // cout << "[Delete Old YearBuckets Array] " << buckets << endl;
//         delete[] buckets;
//     }
//
//     buckets = newBuckets;
//     baseYear = newBase;
//     capacity = newCap;
//
//     if (minYear > maxYear) {
//         minYear = maxYear = newYear;
//     } else {
//         minYear = newYear < minYear ? newYear : minYear;
//         maxYear = newYear > maxYear ? newYear : maxYear;
//     }
// }
//
// void YearHashTable::insert(StudentNode* node) {
//     int year = node->data.getYear();
//     if (capacity == 0 || year < baseYear || year >= baseYear + capacity) {
//         resize(year);
//     } else {
//         //처음 년도가 들어올때 해당학생 기준으로 세팅되어야함
//         //생성자에서 min(9999) > max(-1)으로 세팅하여 처음인지 아닌지 판별
//         if (minYear > maxYear) {
//             minYear = maxYear = year;
//         } else { // min max update
//             if (year < minYear) minYear = year;
//             if (year > maxYear) maxYear = year;
//         }
//     }
//     buckets[hash(year)].insertAscend(node);
// }
//
// StudentNode* YearHashTable::remove(int studentID) {
//     int year = studentID / 10000;
//     if (capacity == 0 || year < minYear || year > maxYear) {
//         return nullptr;
//     }
//     //buck은 year로 remove는 stuID로
//     StudentNode* target = buckets[hash(year)].remove(studentID);
//     //target을 remove할 때 buckets이 다 비었으면서 year이 min이나 max인 경우 boundary update
//     if (target != nullptr && buckets[hash(year)].isEmpty() && (year == minYear || year == maxYear)) {
//         updateBounds();
//     }
//     return target;
// }
//
// void YearHashTable::updateBounds() {
//     int newMin = -1, newMax = -1;
//     for (int i = 0; i < capacity; i++) {
//         if (!buckets[i].isEmpty()) {
//             newMin = (newMin == -1) ? (baseYear + i) : newMin;
//             newMax = baseYear + i;
//         }
//     }
//     //버킷이 빈 경우 다시 9999, -1로 초기화
//     if (newMin == -1) {
//         minYear = EMPTY_MIN_YEAR;
//         maxYear = EMPTY_MAX_YEAR;
//     } else {
//         minYear = newMin;
//         maxYear = newMax;
//     }
// }
//
// void YearHashTable::clearAndDeleteAllNodes() const {
//     if (buckets == nullptr) return;
//     for (int i = 0; i < capacity; i++) {
//         while (!buckets[i].isEmpty()) {
//             StudentNode* target = buckets[i].removeFirst();
//             // cout << "[Delete Remaining StudentNode (Exit)] " << target << " (ID: " << target->data.getStudentID() << ")" << endl;
//             delete target;
//         }
//     }
// }
//
// int YearHashTable::getMaxYear() const {
//     return maxYear;
// }
//
// int YearHashTable::getMinYear() const {
//     return minYear;
// }
//
// SinglyLinkedList<StudentNode> &YearHashTable::getBucketByYear(int year) {
//     return buckets[hash(year)];
// }
//
// const SinglyLinkedList<StudentNode> &YearHashTable::getBucketByYear(int year) const {
//     return buckets[hash(year)];
// }
//
// int YearHashTable::hash(int year) const {
//     return year - baseYear;
// }
//
// bool YearHashTable::isExist(int studentID) const {
//     if (buckets == nullptr || capacity == 0) return false;
//
//     // year 기준
//     int year = studentID / 10000;
//     if (year < minYear || year > maxYear) return false;
//
//     // year이 범위안이면 해시값 가지고 돌면서 탐색
//     int idx = hash(year);
//     if (idx < 0 || idx >= capacity) return false;
//
//     StudentNode* curr = buckets[idx].getHead();
//     while (curr != nullptr) {
//         if (*curr == studentID) return true;
//         curr = curr->next;
//     }
//     return false;
// }