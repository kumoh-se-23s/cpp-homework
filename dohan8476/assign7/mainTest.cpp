#include <iostream>
#include <fstream>
#include <cstdio>
#include <sstream>

#include "FileIO.h"
#include "StudentList.h"
#include "Student.h"
#include "StudentNode.h"
#include "MyString.h"
#include "StudentListManager.h"

using namespace std;

int gPass = 0;
int gFail = 0;

void check(bool cond, const char* msg) {
    if (cond) {
        gPass++;
    } else {
        gFail++;
        cout << "  [FAIL] " << msg << "\n";
    }
}

// 내부 자료구조(단일 연결리스트 + 원형 이중 연결리스트) 포인터 무결성 전수 검사
bool verifyIntegrity(const StudentList& list) {
    int totalCount = 0;
    int sCount = list.getStudentCount();

    if (sCount == 0) {
        return list.getGroupCount() == 0;
    }

    const YearHashTable& yt = list.getYearTable();
    int minY = list.getMinYear();
    int maxY = list.getMaxYear();
    if (minY > maxY) return false;

    for (int y = minY; y <= maxY; ++y) {
        const SinglyLinkedList<StudentNode>& bucket = yt.getBucketByYear(y);
        StudentNode* curr = bucket.getHead();
        int bucketCnt = 0;

        while (curr != nullptr) {
            // 1) 연도 버킷 정합성 및 학번 오름차순 검사
            if (curr->data.getYear() != y) return false;
            if (curr->next != nullptr && !(curr->data < curr->next->data)) return false;

            // 2) 팀 원형 이중 연결리스트 포인터 무결성 검사
            if (!curr->data.hasTeam()) {
                if (curr->nextTeam != nullptr || curr->prevTeam != nullptr) return false;
            } else {
                if (curr->nextTeam == nullptr || curr->prevTeam == nullptr) return false;
                if (curr->nextTeam->prevTeam != curr) return false;
                if (curr->prevTeam->nextTeam != curr) return false;

                // 원형 순회 시 무한 루프 없이 자기 자신으로 돌아오는지 + 팀ID 일치 검사
                StudentNode* walk = curr->nextTeam;
                int steps = 0;
                while (walk != curr) {
                    if (walk == nullptr || steps > sCount) return false;
                    if (walk->data.getTeamID() != curr->data.getTeamID()) return false;
                    if (walk->nextTeam->prevTeam != walk) return false;
                    walk = walk->nextTeam;
                    steps++;
                }
            }

            curr = curr->next;
            bucketCnt++;
            totalCount++;
        }
        if (bucketCnt != bucket.getSize()) return false;
    }

    return totalCount == sCount;
}

// 1. MyString & Student 경계값 (긴 이름, 다중 공백, '-' 무소속 판별)
void test01_StudentAndTeamID() {
    cout << "[Test 01] Student / 팀ID('-') / 긴 문자열 테스트\n";
    Student s1(20240001, MyString("Dash"), MyString("-"));
    Student s2(20240002, MyString("Empty"), MyString(""));
    Student s3(20240003, MyString("Hyphen"), MyString("G-1"));
    Student s4(20240004, MyString("DoubleDash"), MyString("--"));
    Student s5(20240005, MyString("Very Long Name With Multiple Spaces Over Default Capacity"), MyString("VeryLongTeamName12345"));

    check(!s1.hasTeam(), "\"-\" -> 팀 없음");
    check(!s2.hasTeam(), "\"\" -> 팀 없음");
    check(s3.hasTeam(), "\"G-1\" -> 팀 있음");
    check(s4.hasTeam(), "\"--\" -> 팀 있음");
    check(s5.hasTeam() && s5.getYear() == 2024, "긴 이름/팀명 정상 생성");

    StudentList list;
    list.addStudent(s1);
    list.addStudent(s2);
    list.addStudent(s3);
    list.addStudent(s4);
    list.addStudent(s5);
    check(list.getStudentCount() == 5 && list.getGroupCount() == 3, "무소속 2명 + 팀 3개");
    check(verifyIntegrity(list), "포인터 무결성");
}

// 2. 동일 연도 SinglyLinkedList (역순/중간 삽입, head/중간/tail/단독 삭제)
void test02_SinglyListPositions() {
    cout << "[Test 02] 동일 연도 단일 연결리스트 삽입/삭제 위치 테스트\n";
    StudentList list;
    // 역순 + 중간 끼워넣기
    list.addStudent(Student(20240030, MyString("C"), MyString("-")));
    list.addStudent(Student(20240010, MyString("A"), MyString("-")));
    list.addStudent(Student(20240040, MyString("D"), MyString("-")));
    list.addStudent(Student(20240020, MyString("B"), MyString("-")));
    check(verifyIntegrity(list), "4명 정렬 삽입 무결성");

    check(list.deleteStudent(20240010), "head 삭제");
    check(verifyIntegrity(list), "head 삭제 후 무결성");

    check(list.deleteStudent(20240030), "중간 삭제");
    check(verifyIntegrity(list), "중간 삭제 후 무결성");

    check(list.deleteStudent(20240040), "tail 삭제");
    check(verifyIntegrity(list), "tail 삭제 후 무결성");

    check(list.deleteStudent(20240020), "마지막 1명 삭제");
    check(list.getStudentCount() == 0 && verifyIntegrity(list), "0명 상태 무결성");
}

// 3. CircularLinkedList 크기별(1명, 2명, 4명) head/중간/tail 삭제
void test03_CircularListPositions() {
    cout << "[Test 03] 팀 원형 이중 연결리스트(1/2/4명) 포인터 테스트\n";
    StudentList list;

    // 1명 팀 생성 -> 자기 자신 순환 확인 -> 삭제
    list.addStudent(Student(20220001, MyString("Solo"), MyString("T1")));
    check(verifyIntegrity(list), "1명 팀 순환 무결성");
    check(list.deleteStudent(20220001), "1명 팀 삭제");
    check(list.getGroupCount() == 0 && verifyIntegrity(list), "1명 팀 소멸");

    // 2명 팀: head 삭제 vs tail 삭제
    list.addStudent(Student(20220001, MyString("A"), MyString("T2")));
    list.addStudent(Student(20230001, MyString("B"), MyString("T2")));
    check(verifyIntegrity(list), "2명 팀 무결성");
    check(list.deleteStudent(20220001), "2명 팀 head 삭제");
    check(list.getGroupCount() == 1 && verifyIntegrity(list), "1명 남은 팀 자기순환 복구");

    list.addStudent(Student(20210001, MyString("Z"), MyString("T2"))); // Z(head) <-> B(tail)
    check(list.deleteStudent(20230001), "2명 팀 tail 삭제");
    check(list.getGroupCount() == 1 && verifyIntegrity(list), "tail 삭제 후 1명 순환 복구");

    // 4명 팀으로 확장 후 중간/head/tail 순차 삭제
    list.addStudent(Student(20220002, MyString("M1"), MyString("T2")));
    list.addStudent(Student(20230002, MyString("M2"), MyString("T2")));
    list.addStudent(Student(20240002, MyString("Tail"), MyString("T2")));
    check(verifyIntegrity(list), "4명 팀 무결성");

    check(list.deleteStudent(20220002), "4명 팀 중간 삭제");
    check(list.deleteStudent(20210001), "3명 팀 head 삭제");
    check(list.deleteStudent(20240002), "2명 팀 tail 삭제");
    check(list.deleteStudent(20230002), "마지막 1명 삭제");
    check(list.getGroupCount() == 0 && verifyIntegrity(list), "전체 팀원 삭제 후 무결성");
}

// 4. YearHashTable 양방향 확장/축소 & 빈 연도(Gap) 건너뛰기
void test04_YearHashResizeAndGaps() {
    cout << "[Test 04] YearHashTable 양방향 확장/축소 및 빈 연도 갭 테스트\n";
    StudentList list;

    // initYearRange 없이 바로 삽입
    list.addStudent(Student(20200001, MyString("Base1"), MyString("G1")));
    list.addStudent(Student(20200002, MyString("Base2"), MyString("G1")));
    check(list.getMinYear() == 2020 && list.getMaxYear() == 2020, "단일 연도 시작");

    // 과거/미래 극단 확장
    list.addStudent(Student(19750001, MyString("Past"), MyString("G1")));
    list.addStudent(Student(20450001, MyString("Future"), MyString("G1")));
    check(list.getMinYear() == 1975 && list.getMaxYear() == 2045, "양방향 확장 (1975~2045)");
    check(verifyIntegrity(list), "양방향 확장 후 무결성");

    // 경계 연도에 2명 있을 때 1명만 삭제 -> 경계 유지 확인
    list.addStudent(Student(19750002, MyString("Past2"), MyString("G2")));
    list.deleteStudent(19750001);
    check(list.getMinYear() == 1975, "경계 연도 잔여 학생 존재 시 minYear 유지");

    // 경계 연도 마지막 학생 삭제 -> 빈 연도 건너뛰고 2020으로 점프
    list.deleteStudent(19750002);
    check(list.getMinYear() == 2020, "빈 연도 갭 건너뛰고 minYear=2020 복구");

    list.deleteStudent(20450001);
    check(list.getMaxYear() == 2020, "빈 연도 갭 건너뛰고 maxYear=2020 복구");
    check(verifyIntegrity(list), "연도 축소 후 무결성");
}

// 5. TeamHashTable 빈 팀 재가입 + 유령 슬롯 누적 + 다중 리사이징
void test05_TeamHashGhostSlotsAndResize() {
    cout << "[Test 05] TeamHashTable 빈 팀 재가입 & 유령 슬롯 누적 테스트\n";
    StudentList list;

    // 1명짜리 팀 30개를 만들고 바로 삭제 반복 (groupCnt는 0~1 유지되지만 슬롯은 계속 소비)
    for (int i = 0; i < 30; ++i) {
        char tName[16];
        sprintf(tName, "GHOST_%d", i);
        list.addStudent(Student(20200000 + i, MyString("Temp"), MyString(tName)));
        check(list.getGroupCount() == 1, "임시 팀 생성");
        check(list.deleteStudent(20200000 + i), "임시 팀 즉시 삭제");
        check(list.getGroupCount() == 0, "임시 팀 소멸");
    }
    check(list.getStudentCount() == 0 && verifyIntegrity(list), "30회 생성/소멸 후 무결성");

    // 애너그램/유사 팀명 충돌 및 대량 팀 리사이즈 (4 -> 8 -> 16 -> 32 -> 64)
    const char* collisionTeams[] = {"AB", "BA", "G1", "1G", "XYZ", "ZYX", "T10", "T01"};
    for (int i = 0; i < 8; ++i) {
        list.addStudent(Student(20210000 + i, MyString("Col"), MyString(collisionTeams[i])));
    }
    for (int i = 0; i < 40; ++i) {
        char tName[16];
        sprintf(tName, "TEAM_%02d", i);
        list.addStudent(Student(20220000 + i, MyString("Bulk"), MyString(tName)));
    }
    check(list.getGroupCount() == 48 && list.getStudentCount() == 48, "48개 팀 다중 리사이즈");
    check(verifyIntegrity(list), "다중 리사이즈 후 무결성");

    // 기존 팀 비웠다가 재가입
    check(list.deleteStudent(20210000), "AB 팀 비우기");
    check(list.getGroupCount() == 47, "AB 소멸 후 팀 47");
    list.addStudent(Student(20250001, MyString("Rejoin"), MyString("AB")));
    check(list.getGroupCount() == 48, "AB 재가입 후 팀 48 복구");
    check(verifyIntegrity(list), "재가입 후 무결성");
}

// 6. 잘못된 입력 / 범위 밖 삭제 / 이중 삭제 / 삭제 후 다른 팀 재삽입
void test06_InvalidInputsAndReinsert() {
    cout << "[Test 06] 예외 입력, 범위 밖 삭제, 이중 삭제, 재삽입 테스트\n";
    StudentList list;

    check(!list.deleteStudent(20240001), "빈 리스트 삭제 -> false");
    check(!list.deleteStudent(0), "빈 리스트 학번 0 -> false");
    check(!list.deleteStudent(-100), "빈 리스트 음수 학번 -> false");

    list.addStudent(Student(20200001, MyString("User1"), MyString("G1")));
    list.addStudent(Student(20240001, MyString("User2"), MyString("-")));

    check(!list.deleteStudent(0), "학번 0 삭제 -> false");
    check(!list.deleteStudent(-20200001), "음수 학번 삭제 -> false");
    check(!list.deleteStudent(19000001), "minYear 미만 삭제 -> false");
    check(!list.deleteStudent(20990001), "maxYear 초과 삭제 -> false");
    check(!list.deleteStudent(20220001), "빈 갭 연도(2022) 삭제 -> false");
    check(!list.deleteStudent(20209999), "유효 연도 내 미존재 학번 -> false");

    // 이중 삭제
    check(list.deleteStudent(20200001), "1차 삭제 -> true");
    check(!list.deleteStudent(20200001), "2차 이중 삭제 -> false");

    // 삭제한 학번을 무소속 -> 팀 변경 재삽입
    list.addStudent(Student(20200001, MyString("User1 New"), MyString("-")));
    check(list.getGroupCount() == 0 && list.getStudentCount() == 2, "삭제 후 무소속 재삽입");
    check(list.deleteStudent(20200001), "무소속 재삭제");
    list.addStudent(Student(20200001, MyString("User1 Team"), MyString("G9")));
    check(list.getGroupCount() == 1 && verifyIntegrity(list), "팀 변경 재삽입 무결성");
}

// 7. 전체 삭제(0명) -> 재삽입 반복 사이클
void test07_EmptyAndReviveCycle() {
    cout << "[Test 07] 전체 삭제(0명) 후 재삽입 사이클 테스트\n";
    StudentList list;

    for (int round = 0; round < 3; ++round) {
        int baseYear = 2000 + round * 10;
        list.addStudent(Student(baseYear * 10000 + 1, MyString("A"), MyString("G1")));
        list.addStudent(Student((baseYear + 5) * 10000 + 2, MyString("B"), MyString("G1")));
        check(list.getMinYear() == baseYear && list.getMaxYear() == baseYear + 5, "사이클 연도 범위");

        list.deleteStudent(baseYear * 10000 + 1);
        list.deleteStudent((baseYear + 5) * 10000 + 2);
        check(list.getStudentCount() == 0 && list.getGroupCount() == 0, "사이클 0명 복귀");
        check(verifyIntegrity(list), "0명 상태 무결성");
    }
}

// 8. FileIO 특수 케이스 (없는 파일, 0명 파일, 무소속 전용 파일, 다중 공백 이름)
void test08_FileIOEdgeCases() {
    cout << "[Test 08] FileIO 예외 및 특수 파일 저장/재로딩 테스트\n";
    StudentList dummy;
    check(!FileIO::loadFile("definitely_not_exist_12345.txt", dummy), "없는 파일 로드 -> false");

    // 8-1. 0명 저장 & 재로딩
    const char* fEmpty = "edge_empty.txt";
    {
        StudentList emptyList;
        check(FileIO::saveFile(fEmpty, emptyList), "0명 저장");
        StudentList reloaded;
        check(FileIO::loadFile(fEmpty, reloaded), "0명 재로딩");
        check(reloaded.getStudentCount() == 0 && reloaded.getGroupCount() == 0, "0명 재로딩 정합성");
        check(verifyIntegrity(reloaded), "0명 재로딩 무결성");
    }

    // 8-2. 무소속만 있는 파일 + 공백 3개 이름 저장 & 재로딩
    const char* fNoTeam = "edge_noteam.txt";
    {
        StudentList list;
        list.addStudent(Student(20230001, MyString("A B C D"), MyString("-")));
        list.addStudent(Student(20210001, MyString("Solo"), MyString("-")));
        check(FileIO::saveFile(fNoTeam, list), "무소속 전용 파일 저장");

        StudentList reloaded;
        check(FileIO::loadFile(fNoTeam, reloaded), "무소속 전용 파일 재로딩");
        check(reloaded.getStudentCount() == 2 && reloaded.getGroupCount() == 0, "무소속 2명 / 팀 0");
        check(reloaded.getMinYear() == 2021 && reloaded.getMaxYear() == 2023, "연도 범위 2021~2023");
        check(verifyIntegrity(reloaded), "무소속 재로딩 무결성");
    }

    // 8-3. 동일 파일 다중 덮어쓰기(Load -> Modify -> Save -> Load -> Modify -> Save -> Load)
    const char* fOverwrite = "edge_overwrite.txt";
    {
        ofstream out(fOverwrite);
        out << "1998 2024\n9 3\n"
            << "20220987 G1 Harley Kim\n20231555 G1 William Jin\n20221004 G2 Thomas Lee\n"
            << "19980123 G1 Jenny Anh\n19980909 G3 Liam Son\n19990131 G2 Owen Park\n"
            << "19981475 - Aria Lee\n20230554 G2 Oliver Kim\n20240178 G1 Nora Kang\n";
        out.close();

        StudentList l1;
        check(FileIO::loadFile(fOverwrite, l1), "1차 로드");
        l1.addStudent(Student(20250907, MyString("Rosa Jang"), MyString("G3")));
        l1.deleteStudent(19981475);
        check(FileIO::saveFile(fOverwrite, l1), "1차 덮어쓰기 저장");

        StudentList l2;
        check(FileIO::loadFile(fOverwrite, l2), "2차 로드");
        check(l2.getMinYear() == 1998 && l2.getMaxYear() == 2025, "2차 로드 연도 (1998~2025)");
        check(l2.getStudentCount() == 9 && l2.getGroupCount() == 3, "2차 로드 인원(9)/팀(3)");
        l2.deleteStudent(19980123);
        l2.deleteStudent(19980909); // 1998년 전부 삭제 -> minYear=1999
        check(FileIO::saveFile(fOverwrite, l2), "2차 덮어쓰기 저장");

        StudentList l3;
        check(FileIO::loadFile(fOverwrite, l3), "3차 로드");
        check(l3.getMinYear() == 1999 && l3.getMaxYear() == 2025, "3차 로드 연도 (1999~2025)");
        check(l3.getStudentCount() == 7 && l3.getGroupCount() == 3, "3차 로드 인원(7)/팀(3)");
        check(verifyIntegrity(l3), "3차 로드 무결성");
    }

    // 생성한 임시 테스트 파일 정리
    remove(fEmpty);
    remove(fNoTeam);
    remove(fOverwrite);
}

// 9. 대규모 스트레스 테스트 (200명, 40년 범위, 25개 팀 + 무소속 혼합 삽입/삭제)
void test09_StressTest() {
    cout << "[Test 09] 대규모 스트레스 테스트 (200명 삽입/절반 삭제/재삽입)\n";
    StudentList* list = new StudentList();

    // 200명 비순차 삽입 (연도 1990~2029, 팀 T0~T24 + 무소속)
    for (int i = 0; i < 200; ++i) {
        int year = 1990 + ((i * 7) % 40);
        int id = year * 10000 + (i + 1);
        char nameBuf[32];
        char teamBuf[16];
        sprintf(nameBuf, "Student %d Name", i);
        if (i % 5 == 0) {
            sprintf(teamBuf, "-");
        } else {
            sprintf(teamBuf, "T%d", i % 25);
        }
        list->addStudent(Student(id, MyString(nameBuf), MyString(teamBuf)));
    }
    check(list->getStudentCount() == 200, "200명 삽입 완료");
    check(verifyIntegrity(*list), "200명 삽입 후 전체 포인터 무결성");

    // 홀수 인덱스 100명 삭제
    for (int i = 1; i < 200; i += 2) {
        int year = 1990 + ((i * 7) % 40);
        int id = year * 10000 + (i + 1);
        check(list->deleteStudent(id), "스트레스 삭제");
    }
    check(list->getStudentCount() == 100, "100명 잔여 확인");
    check(verifyIntegrity(*list), "100명 삭제 후 전체 포인터 무결성");

    // 남은 100명 전부 삭제
    for (int i = 0; i < 200; i += 2) {
        int year = 1990 + ((i * 7) % 40);
        int id = year * 10000 + (i + 1);
        check(list->deleteStudent(id), "스트레스 잔여 삭제");
    }
    check(list->getStudentCount() == 0 && list->getGroupCount() == 0, "200명 전원 삭제 -> 0명/0팀");
    check(verifyIntegrity(*list), "전원 삭제 후 무결성");

    delete list; // 소멸자 정상 해제 확인
}

// 10. 동적 할당 생성/소멸 반복 (메모리 누수 및 double free 여부 확인용)
void test10_LifecycleMemory() {
    cout << "[Test 10] 동적 할당/소멸 반복 테스트\n";
    for (int i = 0; i < 10; ++i) {
        StudentList* p = new StudentList();
        p->addStudent(Student(20240001, MyString("Mem1"), MyString("M1")));
        p->addStudent(Student(20240002, MyString("Mem2"), MyString("M1")));
        p->addStudent(Student(20200001, MyString("Mem3"), MyString("-")));
        check(verifyIntegrity(*p), "동적 객체 무결성");
        delete p;
    }
}

// 11. Tombstone('-') 덮어쓰기 & 선형 탐사 충돌 체인/순환(wrap-around) 검증
void test11_TombstoneCollisionChain() {
    cout << "[Test 11] Tombstone('-') 충돌 체인 & 덮어쓰기 검증\n";
    StudentList list;

    // 기본 capacity(4)에서 충돌 유도: A, B, C 삽입 후 첫 번째 삭제 -> B, C 탐색 유지 확인
    list.addStudent(Student(20240001, MyString("U1"), MyString("A")));
    list.addStudent(Student(20240002, MyString("U2"), MyString("E"))); // 'A'(65)%4 == 'E'(69)%4 == 1
    list.addStudent(Student(20240003, MyString("U3"), MyString("I"))); // 'I'(73)%4 == 1 (충돌 체인 3연속)
    check(list.getGroupCount() == 3, "충돌 팀 3개(A, E, I) 삽입");

    // 체인 맨 앞(A) 삭제 -> '-'로 덮어씀
    check(list.deleteStudent(20240001), "체인 선두(A) 삭제");
    check(list.getGroupCount() == 2, "A 삭제 후 팀 수 2");

    // 체인 뒤쪽(E, I)에 멤버 추가 시 기존 버킷을 정상 탐색하는지 확인 (중복 버킷 생성 방지)
    list.addStudent(Student(20240004, MyString("U4"), MyString("E")));
    list.addStudent(Student(20240005, MyString("U5"), MyString("I")));
    check(list.getGroupCount() == 2, "E, I 멤버 추가 시 중복 팀 생성 안 됨");

    // 새 팀(M: 77%4 == 1) 삽입 -> A가 있던 '-' 슬롯 재사용 확인
    list.addStudent(Student(20240006, MyString("U6"), MyString("M")));
    check(list.getGroupCount() == 3, "M 삽입 시 '-' 슬롯 재사용 (팀 3)");

    // 체인 중간(E) 전원 삭제 후 뒤쪽(I) 삭제
    check(list.deleteStudent(20240002), "E 1차 삭제");
    check(list.deleteStudent(20240004), "E 2차 삭제(팀 소멸)");
    check(list.getGroupCount() == 2, "E 소멸 후 팀 2");
    check(list.deleteStudent(20240003), "I 1차 삭제");
    check(list.deleteStudent(20240005), "I 2차 삭제(팀 소멸)");
    check(list.getGroupCount() == 1 && verifyIntegrity(list), "M만 잔여 & 포인터 무결성");
}

// 12. 파일 포맷 예외 (Windows CRLF '\r\n', 이름 내 특수문자/다중공백, 1명짜리 파일)
void test12_FileFormatEdgeCases() {
    cout << "[Test 12] 파일 포맷 예외 (CRLF, 단일 학생, 특수문자 이름)\n";
    const char* fCrlf = "edge_crlf.txt";
    {
        ofstream out(fCrlf, ios::binary);
        out << "2024 2024\r\n"
            << "2 1\r\n"
            << "20240002 G1 O'Connor, Jr.\r\n"
            << "20240001 - Jean-Luc  Picard\r\n";
        out.close();

        StudentList list;
        check(FileIO::loadFile(fCrlf, list), "CRLF(\\r\\n) 파일 로드");
        check(list.getStudentCount() == 2 && list.getGroupCount() == 1, "CRLF 로드 인원(2)/팀(1)");
        check(verifyIntegrity(list), "CRLF 로드 무결성");

        // 이름 끝에 '\r'이 딸려 들어오지 않았는지 확인
        StudentNode* head = list.getYearTable().getBucketByYear(2024).getHead();
        if (head != nullptr) {
            const MyString& nm = head->data.getStudentName();
            check(nm.length() > 0 && nm[nm.length() - 1] != '\r', "이름 끝 '\\r' 미포함 확인");
        }
    }
    remove(fCrlf);
}

// 13. initYearRange 직접 호출 후 범위 밖 확장 & 동일 학번 연도 경계(0000 / 9999)
void test13_YearBoundaryAndInit() {
    cout << "[Test 13] initYearRange 및 학번 경계값(xxxx0000, xxxx9999)\n";
    StudentList list;
    list.initYearRange(2020, 2020); // capacity 1인 상태로 시작

    list.addStudent(Student(20200000, MyString("MinID"), MyString("T1")));
    list.addStudent(Student(20209999, MyString("MaxID"), MyString("T1")));
    list.addStudent(Student(20199999, MyString("PrevYearMax"), MyString("T1"))); // 2019로 확장
    list.addStudent(Student(20210000, MyString("NextYearMin"), MyString("T1"))); // 2021로 확장

    check(list.getMinYear() == 2019 && list.getMaxYear() == 2021, "경계 학번 연도 확장 (2019~2021)");
    check(list.getStudentCount() == 4 && list.getGroupCount() == 1, "인원(4)/팀(1)");
    check(verifyIntegrity(list), "경계 학번 포인터 무결성");
}

// 14. 500회 랜덤 퍼징(Fuzzing) - 단순 C배열(정답지)과 매 스텝 교차 검증
void test14_RandomFuzzingOracle() {
    cout << "[Test 14] 500회 랜덤 삽입/삭제 퍼징(Fuzzing) 교차 검증\n";
    srand(42); // 재현 가능한 고정 시드

    const int MAX_REF = 300;
    int refID[MAX_REF];
    MyString refTeam[MAX_REF];
    int refCount = 0;

    const char* teamPool[] = {"-", "-", "G1", "G2", "G3", "A", "E", "I", "M", "T10", "T20", "Z9"};
    const int poolSize = 12;

    StudentList list;

    for (int step = 0; step < 500; ++step) {
        int action = rand() % 10; // 0~5: 삽입(60%), 6~8: 기존 삭제(30%), 9: 없는 학번 삭제(10%)

        if (action < 6 && refCount < MAX_REF) {
            int year = 1995 + (rand() % 35); // 1995~2029
            int id = year * 10000 + (rand() % 9000 + 1);

            // 중복 학번 제외
            bool dup = false;
            for (int i = 0; i < refCount; ++i) {
                if (refID[i] == id) { dup = true; break; }
            }
            if (dup) continue;

            const char* t = teamPool[rand() % poolSize];
            list.addStudent(Student(id, MyString("Fuzz User"), MyString(t)));
            refID[refCount] = id;
            refTeam[refCount] = MyString(t);
            refCount++;
        } else if (action < 9 && refCount > 0) {
            int pick = rand() % refCount;
            int targetID = refID[pick];
            check(list.deleteStudent(targetID), "퍼징 기존 학번 삭제");

            // 정답지 배열에서 제거
            refID[pick] = refID[refCount - 1];
            refTeam[pick] = refTeam[refCount - 1];
            refCount--;
        } else {
            int fakeID = 20500000 + (rand() % 9999);
            check(!list.deleteStudent(fakeID), "퍼징 미존재 학번 삭제 -> false");
        }

        // 정답지 기준 minYear, maxYear, groupCount 계산
        int expMin = 9999, expMax = 0, expGroups = 0;
        for (int i = 0; i < refCount; ++i) {
            int y = refID[i] / 10000;
            if (y < expMin) expMin = y;
            if (y > expMax) expMax = y;
            if (refTeam[i] != "-") {
                bool seen = false;
                for (int j = 0; j < i; ++j) {
                    if (refTeam[j] == refTeam[i]) { seen = true; break; }
                }
                if (!seen) expGroups++;
            }
        }

        bool ok = (list.getStudentCount() == refCount) && (list.getGroupCount() == expGroups);
        if (refCount > 0) {
            ok = ok && (list.getMinYear() == expMin) && (list.getMaxYear() == expMax);
        }
        if (!ok || !verifyIntegrity(list)) {
            cout << "  [FAIL] Fuzzing step " << step << " 불일치! "
                 << "(exp cnt=" << refCount << ", grp=" << expGroups
                 << " / got cnt=" << list.getStudentCount() << ", grp=" << list.getGroupCount() << ")\n";
            gFail++;
            break;
        }
    }
    check(true, "500회 랜덤 퍼징 완료");
}

// 15. 파일 마지막 줄 개행(\n) 없음 & 빈 줄 다수 & 초장문 이름(MyString 다중 리사이즈)
void test15_FileEOFAndLongLine() {
    cout << "[Test 15] EOF 개행 누락 / 끝 빈줄 / 초장문 이름 테스트\n";
    const char* fNoNewline = "edge_no_nl.txt";
    {
        ofstream out(fNoNewline);
        // 마지막 줄 뒤에 '\n'이 없는 경우 + 이름 길이가 MyString 초기 capacity(8)를 크게 초과
        out << "2024 2024\n"
            << "3 1\n"
            << "20240001 - -\n" // 팀ID도 '-', 이름도 '-'
            << "20240002 G1 A\n" // 1글자 이름
            << "20240003 G1 SuperLongStudentNameThatForcesMultipleMyStringResizesOver100Chars_1234567890_ABCDEFGHIJ";
        out.close();

        StudentList list;
        check(FileIO::loadFile(fNoNewline, list), "개행 없는 EOF 파일 로드");
        check(list.getStudentCount() == 3 && list.getGroupCount() == 1, "마지막 줄 누락 없이 3명/1팀 로드");
        check(verifyIntegrity(list), "로드 무결성");
    }
    remove(fNoNewline);

    const char* fTrailingBlank = "edge_blank_lines.txt";
    {
        ofstream out(fTrailingBlank);
        out << "2024 2024\n1 1\n20240001 G1 Only One\n\n\n"; // 끝에 빈 줄 여러 개
        out.close();

        StudentList list;
        check(FileIO::loadFile(fTrailingBlank, list), "끝 빈 줄 포함 파일 로드");
        check(list.getStudentCount() == 1 && list.getGroupCount() == 1, "빈 줄 무시 후 1명/1팀");
        check(verifyIntegrity(list), "빈 줄 파일 무결성");
    }
    remove(fTrailingBlank);
}

// 16. Tombstone('-')로 빈 칸("")이 0개가 된 직후 find/insert/delete (무한루프 방지 검증)
void test16_ZeroEmptySlotsTombstone() {
    cout << "[Test 16] 빈 슬롯(\"\") 0개 + Tombstone('-') 포화 상태 테스트\n";
    StudentList list;

    // capacity=4 가정: 0,1,2 채움 -> 0 삭제('-') -> 3 채움 -> 빈 칸("")이 아예 없는 상태 유도
    list.addStudent(Student(20240001, MyString("U0"), MyString("D"))); // 'D'(68)%4 == 0
    list.addStudent(Student(20240002, MyString("U1"), MyString("A"))); // 'A'(65)%4 == 1
    list.addStudent(Student(20240003, MyString("U2"), MyString("B"))); // 'B'(66)%4 == 2
    list.deleteStudent(20240001);                                      // slot 0 = '-' (groupCnt=2)
    list.addStudent(Student(20240004, MyString("U3"), MyString("C"))); // 'C'(67)%4 == 3 -> "" 슬롯 0개!

    // "" 슬롯이 하나도 없을 때 없는 팀 탐색/삭제 및 기존 팀 멤버 추가 (무한 루프 안 도는지 확인)
    list.addStudent(Student(20240005, MyString("U4"), MyString("C"))); // 기존 팀 C 탐색
    check(list.getGroupCount() == 3, "포화 상태에서 기존 팀 C 탐색 성공");

    // 모든 팀 삭제 -> 슬롯 4개 전부 '-'
    list.deleteStudent(20240002);
    list.deleteStudent(20240003);
    list.deleteStudent(20240004);
    list.deleteStudent(20240005);
    check(list.getGroupCount() == 0, "전 슬롯 Tombstone 상태 (팀 0)");

    // 전부 '-'인 테이블에 새 팀 삽입
    list.addStudent(Student(20240006, MyString("U5"), MyString("Z")));
    check(list.getGroupCount() == 1 && verifyIntegrity(list), "전 슬롯 '-' 상태에서 재삽입 무결성");
}

// 17. YearHashTable 리사이즈 후에도 기존 Team 원형 포인터가 살아있는지 검증
void test17_CrossTablePointerAfterYearResize() {
    cout << "[Test 17] YearHashTable 다중 리사이즈 후 Team 원형 포인터 유지 테스트\n";
    StudentList list;

    // 같은 팀(G_ALL) 학생들을 연도 확장을 유발하며 추가 (노드 주소가 유지되어야 함)
    list.addStudent(Student(20200001, MyString("Y2020"), MyString("G_ALL")));
    list.addStudent(Student(20250001, MyString("Y2025"), MyString("G_ALL"))); // 우측 리사이즈
    list.addStudent(Student(20100001, MyString("Y2010"), MyString("G_ALL"))); // 좌측 리사이즈
    list.addStudent(Student(20350001, MyString("Y2035"), MyString("G_ALL"))); // 우측 재리사이즈
    list.addStudent(Student(19900001, MyString("Y1990"), MyString("G_ALL"))); // 좌측 재리사이즈

    check(list.getStudentCount() == 5 && list.getGroupCount() == 1, "연도 리사이즈 4회 후 5명/1팀");
    check(verifyIntegrity(list), "Year 리사이즈 후 Team 원형/이중 포인터 무결성");

    // 가운데 노드 삭제 후 양쪽 연도 연결 확인
    check(list.deleteStudent(20200001), "중앙 노드 삭제");
    check(verifyIntegrity(list), "중앙 삭제 후 무결성");
}

// 18. 대소문자 팀명 구분 & 숫자/기호 팀명
void test18_CaseSensitiveTeams() {
    cout << "[Test 18] 대소문자 팀명 구분 (G1 vs g1) 및 특수 팀명 테스트\n";
    StudentList list;
    list.addStudent(Student(20240001, MyString("Upper"), MyString("G1")));
    list.addStudent(Student(20240002, MyString("Lower"), MyString("g1")));
    list.addStudent(Student(20240003, MyString("Num"), MyString("123")));
    list.addStudent(Student(20240004, MyString("Sym"), MyString("@#$")));

    check(list.getGroupCount() == 4, "G1, g1, 123, @#$ -> 서로 다른 4팀");
    check(list.deleteStudent(20240001), "G1 삭제");
    check(list.getGroupCount() == 3 && verifyIntegrity(list), "G1 삭제 시 g1 유지 및 무결성");
}

// 19. 파일 헤더(minYear maxYear)가 실제 데이터보다 좁거나 틀린 파일 로드 테스트
void test19_MismatchedFileHeader() {
    cout << "[Test 19] 파일 첫 줄 연도 범위가 실제 학생과 다를 때 로드 테스트\n";
    const char* fMismatch = "edge_mismatch.txt";
    {
        ofstream out(fMismatch);
        out << "2020 2021\n" // 헤더는 2020~2021인데 실제 데이터는 1995, 2026 포함
            << "2 1\n"
            << "19950001 G1 Old Student\n"
            << "20260001 G1 New Student\n";
        out.close();

        StudentList list;
        check(FileIO::loadFile(fMismatch, list), "헤더 불일치 파일 로드");
        check(list.getMinYear() == 1995 && list.getMaxYear() == 2026, "실제 데이터 기준 연도 자동 보정 (1995~2026)");
        check(list.getStudentCount() == 2 && list.getGroupCount() == 1, "인원(2)/팀(1)");
        check(verifyIntegrity(list), "헤더 불일치 로드 무결성");
    }
    remove(fMismatch);
}
// 20. 중복 학번 삽입 차단 테스트 (같은 팀 / 다른 팀 / 무소속)
void test20_DuplicateStudentID() {
    cout << "[Test 20] 중복 학번 삽입 차단 테스트\n";
    StudentList list;

    list.addStudent(Student(20240001, MyString("Original"), MyString("G1")));

    // 1) 완전 동일한 학번 + 같은 팀 재삽입
    list.addStudent(Student(20240001, MyString("Dup SameTeam"), MyString("G1")));
    // 2) 동일한 학번 + 새로운 팀(G2) 재삽입 -> 팀 수가 늘어나면 안 됨
    list.addStudent(Student(20240001, MyString("Dup DiffTeam"), MyString("G2")));
    // 3) 동일한 학번 + 무소속('-') 재삽입
    list.addStudent(Student(20240001, MyString("Dup NoTeam"), MyString("-")));

    check(list.getStudentCount() == 1, "중복 학번 무시 -> 인원(1) 유지");
    check(list.getGroupCount() == 1, "중복 학번의 새 팀(G2) 생성 차단 -> 팀(1) 유지");
    check(verifyIntegrity(list), "중복 삽입 시도 후 포인터 무결성");

    // 1번 삭제했을 때 완전히 사라져야 함 (중복 노드가 남아있으면 안 됨)
    check(list.deleteStudent(20240001), "원본 학번 삭제");
    check(list.getStudentCount() == 0 && list.getGroupCount() == 0, "삭제 후 잔여 중복 노드 없음");
}

// 21. [1번 엣지] 중복 검사 시 빈 테이블 / 범위 밖 연도 조회 크래시 검증
void test21_DuplicateCheckBounds() {
    cout << "[Test 21] 빈 테이블 및 범위 밖 연도 중복 검사 안전성 테스트\n";
    StudentList list;

    // 빈 테이블 상태에서 첫 삽입 (중복 검사 로직이 nullptr/빈 버킷에서 안 터지는지)
    list.addStudent(Student(20200001, MyString("First"), MyString("G1")));

    // 현재 범위(2020~2020)보다 과거/미래 연도 삽입 시 중복 검사에서 범위 초과 안 나는지
    list.addStudent(Student(19900001, MyString("Past"), MyString("G1")));
    list.addStudent(Student(20350001, MyString("Future"), MyString("G1")));

    // 방금 확장된 양 끝 경계 연도에서 중복 삽입 차단 확인
    list.addStudent(Student(19900001, MyString("DupPast"), MyString("G2")));
    list.addStudent(Student(20350001, MyString("DupFuture"), MyString("G3")));

    check(list.getStudentCount() == 3 && list.getGroupCount() == 1, "범위 밖 확장 및 경계 중복 차단");
    check(verifyIntegrity(list), "포인터 무결성");
}

// 22. [2번 엣지] 콘솔 입력 시뮬레이션 (즉시 Q, 오타/소문자, 이름 앞 다중 공백, D 비숫자 입력)
// 주의: 헤더의 클래스명 오타(StudentListManger) 그대로 사용
void test22_ConsoleManagerInput() {
    cout << "[Test 22] StudentListManger 콘솔 입력 엣지 테스트\n";
    const char* fMgr = "edge_mgr.txt";

    // 22-1. 수정 없이 즉시 Q 종료 시 원본 메타데이터 유지 확인
    {
        ofstream out(fMgr);
        out << "1998 2024\n2 1\n19980123 G1 Jenny Anh\n20240178 G1 Nora Kang\n";
        out.close();

        istringstream fakeIn("Q\n");
        streambuf* origCin = cin.rdbuf(fakeIn.rdbuf());
        {
            StudentListManger mgr(fMgr);
            mgr.run();
        }
        cin.rdbuf(origCin);

        StudentList checkList;
        check(FileIO::loadFile(fMgr, checkList), "즉시 Q 종료 후 파일 재로딩");
        check(checkList.getMinYear() == 1998 && checkList.getMaxYear() == 2024, "즉시 Q 후 연도 유지");
        check(checkList.getStudentCount() == 2 && checkList.getGroupCount() == 1, "즉시 Q 후 인원(2)/팀(1)");
    }

    // 22-2. 잘못된 명령어, 이름 앞 다중 공백, D에 문자 입력("abc") 후 정상 Q 종료
    // (주의: cin.clear()/ignore() 미구현 시 "abc"에서 무한루프 발생 가능)
    {
        istringstream fakeIn(
            "X\n"                          // 잘못된 명령어
            "I\n20250001 G2    Space Name\n" // 팀ID 뒤 다중 공백
            "D\nabc\n"                     // 숫자 대신 문자 입력
            "D\n19980123\n"                // 정상 삭제
            "Q\n"
        );
        streambuf* origCin = cin.rdbuf(fakeIn.rdbuf());
        {
            StudentListManger mgr(fMgr);
            mgr.run();
        }
        cin.rdbuf(origCin);

        StudentList checkList;
        FileIO::loadFile(fMgr, checkList);
        check(checkList.getStudentCount() == 2 && checkList.getGroupCount() == 2, "예외 입력 복구 후 인원(2)/팀(2)");

        if (checkList.getMaxYear() >= 2025) {
            StudentNode* n = checkList.getYearTable().getBucketByYear(2025).getHead();
            if (n != nullptr) {
                const MyString& nm = n->data.getStudentName();
                check(nm.length() > 0 && nm[0] != ' ', "이름 맨 앞 공백 제거 확인");
            }
        }
    }
    remove(fMgr);
}

// 23. [3번 엣지] FileIO 파싱 (파일 내 중복 학번, 필드 간 다중 공백/탭)
void test23_FileIODuplicateAndSpaces() {
    cout << "[Test 23] FileIO 파일 내 중복 학번 및 다중 공백 파싱 테스트\n";
    const char* fParse = "edge_parse.txt";
    {
        ofstream out(fParse);
        out << "2024 2024\n"
            << "3 2\n"
            << "20240001    G1     Multi Space Name\n" // 필드 사이 다중 공백
            << "20240001 G2 Duplicate In File\n"       // 파일 내 중복 학번
            << "20240002   -    No Team Spaces\n";
        out.close();

        StudentList list;
        check(FileIO::loadFile(fParse, list), "다중 공백/중복 포함 파일 로드");
        check(list.getStudentCount() == 2 && list.getGroupCount() == 1, "파일 내 중복 무시 -> 인원(2)/팀(1)");

        StudentNode* head = list.getYearTable().getBucketByYear(2024).getHead();
        if (head != nullptr) {
            const MyString& nm = head->data.getStudentName();
            check(nm.length() > 0 && nm[0] != ' ', "파일 로드 시 이름 앞 공백 미포함");
        }
        check(verifyIntegrity(list), "파싱 후 포인터 무결성");
    }
    remove(fParse);
}

// 24. [4번 엣지] MyString 자기 대입(s = s) 및 빈 문자열 대입 안전성
void test24_MyStringSelfAssign() {
    cout << "[Test 24] MyString 자기 대입(s = s) 안전성 테스트\n";
    MyString s("Hello World");
    MyString& ref = s;
    s = ref; // 자기 대입 시 delete[] 후 댕글링 포인터 크래시 방지 확인

    check(s == "Hello World" && s.length() == 11, "자기 대입 후 문자열/길이 보존");

    MyString empty("");
    MyString& emptyRef = empty;
    empty = emptyRef;
    check(empty == "" && empty.length() == 0, "빈 문자열 자기 대입 안전성");
}

// 25. 0바이트 빈 파일, 공백 파일, "0 0 / 0 0" 파일, 데이터 누락 파일 테스트
void test25_EmptyAndZeroFiles() {
    cout << "[Test 25] 0바이트 빈 파일 & '0 0 / 0 0' 입력 테스트\n";

    // 25-1. 완전 빈 파일 (0 byte)
    const char* fZeroByte = "edge_0byte.txt";
    {
        ofstream out(fZeroByte); // 아무것도 안 씀
        out.close();

        StudentList list;
        // 로드 결과(true/false)는 구현 정책에 따르되, 크래시 없이 0명 상태여야 함
        FileIO::loadFile(fZeroByte, list);
        check(list.getStudentCount() == 0 && list.getGroupCount() == 0, "0바이트 파일 로드 후 0명/0팀");
        check(verifyIntegrity(list), "0바이트 로드 후 무결성");

        // 로드 실패/빈 상태 이후에도 정상 삽입 가능해야 함
        list.addStudent(Student(20240001, MyString("After Empty"), MyString("G1")));
        check(list.getMinYear() == 2024 && list.getMaxYear() == 2024, "0바이트 후 삽입 시 연도 정상 (2024)");
        check(verifyIntegrity(list), "0바이트 후 삽입 무결성");
    }
    remove(fZeroByte);

    // 25-2. 공백/개행만 있는 파일
    const char* fSpaces = "edge_spaces.txt";
    {
        ofstream out(fSpaces);
        out << "   \n\t\n  ";
        out.close();

        StudentList list;
        FileIO::loadFile(fSpaces, list);
        check(list.getStudentCount() == 0 && list.getGroupCount() == 0, "공백 전용 파일 -> 0명/0팀");
    }
    remove(fSpaces);

    // 25-3. "0 0\n0 0\n" 입력 파일
    const char* fZeros = "edge_zeros.txt";
    {
        ofstream out(fZeros);
        out << "0 0\n0 0\n";
        out.close();

        StudentList list;
        FileIO::loadFile(fZeros, list);
        check(list.getStudentCount() == 0 && list.getGroupCount() == 0, "'0 0 / 0 0' 로드 후 0명/0팀");

        // 핵심: 이후 2024 학번 추가 시 minYear가 0으로 남지 않고 2024~2024로 잡혀야 함
        list.addStudent(Student(20240001, MyString("After Zero"), MyString("G1")));
        check(list.getMinYear() == 2024 && list.getMaxYear() == 2024, "'0 0' 후 삽입 시 minYear=2024 (0 아님)");
        check(list.getStudentCount() == 1 && list.getGroupCount() == 1, "'0 0' 후 삽입 인원(1)/팀(1)");
        check(verifyIntegrity(list), "'0 0' 후 삽입 무결성");
    }
    remove(fZeros);

    // 25-4. 헤더엔 5명이라 적혀있는데 실제 학생 줄이 없는(잘린) 파일
    const char* fTrunc = "edge_trunc.txt";
    {
        ofstream out(fTrunc);
        out << "2020 2024\n5 2\n";
        out.close();

        StudentList list;
        FileIO::loadFile(fTrunc, list);
        check(list.getStudentCount() == 0 && list.getGroupCount() == 0, "데이터 누락 파일 -> 크래시 없이 0명/0팀");
        check(verifyIntegrity(list), "데이터 누락 파일 무결성");
    }
    remove(fTrunc);
}

// 26. FileIO::saveFile 실제 파일 텍스트 포맷 바이트 단위 검증
void test26_SaveFileExactFormat() {
    cout << "[Test 26] FileIO::saveFile 실제 텍스트 포맷 정확성 테스트\n";

    // 파일 전체 내용을 그대로 MyString으로 읽어오는 헬퍼
    auto readAll = [](const char* path) -> MyString {
        ifstream fin(path);
        MyString content;
        char c;
        while (fin.get(c)) {
            content.append(c);
        }
        return content;
    };

    // 26-1. 정상 데이터 저장 (역순 삽입 -> 학번 오름차순 저장, ""와 "-" 모두 "-"로 출력 확인)
    const char* fOut = "edge_save_exact.txt";
    {
        StudentList list;
        list.addStudent(Student(20240002, MyString("Bob Park"), MyString("G1")));
        list.addStudent(Student(20220001, MyString("Alice Kim"), MyString("-")));
        list.addStudent(Student(20240001, MyString("Charlie Lee"), MyString(""))); // 빈 팀ID도 "-"로 저장되어야 함

        check(FileIO::saveFile(fOut, list), "정상 리스트 저장 -> true");

        MyString actual = readAll(fOut);
        const char* expected =
            "2022 2024\n"
            "3 1\n"
            "20220001 - Alice Kim\n"
            "20240001 - Charlie Lee\n"
            "20240002 G1 Bob Park\n";

        check(actual == expected, "저장된 파일 텍스트 100% 일치");
        if (actual != expected) {
            cout << "--- [Expected] ---\n" << expected
                 << "--- [Actual] ---\n" << actual << "------------------\n";
        }
    }
    remove(fOut);

    // 26-2. 빈 리스트(0명) 저장 포맷 검증
    const char* fEmptyOut = "edge_save_empty.txt";
    {
        StudentList emptyList;
        check(FileIO::saveFile(fEmptyOut, emptyList), "0명 리스트 저장 -> true");

        MyString actual = readAll(fEmptyOut);
        const char* expected = "0 0\n0 0\n";
        check(actual == expected, "0명 저장 포맷(\"0 0\\n0 0\\n\") 일치");
        if (actual != expected) {
            cout << "--- [Actual Empty] ---\n" << actual << "----------------------\n";
        }
    }
    remove(fEmptyOut);

    // 26-3. 존재하지 않는 경로 저장 시 크래시 없이 false 반환 확인
    StudentList dummy;
    check(!FileIO::saveFile("nonexistent_dir_9999/out.txt", dummy), "잘못된 경로 저장 -> false");
}

// 27. 범위 밖 연도 & 비정상 자릿수 학번 차단 테스트 (직접 삽입 / 파일 / 콘솔)
void test27_OutOfRangeYearInputs() {
    cout << "[Test 27] 범위 밖 연도 및 비정상 학번 차단 테스트\n";

    // 설정한 허용 범위에 맞춰 경계값 조정 (예: 1970~2050 또는 1900~2100)
    const int MIN_VALID_YEAR = 1900;
    const int MAX_VALID_YEAR = 2100;

    // 27-1. StudentList::addStudent 직접 호출 차단 확인
    {
        StudentList list;
        list.addStudent(Student(20240001, MyString("Valid"), MyString("G1")));

        // 극단값 및 경계 바깥값
        list.addStudent(Student(0, MyString("Zero"), MyString("G2")));
        list.addStudent(Student(-20240001, MyString("Neg"), MyString("G2")));
        list.addStudent(Student(18000001, MyString("TooOld"), MyString("G2")));
        list.addStudent(Student(99999999, MyString("Max9999"), MyString("G2")));
        list.addStudent(Student((MIN_VALID_YEAR - 1) * 10000 + 1, MyString("UnderMin"), MyString("G2")));
        list.addStudent(Student((MAX_VALID_YEAR + 1) * 10000 + 1, MyString("OverMax"), MyString("G2")));

        check(list.getStudentCount() == 1 && list.getGroupCount() == 1, "직접 삽입: 범위 밖 학번 차단 -> 1명/1팀 유지");
        check(list.getMinYear() == 2024 && list.getMaxYear() == 2024, "직접 삽입: 연도 범위(2024) 오염 없음");
        check(verifyIntegrity(list), "직접 삽입 후 무결성");
    }

    // 27-2. FileIO::loadFile 비정상 헤더(1998 9999) + 비정상 학번 줄 혼합 로드
    const char* fBadYear = "edge_bad_year.txt";
    {
        ofstream out(fBadYear);
        out << "1998 9999\n"                        // 비정상 헤더 범위 (메모리 폭주 방지)
            << "6 2\n"
            << "99999999 G1 Year 9999\n"            // 최대값 범위 초과
            << "18000001 G1 Year 1800\n"            // 과거 범위 미만
            << "2024001 G1 Seven Digits\n"          // 7자리 학번
            << "99999999999 G1 Int Overflow\n"      // 11자리 오버플로우 유발
            << "00000001 G1 Year Zero\n"            // 0000년
            << "20240001 G2 Real Student\n";        // 유일한 정상 데이터
        out.close();

        StudentList list;
        check(FileIO::loadFile(fBadYear, list), "비정상 학번 포함 파일 로드");
        check(list.getStudentCount() == 1 && list.getGroupCount() == 1, "파일 로드: 정상 1명/1팀만 반영");
        check(list.getMinYear() == 2024 && list.getMaxYear() == 2024, "파일 로드: 연도 범위(2024) 유지");
        check(verifyIntegrity(list), "파일 로드 후 무결성");
    }
    remove(fBadYear);

    // 27-3. 콘솔 입력(StudentListManger) I 명령어 범위 밖 연도 / 자릿수 차단
    const char* fMgrYear = "edge_mgr_year.txt";
    {
        ofstream out(fMgrYear);
        out << "2024 2024\n1 1\n20240001 G1 Base User\n";
        out.close();

        istringstream fakeIn(
            "I\n99999999 G9 Too Far\n"
            "I\n18000001 G9 Too Old\n"
            "I\n2024001 G9 Short ID\n"
            "I\n123456789012 G9 Overflow ID\n"
            "I\n20250001 G2 Valid User\n"
            "Q\n"
        );
        streambuf* origCin = cin.rdbuf(fakeIn.rdbuf());
        {
            StudentListManger mgr(fMgrYear);
            mgr.run();
        }
        cin.rdbuf(origCin);

        StudentList checkList;
        check(FileIO::loadFile(fMgrYear, checkList), "콘솔 예외 입력 후 저장 파일 재로딩");
        check(checkList.getStudentCount() == 2 && checkList.getGroupCount() == 2, "콘솔 입력: 정상 2명/2팀만 반영");
        check(checkList.getMinYear() == 2024 && checkList.getMaxYear() == 2025, "콘솔 입력: 연도 범위(2024~2025) 유지");
        check(verifyIntegrity(checkList), "콘솔 입력 후 무결성");
    }
    remove(fMgrYear);
}

int main() {
    test01_StudentAndTeamID();
    test02_SinglyListPositions();
    test03_CircularListPositions();
    test04_YearHashResizeAndGaps();
    test05_TeamHashGhostSlotsAndResize();
    test06_InvalidInputsAndReinsert();
    test07_EmptyAndReviveCycle();
    test08_FileIOEdgeCases();
    test09_StressTest();
    test10_LifecycleMemory();
    test11_TombstoneCollisionChain();
    test12_FileFormatEdgeCases();
    test13_YearBoundaryAndInit();
    test14_RandomFuzzingOracle();
    test15_FileEOFAndLongLine();
    test16_ZeroEmptySlotsTombstone();
    test17_CrossTablePointerAfterYearResize();
    test18_CaseSensitiveTeams();
    test19_MismatchedFileHeader();
    test20_DuplicateStudentID();
    test21_DuplicateCheckBounds();
    test22_ConsoleManagerInput();
    test23_FileIODuplicateAndSpaces();
    test24_MyStringSelfAssign();
    test25_EmptyAndZeroFiles();
    test26_SaveFileExactFormat();
    test27_OutOfRangeYearInputs();


    cout << "\n========================================\n";
    cout << "TOTAL PASS: " << gPass << " / FAIL: " << gFail << "\n";
    cout << "========================================\n";
    return gFail == 0 ? 0 : 1;
}