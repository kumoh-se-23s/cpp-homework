#include <iostream>

#include "App.h"
#include "DoubleNode.h"
#include "NestedDoubleNode.h"
#include "SingleNode.h"
#include "SinglyLinkedList.h"

using namespace std;
//
// int main(int argc, char* argv[]) {
//        App app;
//     app.start();
// }
void printSection(const char *title, const StudentController &controller) {
    std::cout << "===== " << title << " =====\n";
    std::cout << controller << "\n";
}

int main() {
    StudentController controller;

    // 1. 기본 삽입 (같은 학번대/다른 학번대, 같은 그룹/다른 그룹/그룹 없음 "-")
    controller.insertStudent(20230001, "Alpha", "Kim");
    controller.insertStudent(20230002, "Alpha", "Lee");
    controller.insertStudent(20230003, "Alpha", "Park");
    controller.insertStudent(20240001, "Beta", "Choi");
    controller.insertStudent(20240002, "-", "Jung");
    printSection("1. 초기 삽입 후", controller);

    // 2. 그룹 내 중간 노드 삭제 (20230002) -> Alpha는 Kim - Park 만 남아야 함
    controller.deleteStudent(20230002);
    printSection("2. Alpha 중간(20230002) 삭제 후", controller);

    // 3. 그룹 내 head 노드 삭제 (20230001) -> Alpha는 Park 혼자 남아 그룹 출력 생략되어야 함
    controller.deleteStudent(20230001);
    printSection("3. Alpha head(20230001) 삭제 후", controller);

    // 4. 그룹 내 마지막 남은 노드 삭제 (20230003)
    controller.deleteStudent(20230003);
    printSection("4. Alpha 마지막(20230003) 삭제 후", controller);

    // 5. 그룹이 없는 학생("-") 삭제 (20240002)
    controller.deleteStudent(20240002);
    printSection("5. 그룹 없는 학생(20240002) 삭제 후", controller);

    // 6. 존재하지 않는 학번 삭제 (같은 연도 있음 / 연도 자체가 없음)
    controller.deleteStudent(20249999);
    controller.deleteStudent(20190001);
    printSection("6. 없는 학번 삭제 시도 후 (변화 없어야 함)", controller);

    // 7. 비워진 그룹(Alpha) 및 연도(2023)에 재삽입
    controller.insertStudent(20230010, "Alpha", "Han");
    controller.insertStudent(20230020, "Alpha", "Yoon");
    printSection("7. 비워진 그룹/연도 재삽입 후", controller);

    // 8. 전체 삭제 후 빈 상태 출력 확인
    controller.deleteStudent(20240001);
    controller.deleteStudent(20230010);
    controller.deleteStudent(20230020);
    printSection("8. 전체 삭제 후 (비어 있어야 함)", controller);

    std::cout << "모든 테스트 완료\n";
    return 0;
}