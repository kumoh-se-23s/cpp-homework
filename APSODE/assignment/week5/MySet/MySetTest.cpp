#include <gtest/gtest.h>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "MySet.h"

namespace {
    MySet make_set(std::initializer_list<int> elements) {
        MySet set;
        for (int element : elements) {
            set.add(element);
        }
        return set;
    }

    std::string to_string(const MySet &set) {
        std::ostringstream stream;
        stream << set;
        return stream.str();
    }

    // 집합은 순서가 중요하지 않으므로 정렬한 원소 목록으로 비교한다.
    std::vector<int> sorted_elements(const MySet &set) {
        std::vector<int> elements;
        for (int index = 0; index < set.size(); ++index) {
            elements.push_back(set.get(index));
        }
        std::sort(elements.begin(), elements.end());
        return elements;
    }

    // "{a, b, c}" 형식을 파싱해 정렬된 원소 목록으로 돌려준다.
    std::vector<int> parse_output(const std::string &text) {
        EXPECT_FALSE(text.empty());
        EXPECT_EQ(text.front(), '{');
        EXPECT_EQ(text.back(), '}');

        std::vector<int> elements;
        std::string body = text.substr(1, text.size() - 2);
        std::replace(body.begin(), body.end(), ',', ' ');
        std::istringstream stream(body);
        int element;
        while (stream >> element) {
            elements.push_back(element);
        }
        std::sort(elements.begin(), elements.end());
        return elements;
    }
}

// =====================================================================
// 생성자 / 기본 동작
// =====================================================================

TEST(MySetConstructor, DefaultCreatesEmptySet) {
    MySet set;

    EXPECT_EQ(set.size(), 0);
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(to_string(set), "{}");
}

TEST(MySetAdd, IgnoresDuplicateElements) {
    MySet set;
    set.add(3);
    set.add(3);
    set.add(1);
    set.add(3);

    EXPECT_EQ(set.size(), 2);
    EXPECT_EQ(sorted_elements(set), (std::vector<int>{1, 3}));
}

TEST(MySetAdd, GrowsBeyondInitialCapacity) {
    MySet set;
    for (int value = 0; value < 1000; ++value) {
        set.add(value);
    }

    EXPECT_EQ(set.size(), 1000);
    for (int value = 0; value < 1000; ++value) {
        EXPECT_TRUE(set.contains(value)) << value;
    }
}

TEST(MySetAdd, AddAllSkipsDuplicates) {
    const int values[] = {5, 4, 3, 1, 7, 9, 3, 5};
    MySet set;
    set.add_all(values, 8);

    EXPECT_EQ(sorted_elements(set), (std::vector<int>{1, 3, 4, 5, 7, 9}));
}

TEST(MySetAdd, AddAllRejectsNegativeCount) {
    const int values[] = {1};
    MySet set;

    EXPECT_THROW(set.add_all(values, -1), std::invalid_argument);
}

TEST(MySetSet, RejectsDuplicateValue) {
    MySet set = make_set({1, 2, 3});

    EXPECT_THROW(set.set(0, 2), std::invalid_argument);
    EXPECT_EQ(sorted_elements(set), (std::vector<int>{1, 2, 3}));
}

TEST(MySetSet, AllowsSameValueAtSameIndex) {
    MySet set = make_set({1, 2, 3});

    EXPECT_NO_THROW(set.set(1, 2));
    EXPECT_NO_THROW(set.set(1, 10));
    EXPECT_EQ(sorted_elements(set), (std::vector<int>{1, 3, 10}));
}

TEST(MySetRemove, RemovesAndReturnsElement) {
    MySet set = make_set({1, 2, 3});
    const int removed = set.remove(0);

    EXPECT_EQ(set.size(), 2);
    EXPECT_FALSE(set.contains(removed));
}

TEST(MySetAccess, OutOfRangeThrows) {
    MySet set = make_set({1});

    EXPECT_THROW(set.get(1), std::out_of_range);
    EXPECT_THROW(set.get(-1), std::out_of_range);
    EXPECT_THROW(MySet().remove(0), std::out_of_range);
}

// =====================================================================
// 복사 생성자 / 대입 연산자 (깊은 복사)
// =====================================================================

TEST(MySetCopy, CopyConstructorIsDeep) {
    MySet original = make_set({1, 2, 3});
    MySet copy(original);

    copy.add(4);
    copy.set(0, 100);

    EXPECT_EQ(sorted_elements(original), (std::vector<int>{1, 2, 3}));
    EXPECT_EQ(copy.size(), 4);
}

TEST(MySetAssign, AssignmentIsDeep) {
    MySet original = make_set({1, 2, 3});
    MySet target = make_set({7, 8, 9, 10, 11, 12});

    target = original;
    EXPECT_EQ(sorted_elements(target), (std::vector<int>{1, 2, 3}));

    target.add(4);
    target.remove(0);
    EXPECT_EQ(sorted_elements(original), (std::vector<int>{1, 2, 3}));
}

TEST(MySetAssign, SelfAssignmentKeepsElements) {
    MySet set = make_set({1, 2, 3});
    MySet &alias = set;

    set = alias;

    EXPECT_EQ(sorted_elements(set), (std::vector<int>{1, 2, 3}));
}

TEST(MySetAssign, ChainedAssignment) {
    MySet a, b;
    MySet c = make_set({4, 5});

    a = b = c;

    EXPECT_EQ(sorted_elements(a), (std::vector<int>{4, 5}));
    EXPECT_EQ(sorted_elements(b), (std::vector<int>{4, 5}));
}

TEST(MySetAssign, RepeatedResultAssignment) {
    MySet s1 = make_set({2, 3, 1, 4});
    MySet s2 = make_set({5, 4, 3, 1, 7, 9});
    MySet result;

    for (int i = 0; i < 100; ++i) {
        result = s1 + s2;
        result = s1 & s2;
        result = s1 - s2;
    }

    EXPECT_EQ(sorted_elements(result), (std::vector<int>{2}));
}

// =====================================================================
// 집합 연산
// =====================================================================

TEST(MySetOperator, UnionOfExample) {
    MySet s1 = make_set({2, 3, 1, 4});
    MySet s2 = make_set({5, 4, 3, 1, 7, 9});

    EXPECT_EQ(sorted_elements(s1 + s2), (std::vector<int>{1, 2, 3, 4, 5, 7, 9}));
}

TEST(MySetOperator, IntersectionOfExample) {
    MySet s1 = make_set({2, 3, 1, 4});
    MySet s2 = make_set({5, 4, 3, 1, 7, 9});

    EXPECT_EQ(sorted_elements(s1 & s2), (std::vector<int>{1, 3, 4}));
}

TEST(MySetOperator, DifferenceOfExample) {
    MySet s1 = make_set({2, 3, 1, 4});
    MySet s2 = make_set({5, 4, 3, 1, 7, 9});

    EXPECT_EQ(sorted_elements(s1 - s2), (std::vector<int>{2}));
    EXPECT_EQ(sorted_elements(s2 - s1), (std::vector<int>{5, 7, 9}));
}

TEST(MySetOperator, OperandsAreNotModified) {
    MySet s1 = make_set({2, 3, 1, 4});
    MySet s2 = make_set({5, 4, 3, 1, 7, 9});

    (void) (s1 + s2);
    (void) (s1 & s2);
    (void) (s1 - s2);

    EXPECT_EQ(sorted_elements(s1), (std::vector<int>{1, 2, 3, 4}));
    EXPECT_EQ(sorted_elements(s2), (std::vector<int>{1, 3, 4, 5, 7, 9}));
}

TEST(MySetOperator, WithEmptySet) {
    MySet empty;
    MySet set = make_set({1, 2});

    EXPECT_EQ(sorted_elements(set + empty), (std::vector<int>{1, 2}));
    EXPECT_EQ(sorted_elements(empty + set), (std::vector<int>{1, 2}));
    EXPECT_TRUE((set & empty).empty());
    EXPECT_TRUE((empty & set).empty());
    EXPECT_EQ(sorted_elements(set - empty), (std::vector<int>{1, 2}));
    EXPECT_TRUE((empty - set).empty());
    EXPECT_TRUE((empty + empty).empty());
}

TEST(MySetOperator, DisjointSets) {
    MySet s1 = make_set({1, 2, 3});
    MySet s2 = make_set({4, 5, 6});

    EXPECT_EQ((s1 + s2).size(), 6);
    EXPECT_TRUE((s1 & s2).empty());
    EXPECT_EQ(sorted_elements(s1 - s2), (std::vector<int>{1, 2, 3}));
}

TEST(MySetOperator, IdenticalSets) {
    MySet s1 = make_set({1, 2, 3});
    MySet s2 = make_set({3, 2, 1});

    EXPECT_EQ(sorted_elements(s1 + s2), (std::vector<int>{1, 2, 3}));
    EXPECT_EQ(sorted_elements(s1 & s2), (std::vector<int>{1, 2, 3}));
    EXPECT_TRUE((s1 - s2).empty());
}

TEST(MySetOperator, LargeSets) {
    MySet evens, multiples_of_three;
    for (int value = 0; value < 600; value += 2) {
        evens.add(value);
    }
    for (int value = 0; value < 600; value += 3) {
        multiples_of_three.add(value);
    }

    EXPECT_EQ((evens + multiples_of_three).size(), 300 + 200 - 100);
    EXPECT_EQ((evens & multiples_of_three).size(), 100);
    EXPECT_EQ((evens - multiples_of_three).size(), 200);
}

// =====================================================================
// 입출력
// =====================================================================

TEST(MySetOutput, FormatsElements) {
    EXPECT_EQ(to_string(make_set({7})), "{7}");
    EXPECT_EQ(parse_output(to_string(make_set({2, 3, 1}))), (std::vector<int>{1, 2, 3}));

    const std::string text = to_string(make_set({2, 3, 1}));
    EXPECT_NE(text.find(", "), std::string::npos);
}

TEST(MySetInput, StopsAtMinusOne) {
    std::istringstream input("2 3 1 2 4 -1");
    MySet set;

    input >> set;

    EXPECT_EQ(sorted_elements(set), (std::vector<int>{1, 2, 3, 4}));
}

TEST(MySetInput, ReadsTwoSetsInChain) {
    std::istringstream input("2 3 1 2 4 -1\n5 4 3 1 7 9 -1\n");
    MySet s1, s2;

    input >> s1 >> s2;

    EXPECT_EQ(sorted_elements(s1), (std::vector<int>{1, 2, 3, 4}));
    EXPECT_EQ(sorted_elements(s2), (std::vector<int>{1, 3, 4, 5, 7, 9}));
}

// 과제 조건: "음수가 들어올 때까지 입력" — -1 이외의 음수에서도 멈춰야 한다.
TEST(MySetInput, StopsAtAnyNegative) {
    std::istringstream input("1 2 -5 3 4 -100");
    MySet s1, s2;

    input >> s1 >> s2;

    EXPECT_EQ(sorted_elements(s1), (std::vector<int>{1, 2}));
    EXPECT_EQ(sorted_elements(s2), (std::vector<int>{3, 4}));
}

TEST(MySetInput, EmptyInputBeforeTerminator) {
    std::istringstream input("-1");
    MySet set;

    input >> set;

    EXPECT_TRUE(set.empty());
}

// =====================================================================
// 문제의 예시 실행 시나리오
// =====================================================================

TEST(MySetScenario, MatchesAssignmentExample) {
    std::istringstream input("2 3 1 2 4 -1\n5 4 3 1 7 9 -1\n");
    MySet s1, s2, resultSet;

    input >> s1 >> s2;

    resultSet = s1 + s2;
    EXPECT_EQ(parse_output(to_string(resultSet)), (std::vector<int>{1, 2, 3, 4, 5, 7, 9}));

    resultSet = s1 & s2;
    EXPECT_EQ(parse_output(to_string(resultSet)), (std::vector<int>{1, 3, 4}));

    resultSet = s1 - s2;
    EXPECT_EQ(parse_output(to_string(resultSet)), (std::vector<int>{2}));

    resultSet = s2 - s1;
    EXPECT_EQ(parse_output(to_string(resultSet)), (std::vector<int>{5, 7, 9}));
}
