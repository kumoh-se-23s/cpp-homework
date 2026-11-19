#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "MyString.h"
#include "StudentController.h"

namespace {
    std::string to_std_string(const MyString &str) {
        std::ostringstream os;
        os << str;
        return os.str();
    }

    std::string stream_output(const StudentController &controller) {
        std::ostringstream os;
        os << controller;
        return os.str();
    }
}

// ---------------------------------------------------------------------
// 기본 삽입 및 출력
// ---------------------------------------------------------------------

TEST(StudentControllerBasic, EmptyControllerOutputsEmptyString) {
    const StudentController controller;
    EXPECT_EQ("", to_std_string(controller.toString()));
    EXPECT_EQ("", stream_output(controller));
}

TEST(StudentControllerBasic, InsertSingleStudent) {
    StudentController controller;
    controller.insertStudent(20220001, MyString("G1"), MyString("Alice"));

    const std::string output = to_std_string(controller.toString());
    EXPECT_NE(std::string::npos, output.find("20220001"));
    EXPECT_NE(std::string::npos, output.find("Alice"));
    EXPECT_NE(std::string::npos, output.find("G1"));
    EXPECT_EQ(output, stream_output(controller));
}

TEST(StudentControllerBasic, StudentsOrderedByYearAndStudentId) {
    StudentController controller;
    controller.insertStudent(20240002, MyString("G1"), MyString("Nora"));
    controller.insertStudent(19980005, MyString("G2"), MyString("Jenny"));
    controller.insertStudent(20220010, MyString("G3"), MyString("Harley"));
    controller.insertStudent(20220001, MyString("G4"), MyString("Liam"));

    const std::string output = to_std_string(controller.toString());
    const std::size_t pos1998 = output.find("19980005");
    const std::size_t pos2022_1 = output.find("20220001");
    const std::size_t pos2022_10 = output.find("20220010");
    const std::size_t pos2024 = output.find("20240002");

    ASSERT_NE(std::string::npos, pos1998);
    ASSERT_NE(std::string::npos, pos2022_1);
    ASSERT_NE(std::string::npos, pos2022_10);
    ASSERT_NE(std::string::npos, pos2024);

    EXPECT_LT(pos1998, pos2022_1);
    EXPECT_LT(pos2022_1, pos2022_10);
    EXPECT_LT(pos2022_10, pos2024);
}

// ---------------------------------------------------------------------
// 같은 그룹원 출력
// ---------------------------------------------------------------------

TEST(StudentControllerGroup, SameGroupMembersAppearTogether) {
    StudentController controller;
    controller.insertStudent(20210001, MyString("G1"), MyString("Alice"));
    controller.insertStudent(20230002, MyString("G1"), MyString("Bob"));
    controller.insertStudent(20220003, MyString("G2"), MyString("Charlie"));

    const std::string output = to_std_string(controller.toString());
    EXPECT_NE(std::string::npos, output.find("Alice"));
    EXPECT_NE(std::string::npos, output.find("Bob"));
    EXPECT_NE(std::string::npos, output.find("Charlie"));

    // G1에 2명이 있으므로 각 학생 출력 시 그룹원이 함께 출력되어 Alice가 2번 이상 등장한다
    const std::size_t firstAlice = output.find("Alice");
    const std::size_t secondAlice = output.find("Alice", firstAlice + 1);
    EXPECT_NE(std::string::npos, secondAlice);
}

// ---------------------------------------------------------------------
// 삭제
// ---------------------------------------------------------------------

TEST(StudentControllerDelete, DeleteOnEmptyOrMissingIdDoesNotCrash) {
    StudentController controller;
    controller.deleteStudent(20220001);
    EXPECT_EQ("", to_std_string(controller.toString()));

    controller.insertStudent(20220001, MyString("G1"), MyString("Alice"));
    controller.deleteStudent(99999999);  // 없는 학번
    controller.deleteStudent(20229999);  // 같은 년도, 없는 학번

    const std::string output = to_std_string(controller.toString());
    EXPECT_NE(std::string::npos, output.find("Alice"));
}

TEST(StudentControllerDelete, DeleteOneMemberFromMultiMemberGroup) {
    StudentController controller;
    controller.insertStudent(20210001, MyString("G1"), MyString("Alice"));
    controller.insertStudent(20220002, MyString("G1"), MyString("Bob"));
    controller.insertStudent(20230003, MyString("G1"), MyString("Charlie"));

    controller.deleteStudent(20220002);

    const std::string output = to_std_string(controller.toString());
    EXPECT_EQ(std::string::npos, output.find("20220002"));
    EXPECT_EQ(std::string::npos, output.find("Bob"));
    EXPECT_NE(std::string::npos, output.find("Alice"));
    EXPECT_NE(std::string::npos, output.find("Charlie"));
}

TEST(StudentControllerDelete, DeleteAllStudentsAndReinsert) {
    StudentController controller;
    controller.insertStudent(20210001, MyString("G1"), MyString("Alice"));
    controller.insertStudent(20220002, MyString("G2"), MyString("Bob"));

    controller.deleteStudent(20210001);
    controller.deleteStudent(20220002);
    EXPECT_EQ("", to_std_string(controller.toString()));

    controller.insertStudent(20250001, MyString("G1"), MyString("Dave"));
    const std::string output = to_std_string(controller.toString());
    EXPECT_NE(std::string::npos, output.find("20250001"));
    EXPECT_NE(std::string::npos, output.find("Dave"));
    EXPECT_EQ(std::string::npos, output.find("Alice"));
}

// ---------------------------------------------------------------------
// 용량 확장 (LookUpTable resize 시 NestedDoubleNode 포인터 안전성)
// ---------------------------------------------------------------------

TEST(StudentControllerGrowth, InsertBeyondDefaultCapacityYearsAndGroups) {
    StudentController controller;

    // DEFAULT_CAPACITY(8) 를 넘도록 15개 년도 / 15개 그룹 삽입
    for (int i = 0; i < 15; ++i) {
        const int studentId = (2010 + i) * 10000 + i + 1;
        const MyString group(("G" + std::to_string(i)).c_str());
        const MyString name(("Student" + std::to_string(i)).c_str());
        controller.insertStudent(studentId, group, name);
    }

    const std::string output = to_std_string(controller.toString());
    for (int i = 0; i < 15; ++i) {
        const std::string expectedName = "Student" + std::to_string(i);
        EXPECT_NE(std::string::npos, output.find(expectedName)) << "missing: " << expectedName;
    }

    // 절반 삭제 후 나머지 정상 출력 확인
    for (int i = 0; i < 15; i += 2) {
        controller.deleteStudent((2010 + i) * 10000 + i + 1);
    }

    const std::string afterDelete = to_std_string(controller.toString());
    for (int i = 0; i < 15; ++i) {
        const std::string idStr = std::to_string((2010 + i) * 10000 + i + 1);
        if (i % 2 == 0) {
            EXPECT_EQ(std::string::npos, afterDelete.find(idStr)) << "should be deleted: " << idStr;
        } else {
            EXPECT_NE(std::string::npos, afterDelete.find(idStr)) << "should remain: " << idStr;
        }
    }
}