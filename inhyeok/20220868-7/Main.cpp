//
// Edge case only main
//

#include "MyString.h"
#include <iostream>
#include <fstream>

#include "StudentTable.h"

static void section(const char *title) {
    std::cout << "\n===== " << title << " =====" << std::endl;
}

static void show(const StudentTable &table) {
    std::cout << table << std::endl;
}

static Student s(const int num, const char *name = "", const char *group = "") {
    return Student(num, MyString(name), MyString(group));
}

int main() {
    // 1. 빈 테이블
    section("1. empty table");
    {
        StudentTable table;
        show(table);
    }

    // 2. 빈 테이블에서 삭제
    section("2. remove from empty table");
    {
        StudentTable table;
        table.removeStudent(20220868);
        show(table);
    }

    // 3. 학생 1명 (그룹 없음)
    section("3. single student, no group");
    {
        StudentTable table;
        table.addStudent(s(20220868, "Kim"));
        show(table);
    }

    // 4. 학생 1명 (그룹 있음, 팀원 본인뿐)
    section("4. single student, solo group");
    {
        StudentTable table;
        table.addStudent(s(20220868, "Kim", "G1"));
        show(table);
    }

    // 5. 유일한 학생 삭제 후 출력
    section("5. remove the only student");
    {
        StudentTable table;
        table.addStudent(s(20220868, "Kim", "G1"));
        table.removeStudent(20220868);
        show(table);
    }

    // 6. 존재하지 않는 학번 삭제 (연도는 존재 / 연도 자체가 없음)
    section("6. remove non-existent number");
    {
        StudentTable table;
        table.addStudent(s(20220868, "Kim", "G1"));
        table.removeStudent(20220001); // 같은 연도, 없는 학번
        table.removeStudent(19990001); // 없는 연도
        show(table);
    }

    // 7. 같은 학번 두 번 삭제
    section("7. remove same number twice");
    {
        StudentTable table;
        table.addStudent(s(20220868, "Kim", "G1"));
        table.addStudent(s(20220869, "Lee", "G1"));
        table.removeStudent(20220868);
        table.removeStudent(20220868);
        show(table);
    }

    // 8. 연도 삽입 순서: 오름차순 / 내림차순 / 중간 삽입
    section("8. year insertion order");
    {
        StudentTable asc;
        asc.addStudent(s(20200001, "A"));
        asc.addStudent(s(20210001, "B"));
        asc.addStudent(s(20220001, "C"));
        show(asc);

        StudentTable desc;
        desc.addStudent(s(20220001, "C"));
        desc.addStudent(s(20210001, "B"));
        desc.addStudent(s(20200001, "A"));
        show(desc);

        StudentTable mid;
        mid.addStudent(s(20200001, "A"));
        mid.addStudent(s(20220001, "C"));
        mid.addStudent(s(20210001, "B"));
        show(mid);
    }

    // 9. 같은 연도 다수 학생 (addFirst 순서 확인)
    section("9. many students in same year");
    {
        StudentTable table;
        table.addStudent(s(20220001, "A"));
        table.addStudent(s(20220002, "B"));
        table.addStudent(s(20220003, "C"));
        show(table);
    }

    // 10. 같은 연도의 연도 리스트 head / 중간 / tail 삭제
    section("10. remove head / middle / tail in same year");
    {
        StudentTable table;
        table.addStudent(s(20220001, "A"));
        table.addStudent(s(20220002, "B"));
        table.addStudent(s(20220003, "C"));
        table.addStudent(s(20220004, "D"));
        table.removeStudent(20220004); // head (마지막에 추가된 것)
        show(table);
        table.removeStudent(20220002); // middle
        show(table);
        table.removeStudent(20220001); // tail
        show(table);
        table.removeStudent(20220003); // 마지막 1명
        show(table);
    }

    // 11. 연도 리스트 head / tail 연도 비우기 후 min~max 확인
    section("11. min/max after emptying edge years");
    {
        StudentTable table;
        table.addStudent(s(20200001, "A"));
        table.addStudent(s(20210001, "B"));
        table.addStudent(s(20220001, "C"));
        table.removeStudent(20200001);
        show(table);
        table.removeStudent(20220001);
        show(table);
        table.removeStudent(20210001);
        show(table);
    }

    // 12. 삭제한 연도 / 그룹 재추가
    section("12. re-add after removal");
    {
        StudentTable table;
        table.addStudent(s(20220868, "Kim", "G1"));
        table.removeStudent(20220868);
        table.addStudent(s(20220868, "Kim2", "G1"));
        show(table);
    }

    // 13. 팀 구성: 팀원 head / 중간 / tail 삭제
    section("13. team member removal");
    {
        StudentTable table;
        table.addStudent(s(20200001, "A", "G1"));
        table.addStudent(s(20210001, "B", "G1"));
        table.addStudent(s(20220001, "C", "G1"));
        table.addStudent(s(20230001, "D", "G1"));
        show(table);
        table.removeStudent(20230001); // 마지막 추가된 팀원
        show(table);
        table.removeStudent(20210001); // 중간 팀원
        show(table);
        table.removeStudent(20200001); // 가장 먼저 추가된 팀원
        show(table);
        table.removeStudent(20220001); // 마지막 1명
        show(table);
    }

    // 14. 팀 2명 -> 1명
    section("14. team of two shrinks to one");
    {
        StudentTable table;
        table.addStudent(s(20210001, "A", "G1"));
        table.addStudent(s(20220001, "B", "G1"));
        table.removeStudent(20210001);
        show(table);
    }

    // 15. 그룹 있는 학생 / 없는 학생 혼합 (같은 연도)
    section("15. mixed grouped and ungrouped");
    {
        StudentTable table;
        table.addStudent(s(20220001, "A"));
        table.addStudent(s(20220002, "B", "G1"));
        table.addStudent(s(20220003, "C"));
        table.addStudent(s(20220004, "D", "G1"));
        show(table);
    }

    // 16. 그룹 없는 학생만 존재 / 그룹 수 계산
    section("16. only ungrouped students");
    {
        StudentTable table;
        table.addStudent(s(20210001, "A"));
        table.addStudent(s(20220001, "B"));
        show(table);
    }

    // 17. 그룹 ID 삽입 순서 (사전순, 중간 삽입)
    section("17. group id ordering");
    {
        StudentTable table;
        table.addStudent(s(20220001, "A", "B"));
        table.addStudent(s(20220002, "B", "A"));
        table.addStudent(s(20220003, "C", "C"));
        table.addStudent(s(20220004, "D", "AA"));
        table.addStudent(s(20220005, "E"));
        show(table);
    }

    // 18. 접두사 관계의 그룹 ID ("G" vs "G1" vs "G10")
    section("18. prefix group ids");
    {
        StudentTable table;
        table.addStudent(s(20220001, "A", "G10"));
        table.addStudent(s(20220002, "B", "G1"));
        table.addStudent(s(20220003, "C", "G"));
        table.addStudent(s(20220004, "D", "G1"));
        table.addStudent(s(20220005, "E", "G10"));
        show(table);
    }

    // 19. 이름에 공백 / 빈 이름
    section("19. names with spaces and empty name");
    {
        StudentTable table;
        table.addStudent(s(20220001, "Kim Min Su", "G1"));
        table.addStudent(s(20220002, "", "G1"));
        table.addStudent(s(20220003, "  leading", ""));
        show(table);
    }

    // 20. 학번 경계값 (연도 0, 연도 9999 이하 / 1 / 매우 큰 연도)
    section("20. number boundaries");
    {
        StudentTable table;
        table.addStudent(s(0, "Zero"));
        table.addStudent(s(9999, "Year0Max"));
        table.addStudent(s(10000, "Year1"));
        table.addStudent(s(99999999, "Year9999"));
        table.addStudent(s(2147483647, "IntMax", "G1"));
        show(table);
        table.removeStudent(0);
        table.removeStudent(2147483647);
        show(table);
    }

    // 21. 중복 학번 추가
    section("21. duplicate number");
    {
        StudentTable table;
        table.addStudent(s(20220868, "Kim", "G1"));
        table.addStudent(s(20220868, "Kim", "G1"));
        show(table);
        table.removeStudent(20220868);
        show(table);
    }

    // 22. 해시맵 리해시를 유발하는 대량 연도 / 대량 그룹
    section("22. many years and groups");
    {
        StudentTable table;
        for (int i = 0; i < 100; ++i) {
            const MyString group = MyString("G") + MyString::toString(i);
            table.addStudent(Student(1990 * 0 + (2000 + i) * 10000 + 1, "N", group));
        }
        show(table);
        for (int i = 0; i < 100; i += 2) {
            table.removeStudent((2000 + i) * 10000 + 1);
        }
        show(table);
    }

    // 23. 대량 학생이 같은 팀 / 같은 연도
    section("23. large single team and single year");
    {
        StudentTable table;
        for (int i = 1; i <= 10; ++i) {
            table.addStudent(Student(20220000 + i, "N", "G1"));
        }
        show(table);
        for (int i = 1; i <= 10; ++i) {
            table.removeStudent(20220000 + i);
        }
        show(table);
    }

    // 24. 전부 삭제 후 재사용
    section("24. remove everything then reuse");
    {
        StudentTable table;
        table.addStudent(s(20200001, "A", "G1"));
        table.addStudent(s(20210001, "B", "G2"));
        table.addStudent(s(20220001, "C"));
        table.removeStudent(20200001);
        table.removeStudent(20210001);
        table.removeStudent(20220001);
        show(table);
        table.addStudent(s(20230001, "D", "G3"));
        show(table);
    }

    // 25. 파일 내보내기: 빈 테이블 / 일반 / 그룹 없음 표기
    section("25. exportFile");
    {
        const char *path = "edge_export.txt";
        MyString filename = path;

        StudentTable empty;
        empty.exportFile(filename);
        std::cout << "[empty]" << std::endl;
        {
            std::ifstream in(path);
            std::cout << in.rdbuf() << std::endl;
        }

        StudentTable table;
        table.addStudent(s(20210001, "Lee Ji", "G1"));
        table.addStudent(s(20220868, "Kim", "G1"));
        table.addStudent(s(20220001, "Park"));
        table.exportFile(filename);
        std::cout << "[filled]" << std::endl;
        {
            std::ifstream in(path);
            std::cout << in.rdbuf() << std::endl;
        }
        std::remove(path);
    }

    return 0;
}