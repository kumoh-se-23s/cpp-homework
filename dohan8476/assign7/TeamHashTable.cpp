#include "TeamHashTable.h"
#include <iostream>

using namespace std;

TeamHashTable::TeamHashTable(int initCap) : capacity(initCap), groupCount(0) {
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

void TeamHashTable::resize() {
    int oldCap = capacity;
    CircularLinkedList<StudentNode>* oldBuckets = buckets;
    MyString* oldTeamIDs = teamIDs;

    capacity *= 2;
    CircularLinkedList<StudentNode>* newBuckets = new CircularLinkedList<StudentNode>[capacity];
    // cout << "[New resize TeamBuckets Array] " << newBuckets << endl;
    MyString* newTeamIDs = new MyString[capacity];
    // cout << "[New resize TeamIDs Array] " << newTeamIDs << endl;

    for (int i = 0; i < oldCap; ++i) {
        // 삭제 시 "-"로 마킹해서 -도 스킵해서 최적화
        if (oldTeamIDs[i] != "" && oldTeamIDs[i] != "-") {
            int start = hash(oldTeamIDs[i]);
            for (int j = 0; j < capacity; ++j) {
                int idx = (start + j) % capacity;
                if (newTeamIDs[idx] == "") {
                    newTeamIDs[idx] = oldTeamIDs[i];
                    newBuckets[idx].moveFrom(oldBuckets[i]);
                    break;
                }
            }
        }
    }

    // cout << "[Delete Old TeamBuckets Array] " << buckets << endl;
    delete[] oldBuckets;
    // cout << "[Delete Old TeamIDs Array] " << teamIDs << endl;
    delete[] oldTeamIDs;

    buckets = newBuckets;
    teamIDs = newTeamIDs;
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
    if (teamID == "") return -1;

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
    // 75% 정도 차면 resize해서 해시 충돌 횟수 완화
    // groupCnt + 1 >= cap으로하면 해시 충돌이 많이나서 해시의 이점이 적어짐
    if (groupCount * 4 >= capacity * 3) {
        resize();
    }

    int start = hash(teamID);
    for (int i = 0; i < capacity; i++) {
        int idx = (start + i) % capacity;
        if (teamIDs[idx] == "" || teamIDs[idx] == "-") {
            teamIDs[idx] = teamID;
            return idx;
        }
    }
    return -1;
}

void TeamHashTable::insert(StudentNode* node) {
    if (!node->data.hasTeam()) return;

    int idx = find(node->data.getTeamID());
    //find 결과가 없으니 버킷 만들어야함
    if (idx == -1) {
        // cout << "[create New Team] " << node->data.getTeamID() << endl;
        idx = createBucket(node->data.getTeamID());
    }
    //team에 재가입
    if (buckets[idx].isEmpty()) {
        groupCount++;
    }
    buckets[idx].insertAscend(node);
}

void TeamHashTable::remove(StudentNode* node) {
    if (node == nullptr || !node->data.hasTeam()) return;

    int idx = find(node->data.getTeamID());
    if (idx == -1) return;

    StudentNode* removed = buckets[idx].remove(node);

    if (removed != nullptr && buckets[idx].isEmpty()) {
        teamIDs[idx] = "-"; // 삭제 마킹
        groupCount--;
    }
}

int TeamHashTable::getGroupCount() const {
    return groupCount;
}