#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "MyString.h"
#include "Student.h"

// =====================================================================
// Student 테스트
//
//  - 비교 연산자(==, !=, <, >, <=, >=)는 const Student& 를 받아 학번(studentId) 기준으로 비교한다.
//  - toString() 은 학번, 그룹, 이름이 이 순서로 들어 있는지만 확인한다.
//    (구분자 포맷 변경에 유연하도록 전체 문자열 대신 순서와 포함 여부로 검증)
// =====================================================================

namespace {
    std::string to_std_string(const MyString &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    void expect_student(const Student &student, const int id, const char *group, const char *name) {
        EXPECT_EQ(id, student.getStudentId());
        EXPECT_EQ(group, to_std_string(student.getStudentGroup()));
        EXPECT_EQ(name, to_std_string(student.getStudentName()));
    }
}

// ---------------------------------------------------------------------
// 생성자
// ---------------------------------------------------------------------

TEST(StudentConstruct, WithGroupAndSpacedName) {
    const Student student(19980123, "G1", "Jenny Anh");
    expect_student(student, 19980123, "G1", "Jenny Anh");
}

TEST(StudentConstruct, WithoutGroupUsesDash) {
    const Student student(19981475, "Aria Lee");
    expect_student(student, 19981475, "-", "Aria Lee");
}

TEST(StudentConstruct, EmptyStrings) {
    const Student student(0, "", "");
    expect_student(student, 0, "", "");
}

TEST(StudentConstruct, KoreanNameAndGroup) {
    const Student student(1, "알파팀", "홍길동");
    expect_student(student, 1, "알파팀", "홍길동");
}

TEST(StudentConstruct, LongStringsBeyondInitialCapacity) {
    const std::string group(100, 'g');
    const std::string name(100, 'n');
    const Student student(7, MyString(group.c_str()), MyString(name.c_str()));
    expect_student(student, 7, group.c_str(), name.c_str());
}

// ---------------------------------------------------------------------
// 복사 / 이동
// ---------------------------------------------------------------------

TEST(StudentCopy, CopyConstructor) {
    const Student original(10, "G1", "Harley Kim");
    const Student copy(original);
    expect_student(copy, 10, "G1", "Harley Kim");
    expect_student(original, 10, "G1", "Harley Kim");
}

TEST(StudentCopy, CopyIsDeepNotShared) {
    const Student original(10, "G1", "Harley Kim");
    const Student copy(original);
    EXPECT_NE(&original.getStudentName(), &copy.getStudentName());
    EXPECT_NE(&original.getStudentGroup(), &copy.getStudentGroup());
}

TEST(StudentCopy, MoveConstructor) {
    Student original(10, "G1", "Harley Kim");
    const Student moved(std::move(original));
    expect_student(moved, 10, "G1", "Harley Kim");
}

TEST(StudentCopy, CopyAssignment) {
    const Student source(10, "G1", "Harley Kim");
    Student target(99, "G9", "Owen Park");
    target = source;
    expect_student(target, 10, "G1", "Harley Kim");
    expect_student(source, 10, "G1", "Harley Kim");
}

TEST(StudentCopy, MoveAssignment) {
    Student source(10, "G1", "Harley Kim");
    Student target(99, "G9", "Owen Park");
    target = std::move(source);
    expect_student(target, 10, "G1", "Harley Kim");
}

TEST(StudentCopy, SelfCopyAndMoveAssignment) {
    Student student(10, "G1", "Harley Kim");
    Student &alias = student;
    student = alias;
    expect_student(student, 10, "G1", "Harley Kim");

    student = std::move(alias);
    expect_student(student, 10, "G1", "Harley Kim");
}

TEST(StudentCopy, AssignmentChain) {
    const Student source(1, "A", "a");
    Student first(2, "B", "b");
    Student second(3, "C", "c");
    first = second = source;
    expect_student(first, 1, "A", "a");
    expect_student(second, 1, "A", "a");
}

TEST(StudentCopy, VectorReallocationKeepsAllValues) {
    std::vector<Student> students;
    for (int index = 0; index < 200; ++index) {
        students.emplace_back(index, MyString("G1"), MyString(std::to_string(index).c_str()));
    }
    for (int index = 0; index < 200; ++index) {
        EXPECT_EQ(index, students[index].getStudentId());
        EXPECT_EQ("G1", to_std_string(students[index].getStudentGroup()));
        EXPECT_EQ(std::to_string(index), to_std_string(students[index].getStudentName()));
    }
}

// ---------------------------------------------------------------------
// 비교 연산자 (Student 객체 간 학번 기준)
// ---------------------------------------------------------------------

TEST(StudentCompare, EqualAndNotEqual) {
    const Student a(100, "G1", "Kim");
    const Student same(100, "G1", "Kim");
    const Student sameIdDifferentInfo(100, "G2", "Lee");
    const Student smaller(99, "G1", "Kim");
    const Student bigger(101, "G1", "Kim");

    EXPECT_TRUE(a == a);
    EXPECT_TRUE(a == same);
    EXPECT_TRUE(a == sameIdDifferentInfo);
    EXPECT_FALSE(a == smaller);
    EXPECT_FALSE(a == bigger);

    EXPECT_FALSE(a != a);
    EXPECT_FALSE(a != same);
    EXPECT_FALSE(a != sameIdDifferentInfo);
    EXPECT_TRUE(a != smaller);
    EXPECT_TRUE(a != bigger);
}

TEST(StudentCompare, EqualAndNotEqualAreOpposite) {
    const Student base(100, "G1", "Kim");
    for (int id = 95; id <= 105; ++id) {
        const Student other(id, "G2", "Other");
        EXPECT_NE(base == other, base != other) << "id: " << id;
    }
}

TEST(StudentCompare, LessThanAndGreaterThan) {
    const Student small(19980123, "G1", "Jenny Anh");
    const Student same(19980123, "G3", "Different Name");
    const Student big(20220987, "G1", "Harley Kim");

    EXPECT_TRUE(small < big);
    EXPECT_FALSE(big < small);
    EXPECT_FALSE(small < same);
    EXPECT_FALSE(small < small);

    EXPECT_TRUE(big > small);
    EXPECT_FALSE(small > big);
    EXPECT_FALSE(small > same);
    EXPECT_FALSE(small > small);
}

TEST(StudentCompare, LessOrEqualAndGreaterOrEqual) {
    const Student small(100, "G1", "Kim");
    const Student same(100, "G2", "Lee");
    const Student big(200, "G1", "Park");

    EXPECT_TRUE(small <= big);
    EXPECT_TRUE(small <= same);
    EXPECT_TRUE(small <= small);
    EXPECT_FALSE(big <= small);

    EXPECT_TRUE(big >= small);
    EXPECT_TRUE(small >= same);
    EXPECT_TRUE(small >= small);
    EXPECT_FALSE(small >= big);
}

TEST(StudentCompare, IdZeroAndNegative) {
    const Student neg(-1, "-", "neg");
    const Student zero(0, "-", "zero");
    const Student pos(1, "-", "pos");

    EXPECT_TRUE(neg < zero);
    EXPECT_TRUE(zero < pos);
    EXPECT_TRUE(zero > neg);
    EXPECT_TRUE(zero <= zero);
    EXPECT_TRUE(zero >= zero);
}

TEST(StudentCompare, IgnoresGroupAndName) {
    const Student a(5, "G1", "Alpha");
    const Student b(5, "G9", "Zebra");

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a > b);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a >= b);
}

// ---------------------------------------------------------------------
// getter
// ---------------------------------------------------------------------

TEST(StudentGetter, ReturnReferencesToTheSameMembers) {
    const Student student(1, "G1", "Kim");
    EXPECT_EQ(&student.getStudentGroup(), &student.getStudentGroup());
    EXPECT_EQ(&student.getStudentName(), &student.getStudentName());
}

// ---------------------------------------------------------------------
// toString
// ---------------------------------------------------------------------

TEST(StudentToString, ContainsIdGroupNameInOrder) {
    const Student student(19980123, "G1", "Jenny Anh");
    const std::string text = to_std_string(student.toString());

    const std::size_t idPos = text.find("19980123");
    const std::size_t groupPos = text.find("G1");
    const std::size_t namePos = text.find("Jenny Anh");

    ASSERT_NE(std::string::npos, idPos) << text;
    ASSERT_NE(std::string::npos, groupPos) << text;
    ASSERT_NE(std::string::npos, namePos) << text;
    EXPECT_LT(idPos, groupPos) << text;
    EXPECT_LT(groupPos, namePos) << text;
}

TEST(StudentToString, IdIsPrintedInDecimalForAnyDigitCount) {
    const int ids[] = {0, 7, 10, 123, 1000, 19980123, 20250907};
    for (const int id : ids) {
        const Student student(id, "G", "N");
        const std::string text = to_std_string(student.toString());
        EXPECT_EQ(0u, text.find(std::to_string(id))) << "id: " << id << ", text: " << text;
    }
}

TEST(StudentToString, DefaultGroupDashIsIncluded) {
    const Student student(19981475, "Aria Lee");
    const std::string text = to_std_string(student.toString());
    EXPECT_NE(std::string::npos, text.find('-')) << text;
    EXPECT_NE(std::string::npos, text.find("Aria Lee")) << text;
}

TEST(StudentToString, CallingTwiceGivesSameResultAndDoesNotModifyStudent) {
    const Student student(123, "G1", "Kim");
    const std::string first = to_std_string(student.toString());
    const std::string second = to_std_string(student.toString());
    EXPECT_EQ(first, second);
    expect_student(student, 123, "G1", "Kim");
}

TEST(StudentToString, KoreanText) {
    const Student student(1, "알파팀", "홍길동");
    const std::string text = to_std_string(student.toString());
    EXPECT_NE(std::string::npos, text.find("알파팀")) << text;
    EXPECT_NE(std::string::npos, text.find("홍길동")) << text;
}