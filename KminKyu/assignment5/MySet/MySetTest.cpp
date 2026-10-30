#include <gtest/gtest.h>

#include <algorithm>
#include <climits>
#include <iterator>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// private 멤버(array, size, capacity, append, binarySearch)를 직접 검사하기 위한 트릭.
// 표준 헤더를 모두 include 한 "뒤", MySet.h 를 include 하기 "전"에 정의해야 한다.
//  - array/capacity/size 는 "private:" 키워드 없이 class 기본 접근으로 private 이므로
//    #define private public 만으로는 열리지 않는다. class -> struct 로 바꿔 기본 접근을 public 으로 만든다.
//  - append/binarySearch 는 "private:" 아래에 있으므로 #define private public 으로 연다.
#define class struct
#define private public
#include "MySet.h"
#undef private
#undef class

// 빌드 예시:
//   g++ -std=c++17 -g -fsanitize=address,undefined MySetTest.cpp MySet.cpp -lgtest -lgtest_main -pthread -o MySetTest
//
// 현재 구현 기준 사양
//  - MySet(c = 4)                : capacity = max(c, 0), size = 0
//  - insert(v)                   : 오름차순 유지, 중복이면 무시
//  - binarySearch(left, right, v): 반열린 구간 [left, right) 탐색.
//                                  v 가 있으면 -1, 없으면 삽입 위치(left..right) 반환
//  - resize()                    : capacity 가 0 이면 4, 아니면 2배. 원소/size 유지
//  - get(i)                      : 범위 밖이면 INT_MIN
//  - operator>>                  : 기존 내용을 비우고 -1 이 나올 때까지 insert
//  - operator<<                  : "{1, 2, 3}" 형식, 빈 집합은 "{}"

namespace {
    using Vec = std::vector<int>;

    // 공개 API 만 사용해 집합 생성
    MySet make_set(const Vec &values) {
        MySet s;
        for (int v : values) s.insert(v);
        return s;
    }

    // 공개 API(get) 로 원소 꺼내기 - 정렬하지 않으므로 순서까지 검사됨
    Vec elements(const MySet &s) {
        Vec v;
        for (int i = 0; i < s.getLength(); ++i) v.push_back(s.get(i));
        return v;
    }

    Vec sorted_unique(Vec v) {
        std::sort(v.begin(), v.end());
        v.erase(std::unique(v.begin(), v.end()), v.end());
        return v;
    }

    Vec random_vec(std::mt19937 &rng, int n, int maxValue) {
        Vec v(n);
        for (int &x : v) x = static_cast<int>(rng() % maxValue);  // 0..maxValue-1 (-1 없음)
        return v;
    }

    MySet read_set(const std::string &text) {
        std::istringstream in(text);
        MySet s;
        in >> s;
        return s;
    }

    std::string to_string(const MySet &s) {
        std::ostringstream out;
        out << s;
        return out.str();
    }

    // 불변식: 0 <= size <= capacity, 원소는 엄격한 오름차순
    void expect_invariant(const MySet &s) {
        EXPECT_GE(s.size, 0);
        EXPECT_LE(s.size, s.capacity);
        for (int i = 1; i < s.size; ++i) {
            EXPECT_LT(s.array[i - 1], s.array[i]) << "index " << i;
        }
    }
}

// =====================================================================
// 생성자
// =====================================================================

TEST(MySetConstructorTest, DefaultIsEmptyWithCapacity4) {
    MySet s;
    EXPECT_EQ(0, s.getLength());
    EXPECT_EQ(4, s.getCapacity());
    EXPECT_EQ("{}", to_string(s));
}

TEST(MySetConstructorTest, CustomCapacity) {
    MySet s(10);
    EXPECT_EQ(0, s.getLength());
    EXPECT_EQ(10, s.getCapacity());
}

TEST(MySetConstructorTest, NegativeCapacityBecomesZero) {
    MySet s(-5);
    EXPECT_EQ(0, s.getCapacity());
    EXPECT_EQ(0, s.getLength());
}

TEST(MySetConstructorTest, ZeroOrNegativeCapacityStillUsable) {
    for (int c : {0, -1, -100}) {
        MySet s(c);
        s.insert(3);
        s.insert(1);
        s.insert(2);
        EXPECT_EQ((Vec{1, 2, 3}), elements(s)) << "capacity " << c;
        expect_invariant(s);
    }
}

// =====================================================================
// insert
// =====================================================================

TEST(MySetInsertTest, KeepsAscendingOrder) {
    MySet s = make_set({5, 1, 4, 2, 3});
    EXPECT_EQ((Vec{1, 2, 3, 4, 5}), elements(s));
    expect_invariant(s);
}

TEST(MySetInsertTest, IgnoresDuplicates) {
    MySet s = make_set({3, 3, 1, 3, 1, 2, 2});
    EXPECT_EQ((Vec{1, 2, 3}), elements(s));
    EXPECT_EQ(3, s.getLength());
}

TEST(MySetInsertTest, FrontMiddleBack) {
    MySet s = make_set({10, 30});
    s.insert(5);   // 앞
    s.insert(20);  // 중간
    s.insert(40);  // 뒤
    EXPECT_EQ((Vec{5, 10, 20, 30, 40}), elements(s));
}

TEST(MySetInsertTest, NegativeAndExtremeValues) {
    MySet s = make_set({0, INT_MAX, -7, INT_MIN + 1, -1});
    EXPECT_EQ((Vec{INT_MIN + 1, -7, -1, 0, INT_MAX}), elements(s));
}

TEST(MySetInsertTest, GrowsCapacityWhenFull) {
    MySet s;  // capacity 4
    for (int i = 0; i < 5; ++i) s.insert(i);
    EXPECT_EQ(5, s.getLength());
    EXPECT_EQ(8, s.getCapacity());
    expect_invariant(s);
}

TEST(MySetInsertTest, DuplicateDoesNotGrowCapacity) {
    MySet s = make_set({1, 2, 3, 4});  // 가득 참
    s.insert(2);
    EXPECT_EQ(4, s.getLength());
    EXPECT_EQ(4, s.getCapacity());
}

TEST(MySetInsertTest, MatchesStdSetOnRandomData) {
    std::mt19937 rng(7);
    for (int trial = 0; trial < 50; ++trial) {
        Vec v = random_vec(rng, 1 + static_cast<int>(rng() % 300), 200);
        MySet s = make_set(v);
        ASSERT_EQ(sorted_unique(v), elements(s)) << "trial " << trial;
        expect_invariant(s);
    }
}

// =====================================================================
// binarySearch (private) - [left, right) 반열린 구간
// =====================================================================

TEST(MySetBinarySearchTest, ExistingValueReturnsMinusOne) {
    MySet s = make_set({1, 3, 5, 7, 9});
    for (int v : {1, 3, 5, 7, 9}) {
        EXPECT_EQ(-1, s.binarySearch(0, s.size, v)) << "value " << v;
    }
}

TEST(MySetBinarySearchTest, MissingValueReturnsInsertionPoint) {
    MySet s = make_set({10, 20, 30, 40});
    EXPECT_EQ(0, s.binarySearch(0, 4, 5));
    EXPECT_EQ(1, s.binarySearch(0, 4, 15));
    EXPECT_EQ(2, s.binarySearch(0, 4, 25));
    EXPECT_EQ(3, s.binarySearch(0, 4, 35));
    EXPECT_EQ(4, s.binarySearch(0, 4, 45));
}

TEST(MySetBinarySearchTest, EmptyRange) {
    MySet s;
    EXPECT_EQ(0, s.binarySearch(0, 0, 42));
}

TEST(MySetBinarySearchTest, RightIsExclusive) {
    MySet s = make_set({1, 2, 3, 4, 5});
    // [0, 3) 에는 4 가 없다 -> 삽입 위치 3
    EXPECT_EQ(3, s.binarySearch(0, 3, 4));
    // [0, 4) 에는 4 가 있다
    EXPECT_EQ(-1, s.binarySearch(0, 4, 4));
}

TEST(MySetBinarySearchTest, MatchesLowerBoundOnRandomData) {
    std::mt19937 rng(17);
    for (int trial = 0; trial < 300; ++trial) {
        Vec v = sorted_unique(random_vec(rng, 1 + static_cast<int>(rng() % 100), 200));
        MySet s = make_set(v);
        const int value = static_cast<int>(rng() % 220);
        const int idx = s.binarySearch(0, s.size, value);
        if (std::binary_search(v.begin(), v.end(), value)) {
            ASSERT_EQ(-1, idx) << "trial " << trial << ", value " << value;
        } else {
            const int expected = static_cast<int>(
                    std::lower_bound(v.begin(), v.end(), value) - v.begin());
            ASSERT_EQ(expected, idx) << "trial " << trial << ", value " << value;
        }
    }
}

// =====================================================================
// resize
// =====================================================================

TEST(MySetResizeTest, DoublesCapacityAndKeepsContent) {
    MySet s = make_set({1, 2, 3, 4});
    s.resize();
    EXPECT_EQ(8, s.getCapacity());
    s.resize();
    EXPECT_EQ(16, s.getCapacity());
    EXPECT_EQ((Vec{1, 2, 3, 4}), elements(s));
}

TEST(MySetResizeTest, ZeroCapacityBecomesFour) {
    MySet s(0);
    s.resize();
    EXPECT_EQ(4, s.getCapacity());
    EXPECT_EQ(0, s.getLength());
}

TEST(MySetResizeTest, NewSpaceIsWritable) {
    MySet s = make_set({1, 2, 3, 4});
    s.resize();
    s.resize();
    // 새로 생긴 칸 전부에 접근 (ASan 으로 범위 검사)
    for (int i = 4; i < s.capacity; ++i) s.array[i] = i + 1;
    s.size = s.capacity;
    for (int i = 0; i < s.size; ++i) EXPECT_EQ(i + 1, s.array[i]);
}

// =====================================================================
// get / clear
// =====================================================================

TEST(MySetGetTest, OutOfRangeReturnsIntMin) {
    MySet s = make_set({1, 2});
    EXPECT_EQ(1, s.get(0));
    EXPECT_EQ(2, s.get(1));
    EXPECT_EQ(INT_MIN, s.get(2));
    EXPECT_EQ(INT_MIN, s.get(-1));
    EXPECT_EQ(INT_MIN, MySet().get(0));
}

TEST(MySetClearTest, EmptiesButKeepsCapacityAndIsReusable) {
    MySet s = make_set({1, 2, 3, 4, 5});
    const int cap = s.getCapacity();
    s.clear();
    EXPECT_EQ(0, s.getLength());
    EXPECT_EQ(cap, s.getCapacity());
    s.insert(9);
    s.insert(8);
    EXPECT_EQ((Vec{8, 9}), elements(s));
}

// =====================================================================
// 복사 / 이동
// =====================================================================

TEST(MySetCopyTest, CopyConstructorIsDeep) {
    MySet a = make_set({1, 2, 3});
    MySet b(a);
    EXPECT_NE(a.array, b.array);
    b.insert(4);
    a.insert(0);
    EXPECT_EQ((Vec{0, 1, 2, 3}), elements(a));
    EXPECT_EQ((Vec{1, 2, 3, 4}), elements(b));
}

TEST(MySetCopyTest, CopyAssignmentIsDeep) {
    MySet a = make_set({1, 2, 3});
    MySet b = make_set({7, 8, 9, 10, 11});
    b = a;
    EXPECT_NE(a.array, b.array);
    EXPECT_EQ(elements(a), elements(b));
    EXPECT_EQ(a.getCapacity(), b.getCapacity());
    b.insert(100);
    EXPECT_EQ((Vec{1, 2, 3}), elements(a));
}

TEST(MySetCopyTest, SelfAssignmentKeepsContent) {
    MySet a = make_set({1, 2, 3});
    MySet &ref = a;
    a = ref;
    EXPECT_EQ((Vec{1, 2, 3}), elements(a));
}

TEST(MySetCopyTest, CopyOfMovedFromSet) {
    MySet a = make_set({1, 2});
    MySet b(std::move(a));
    MySet c(a);  // capacity 0, array nullptr 인 집합 복사
    EXPECT_EQ(0, c.getLength());
    c.insert(5);
    EXPECT_EQ((Vec{5}), elements(c));
}

TEST(MySetMoveTest, MoveConstructorStealsBuffer) {
    MySet a = make_set({1, 2, 3});
    int *buffer = a.array;
    MySet b(std::move(a));
    EXPECT_EQ(buffer, b.array);
    EXPECT_EQ((Vec{1, 2, 3}), elements(b));
    EXPECT_EQ(nullptr, a.array);
    EXPECT_EQ(0, a.getLength());
    EXPECT_EQ(0, a.getCapacity());
}

TEST(MySetMoveTest, MoveAssignmentStealsBuffer) {
    MySet a = make_set({1, 2, 3});
    MySet b = make_set({9, 8});
    int *buffer = a.array;
    b = std::move(a);
    EXPECT_EQ(buffer, b.array);
    EXPECT_EQ((Vec{1, 2, 3}), elements(b));
    EXPECT_EQ(0, a.getLength());
}

TEST(MySetMoveTest, MovedFromSetIsReusable) {
    MySet a = make_set({1, 2, 3});
    MySet b(std::move(a));
    a.insert(3);
    a.insert(1);
    EXPECT_EQ((Vec{1, 3}), elements(a));
    expect_invariant(a);
}

TEST(MySetMoveTest, SelfMoveAssignmentKeepsContent) {
    MySet a = make_set({1, 2, 3});
    MySet &ref = a;
    a = std::move(ref);
    EXPECT_EQ((Vec{1, 2, 3}), elements(a));
}

TEST(MySetMoveTest, MoveOperationsAreNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<MySet>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<MySet>::value);
}

TEST(MySetMoveTest, WorksInsideStdVector) {
    std::vector<MySet> sets;
    for (int i = 0; i < 50; ++i) sets.push_back(make_set({i, i + 1}));
    for (int i = 0; i < 50; ++i) EXPECT_EQ((Vec{i, i + 1}), elements(sets[i]));
}

// =====================================================================
// 집합 연산
// =====================================================================

TEST(MySetOperationTest, Union) {
    MySet a = make_set({1, 3, 5}), b = make_set({2, 3, 4});
    EXPECT_EQ((Vec{1, 2, 3, 4, 5}), elements(a + b));
    EXPECT_EQ(elements(a + b), elements(a.unionSet(b)));
}

TEST(MySetOperationTest, Intersection) {
    MySet a = make_set({1, 3, 5, 7}), b = make_set({3, 4, 5});
    EXPECT_EQ((Vec{3, 5}), elements(a & b));
    EXPECT_EQ(elements(a & b), elements(a.intersectionSet(b)));
}

TEST(MySetOperationTest, Difference) {
    MySet a = make_set({1, 3, 5, 7}), b = make_set({3, 4, 5});
    EXPECT_EQ((Vec{1, 7}), elements(a - b));
    EXPECT_EQ((Vec{4}), elements(b - a));
    EXPECT_EQ(elements(a - b), elements(a.differenceSet(b)));
}

TEST(MySetOperationTest, WithEmptySet) {
    MySet a = make_set({1, 2, 3}), e;
    EXPECT_EQ((Vec{1, 2, 3}), elements(a + e));
    EXPECT_EQ((Vec{1, 2, 3}), elements(e + a));
    EXPECT_EQ(Vec{}, elements(a & e));
    EXPECT_EQ(Vec{}, elements(e & a));
    EXPECT_EQ((Vec{1, 2, 3}), elements(a - e));
    EXPECT_EQ(Vec{}, elements(e - a));
    EXPECT_EQ(Vec{}, elements(e + e));
}

TEST(MySetOperationTest, WithMovedFromSet) {
    MySet a = make_set({1, 2, 3});
    MySet b(std::move(a));  // a: array nullptr
    EXPECT_EQ((Vec{1, 2, 3}), elements(a + b));
    EXPECT_EQ(Vec{}, elements(a & b));
    EXPECT_EQ((Vec{1, 2, 3}), elements(b - a));
}

TEST(MySetOperationTest, DisjointAndIdentical) {
    MySet a = make_set({1, 2, 3}), b = make_set({4, 5, 6});
    EXPECT_EQ((Vec{1, 2, 3, 4, 5, 6}), elements(a + b));
    EXPECT_EQ(Vec{}, elements(a & b));
    EXPECT_EQ((Vec{1, 2, 3}), elements(a - b));

    MySet c = a;
    EXPECT_EQ((Vec{1, 2, 3}), elements(a + c));
    EXPECT_EQ((Vec{1, 2, 3}), elements(a & c));
    EXPECT_EQ(Vec{}, elements(a - c));
}

TEST(MySetOperationTest, OperandsAreNotModified) {
    MySet a = make_set({1, 2, 3}), b = make_set({2, 3, 4});
    MySet u = a + b, i = a & b, d = a - b;
    EXPECT_EQ((Vec{1, 2, 3}), elements(a));
    EXPECT_EQ((Vec{2, 3, 4}), elements(b));
}

TEST(MySetOperationTest, ResultsKeepInvariant) {
    MySet a = make_set({1, 4, 9, 16, 25}), b = make_set({4, 8, 16, 32});
    expect_invariant(a + b);
    expect_invariant(a & b);
    expect_invariant(a - b);
}

TEST(MySetOperationTest, ChainedOperations) {
    MySet a = make_set({1, 2, 3, 4}), b = make_set({3, 4, 5}), c = make_set({4, 6});
    EXPECT_EQ((Vec{1, 2, 3, 5, 6}), elements((a + b + c) - (a & b & c)));
}

TEST(MySetOperationTest, MatchesStdAlgorithmsOnRandomData) {
    std::mt19937 rng(31);
    for (int trial = 0; trial < 30; ++trial) {
        Vec va = sorted_unique(random_vec(rng, static_cast<int>(rng() % 500), 400));
        Vec vb = sorted_unique(random_vec(rng, static_cast<int>(rng() % 500), 400));
        Vec eu, ei, ed;
        std::set_union(va.begin(), va.end(), vb.begin(), vb.end(), std::back_inserter(eu));
        std::set_intersection(va.begin(), va.end(), vb.begin(), vb.end(), std::back_inserter(ei));
        std::set_difference(va.begin(), va.end(), vb.begin(), vb.end(), std::back_inserter(ed));

        MySet a = make_set(va), b = make_set(vb);
        ASSERT_EQ(eu, elements(a + b)) << "trial " << trial;
        ASSERT_EQ(ei, elements(a & b)) << "trial " << trial;
        ASSERT_EQ(ed, elements(a - b)) << "trial " << trial;
    }
}

// =====================================================================
// 입출력
// =====================================================================

TEST(MySetIoTest, OutputFormat) {
    EXPECT_EQ("{}", to_string(MySet()));
    EXPECT_EQ("{7}", to_string(make_set({7})));
    EXPECT_EQ("{-3, 0, 12}", to_string(make_set({12, -3, 0})));
}

TEST(MySetIoTest, ReadsUntilMinusOne) {
    EXPECT_EQ((Vec{1, 2, 3}), elements(read_set("3 1 2 -1")));
}

TEST(MySetIoTest, ImmediateMinusOneGivesEmptySet) {
    EXPECT_EQ(Vec{}, elements(read_set("-1")));
}

TEST(MySetIoTest, DuplicatesAndOtherNegativesInInput) {
    EXPECT_EQ((Vec{-5, 0, 2}), elements(read_set("2 -5 2 0 -5 -1")));
}

TEST(MySetIoTest, StopsAtMinusOneAndLeavesRest) {
    std::istringstream in("1 2 -1 3 4 -1");
    MySet a, b;
    in >> a >> b;
    EXPECT_EQ((Vec{1, 2}), elements(a));
    EXPECT_EQ((Vec{3, 4}), elements(b));
}

TEST(MySetIoTest, ReadingClearsPreviousContent) {
    MySet s = make_set({100, 200});
    std::istringstream in("1 -1");
    in >> s;
    EXPECT_EQ((Vec{1}), elements(s));
}

TEST(MySetIoTest, LargeInputMatchesStdSet) {
    std::mt19937 rng(29);
    Vec v = random_vec(rng, 5000, 1000);
    std::ostringstream text;
    for (int x : v) text << x << ' ';
    text << "-1";
    MySet s = read_set(text.str());
    EXPECT_EQ(sorted_unique(v), elements(s));
    expect_invariant(s);
}

TEST(MySetIoTest, OutputThenInputRoundTrip) {
    MySet s = make_set({5, -2, 9, 0});
    std::string text;
    for (int i = 0; i < s.getLength(); ++i) text += std::to_string(s.get(i)) + " ";
    text += "-1";
    EXPECT_EQ(elements(s), elements(read_set(text)));
}