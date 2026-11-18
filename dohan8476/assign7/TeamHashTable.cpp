#include "TeamHashTable.h"
#include <iostream>

using namespace std;

TeamHashTable::TeamHashTable(int initCap) : capacity(initCap), groupCnt(0) {
    buckets = new CircularLinkedList<StudentNode>[capacity];
    // cout << "[New TeamHashTable Buckets Array] " << buckets << endl;
    teamIDs = new MyString[capacity];
    // cout << "[New TeamHashTable TeamIDs Array] " << teamIDs << endl;
}

TeamHashTable::~TeamHashTable() {
    // StudentNode들은 YearHashTable에서 이미 delete 했으므로 버킷 배열만 해제
    if (buckets != nullptr) {
        // cout << "[Delete TeamHashTable Buckets Array] " << buckets << endl;
        delete[] buckets;
        buckets = nullptr;
    }
    if (teamIDs != nullptr) {
        // cout << "[Delete TeamHashTable TeamIDs Array] " << teamIDs << endl;
        delete[] teamIDs;
        teamIDs = nullptr;
    }
}

void TeamHashTable::resize(int newCap) {
    CircularLinkedList<StudentNode>* newBuckets = new CircularLinkedList<StudentNode>[newCap];
    // cout << "[New resize TeamBuckets Array] " << newBuckets << endl;
    MyString* newTeamIDs = new MyString[newCap];
    // cout << "[New resize TeamIDs Array] " << newTeamIDs << endl;

    for (int i = 0; i < capacity; ++i) {
        newBuckets[i].moveFrom(buckets[i]);
        newTeamIDs[i] = teamIDs[i];
    }

    // cout << "[Delete Old TeamBuckets Array] " << buckets << endl;
    delete[] buckets;
    // cout << "[Delete Old TeamIDs Array] " << teamIDs << endl;
    delete[] teamIDs;

    buckets = newBuckets;
    teamIDs = newTeamIDs;
    capacity = newCap;
}

int TeamHashTable::hash(const MyString& teamID) const {
    unsigned long hashIdx = 0;
    for (int i = 0; i < teamID.length(); i++) {
        //주로 소수를 곱함(AB, BA)처럼 문자열 같고 순서가 다를때의 가중치를 다르게 하기 위해서
        //AB, capa 10이면
        //65(A) -> 2081(65 * 31 + 66) -> %10 = 1
        hashIdx = hashIdx * 31 + static_cast<unsigned char>(teamID[i]);
    }
    return hashIdx % capacity;
}

int TeamHashTable::find(const MyString& teamID) const {
    int start = hash(teamID);
    for (int i = 0; i < capacity; i++) {
        int idx = (start + i) % capacity;
        if (teamIDs[idx] == teamID) return idx;
        if (teamIDs[idx] == "") return -1;
    }
    return -1;
}

//teamID 받아서 버킷 만들기
int TeamHashTable::createBucket(const MyString& teamID) {
    for (int i = 0; i < capacity; i++) {
        if (teamIDs[i] == "") {
            teamIDs[i] = teamID;
            return i;
        }
    }

    int oldCap = capacity;
    resize(capacity * 2);
    teamIDs[oldCap] = teamID;
    return oldCap;
}

void TeamHashTable::insert(StudentNode* node) {
    if (!node->data.hasTeam()) return;

    int idx = find(node->data.getTeamID());
    //find 결과가 없으니 버킷 만들어야함
    if (idx == -1) {
        idx = createBucket(node->data.getTeamID());
        groupCnt++;
    }
    buckets[idx].insertAscend(node);
}

void TeamHashTable::remove(StudentNode* node) {
    if (node == nullptr || !node->data.hasTeam()) return;

    int idx = find(node->data.getTeamID());
    if (idx == -1) return;

    buckets[idx].remove(node);

    if (buckets[idx].isEmpty()) {
        groupCnt--;
        teamIDs[idx] = "";
    }
}

int TeamHashTable::getGroupCount() const {
    return groupCnt;
}