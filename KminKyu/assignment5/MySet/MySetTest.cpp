#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// private 멤버(timSort, mergeSort, insertionSort, binarySearch, nomarlize, resize,
// array, size, capacity)를 직접 테스트하기 위한 트릭.
// 반드시 표준 헤더를 모두 include 한 "뒤에", MySet.h 를 include 하기 "전에" 정의해야 한다.
// 그래서 MySetTest.cpp 와 파일을 분리했다.
#define private public
#include "MySet.h"
#undef private

// 빌드 예시:
//   g++ -std=c++17 -g -fsanitize=address MySetSortTest.cpp MySet.cpp -lgtest -lgtest_main -pthread -o MySetSortTest
//
// 사용한 가정 (구현과 다르면 해당 테스트만 고치세요)
//  1) insertionSort(left, right) : [left, right] 폐구간(양 끝 포함)만 오름차순 정렬
//  2) mergeSort(left, mid, right) : [left, mid] 와 [mid+1, right] 가 각각 이미 정렬되어 있을 때
//                                   [left, right] 전체를 오름차순으로 병합 (폐구간)
//  3) binarySearch(left, right, value) : 정렬된 [left, right] 에서 value 의 인덱스 반환.
//                                   없으면 -1 또는 삽입 위치(left..right+1) 를 반환해도 허용
//  4) nomarlize() : 정렬 + 중복 제거 후 size 갱신
//  5) resize(newCapacity) : 기존 원소와 size 는 유지, capacity 만 변경 (size 보다 작게 줄이는 경우는 테스트 안 함)
//  6) timSort() : [0, size) 전체 오름차순 정렬

namespace {
    using Vec = std::vector<int>;

    const int MIN_RUN_VALUE = 32; // ODR-use 를 피하기 위해 지역 상수로 복사

    // 원하는 내용의 배열을 직접 가진 MySet 생성 (중복 허용 - 정렬 함수 단위 테스트용)
    MySet make_raw(const Vec &values) {
        const int n = static_cast<int>(values.size());
        MySet s(0, std::max(4, n));
        for (int i = 0; i < n; ++i) s.array[i] = values[i];
        s.size = n;
        return s;
    }

    Vec raw(const MySet &s) {
        return Vec(s.array, s.array + s.size);
    }

    Vec random_vec(std::mt19937 &rng, int n, int maxValue) {
        Vec v(n);
        for (int &x: v) x = static_cast<int>(rng() % maxValue) + 1;
        return v;
    }

    Vec sorted_random_vec(std::mt19937 &rng, int n, int maxValue) {
        Vec v = random_vec(rng, n, maxValue);
        std::sort(v.begin(), v.end());
        return v;
    }

    template<typename F>
    long long measure_ms(F f) {
        const auto start = std::chrono::steady_clock::now();
        f();
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count();
    }
}

// =====================================================================
// timSort
// =====================================================================

TEST(MySetTimSortTest, EmptyArray) {
    MySet s;
    s.timSort();
    EXPECT_EQ(0, s.size);
}

TEST(MySetTimSortTest, SingleElement) {
    MySet s = make_raw({5});
    s.timSort();
    EXPECT_EQ(Vec{5}, raw(s));
}

TEST(MySetTimSortTest, TwoElements) {
    MySet s = make_raw({9, 3});
    s.timSort();
    EXPECT_EQ((Vec{3, 9}), raw(s));
}

TEST(MySetTimSortTest, AlreadySorted) {
    Vec v;
    for (int i = 1; i <= 100; ++i) v.push_back(i);
    MySet s = make_raw(v);
    s.timSort();
    EXPECT_EQ(v, raw(s));
}

TEST(MySetTimSortTest, ReverseSorted) {
    Vec v;
    for (int i = 100; i >= 1; --i) v.push_back(i);
    MySet s = make_raw(v);
    s.timSort();
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v, raw(s));
}

TEST(MySetTimSortTest, AllEqual) {
    MySet s = make_raw(Vec(77, 4));
    s.timSort();
    EXPECT_EQ(Vec(77, 4), raw(s));
}

TEST(MySetTimSortTest, WithDuplicates) {
    Vec v = {5, 1, 5, 2, 2, 9, 1, 5, 3, 3};
    MySet s = make_raw(v);
    s.timSort();
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v, raw(s));
}

TEST(MySetTimSortTest, SizeAndCapacityAreNotChanged) {
    std::mt19937 rng(1);
    MySet s = make_raw(random_vec(rng, 50, 1000));
    const int size = s.size, capacity = s.capacity;
    s.timSort();
    EXPECT_EQ(size, s.size);
    EXPECT_EQ(capacity, s.capacity);
}

TEST(MySetTimSortTest, SortsOnlyUpToSizeNotCapacity) {
    MySet s(0, 10);
    const Vec v = {5, 4, 3, 2, 1};
    for (size_t i = 0; i < v.size(); ++i) s.array[i] = v[i];
    s.size = 5;
    s.array[5] = -77; // size 바깥 값은 건드리면 안 됨
    s.timSort();
    EXPECT_EQ((Vec{1, 2, 3, 4, 5}), raw(s));
    EXPECT_EQ(-77, s.array[5]);
}

// MIN_RUN 경계(run 하나 / run 여러 개 / 병합 단계) 크기들
TEST(MySetTimSortTest, SizesAroundMinRun) {
    std::mt19937 rng(20261024);
    const int base = MIN_RUN_VALUE;
    const Vec sizes = {base - 1, base, base + 1, 2 * base - 1, 2 * base, 2 * base + 1,
                       3 * base + 5, 4 * base, 4 * base + 1, 7 * base + 3, 10 * base - 1};
    for (int n: sizes) {
        Vec v = random_vec(rng, n, 100000);
        MySet s = make_raw(v);
        s.timSort();
        std::sort(v.begin(), v.end());
        ASSERT_EQ(v, raw(s)) << "size: " << n;
    }
}

TEST(MySetTimSortTest, RandomManyTrials) {
    std::mt19937 rng(7);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = static_cast<int>(rng() % 400);
        const int maxValue = 1 + static_cast<int>(rng() % 200); // 값 범위가 좁으면 중복이 많아짐
        Vec v = random_vec(rng, n, maxValue);
        MySet s = make_raw(v);
        s.timSort();
        std::sort(v.begin(), v.end());
        ASSERT_EQ(v, raw(s)) << "trial: " << trial << ", n: " << n;
    }
}

TEST(MySetTimSortTest, PartiallySortedRuns) {
    // 정렬된 구간 + 역순 구간이 섞여 있는 입력
    Vec v;
    for (int i = 0; i < 500; ++i) v.push_back(i);
    for (int i = 1000; i > 500; --i) v.push_back(i);
    for (int i = 0; i < 500; ++i) v.push_back(i * 2);
    MySet s = make_raw(v);
    s.timSort();
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v, raw(s));
}

TEST(MySetTimSortTest, NegativeAndZeroValues) {
    Vec v = {3, -1, 0, -5, 2, 0, -1};
    MySet s = make_raw(v);
    s.timSort();
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v, raw(s));
}

// ---------- timSort 속도 ----------

TEST(MySetTimSortPerformanceTest, RandomOneMillion) {
    std::mt19937 rng(1);
    Vec v = random_vec(rng, 1000000, 1000000000);
    MySet s = make_raw(v);
    const long long ms = measure_ms([&] { s.timSort(); });
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v, raw(s));
    EXPECT_LT(ms, 2000) << ms << "ms";
}

TEST(MySetTimSortPerformanceTest, SortedAndReverseOneMillion) {
    Vec asc(1000000);
    for (int i = 0; i < 1000000; ++i) asc[i] = i;
    Vec desc(asc.rbegin(), asc.rend());

    MySet s1 = make_raw(asc), s2 = make_raw(desc);
    const long long ms = measure_ms([&] {
        s1.timSort();
        s2.timSort();
    });
    EXPECT_EQ(asc, raw(s1));
    EXPECT_EQ(asc, raw(s2));
    EXPECT_LT(ms, 2000) << ms << "ms";
}

TEST(MySetTimSortPerformanceTest, ManyDuplicatesOneMillion) {
    std::mt19937 rng(2);
    Vec v = random_vec(rng, 1000000, 5);
    MySet s = make_raw(v);
    const long long ms = measure_ms([&] { s.timSort(); });
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v, raw(s));
    EXPECT_LT(ms, 2000) << ms << "ms";
}

// =====================================================================
// insertionSort(left, right)
// =====================================================================

TEST(MySetInsertionSortTest, SortsWholeArray) {
    Vec v = {5, 2, 4, 6, 1, 3};
    MySet s = make_raw(v);
    s.insertionSort(0, 5);
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v, raw(s));
}

TEST(MySetInsertionSortTest, SortsOnlyGivenRange) {
    MySet s = make_raw({9, 8, 7, 6, 5, 4, 3, 2, 1});
    s.insertionSort(2, 5); // 인덱스 2..5 : 7 6 5 4 -> 4 5 6 7
    EXPECT_EQ((Vec{9, 8, 4, 5, 6, 7, 3, 2, 1}), raw(s));
}

TEST(MySetInsertionSortTest, SingleElementRangeDoesNothing) {
    Vec v = {3, 1, 2};
    MySet s = make_raw(v);
    s.insertionSort(1, 1);
    EXPECT_EQ(v, raw(s));
}

TEST(MySetInsertionSortTest, RangeIncludesLastIndex) {
    MySet s = make_raw({1, 9, 8});
    s.insertionSort(1, 2);
    EXPECT_EQ((Vec{1, 8, 9}), raw(s));
}

TEST(MySetInsertionSortTest, AlreadySortedAndReverse) {
    MySet a = make_raw({1, 2, 3, 4, 5});
    a.insertionSort(0, 4);
    EXPECT_EQ((Vec{1, 2, 3, 4, 5}), raw(a));

    MySet b = make_raw({5, 4, 3, 2, 1});
    b.insertionSort(0, 4);
    EXPECT_EQ((Vec{1, 2, 3, 4, 5}), raw(b));
}

TEST(MySetInsertionSortTest, DuplicatesKeepValuesIntact) {
    Vec v = {3, 1, 3, 2, 1, 2};
    MySet s = make_raw(v);
    s.insertionSort(0, 5);
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v, raw(s));
}

TEST(MySetInsertionSortTest, RandomRanges) {
    std::mt19937 rng(11);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 1 + static_cast<int>(rng() % 100);
        Vec v = random_vec(rng, n, 50);
        const int a = static_cast<int>(rng() % n), b = static_cast<int>(rng() % n);
        const int left = std::min(a, b), right = std::max(a, b);

        MySet s = make_raw(v);
        s.insertionSort(left, right);
        std::sort(v.begin() + left, v.begin() + right + 1);
        ASSERT_EQ(v, raw(s)) << "trial " << trial << " range [" << left << ", " << right << "]";
    }
}

// =====================================================================
// mergeSort(left, mid, right) : 정렬된 두 구간 병합
// =====================================================================

TEST(MySetMergeSortTest, MergesTwoSortedHalves) {
    MySet s = make_raw({1, 3, 5, 7, 2, 4, 6, 8});
    s.mergeSort(0, 3, 7);
    EXPECT_EQ((Vec{1, 2, 3, 4, 5, 6, 7, 8}), raw(s));
}

TEST(MySetMergeSortTest, UnevenHalves) {
    MySet s = make_raw({10, 2, 3, 4, 5, 6, 7});
    s.mergeSort(0, 0, 6);
    EXPECT_EQ((Vec{2, 3, 4, 5, 6, 7, 10}), raw(s));

    MySet t = make_raw({2, 3, 4, 5, 6, 7, 1});
    t.mergeSort(0, 5, 6);
    EXPECT_EQ((Vec{1, 2, 3, 4, 5, 6, 7}), raw(t));
}

TEST(MySetMergeSortTest, TwoSingleElements) {
    MySet s = make_raw({2, 1});
    s.mergeSort(0, 0, 1);
    EXPECT_EQ((Vec{1, 2}), raw(s));
}

TEST(MySetMergeSortTest, AlreadyOrderedHalves) {
    MySet s = make_raw({1, 2, 3, 4, 5, 6});
    s.mergeSort(0, 2, 5);
    EXPECT_EQ((Vec{1, 2, 3, 4, 5, 6}), raw(s));
}

TEST(MySetMergeSortTest, CompletelyInterleavedAndSwapped) {
    MySet s = make_raw({4, 5, 6, 1, 2, 3});
    s.mergeSort(0, 2, 5);
    EXPECT_EQ((Vec{1, 2, 3, 4, 5, 6}), raw(s));
}

TEST(MySetMergeSortTest, DuplicatesAcrossHalves) {
    MySet s = make_raw({1, 2, 2, 3, 2, 3, 3, 4});
    s.mergeSort(0, 3, 7);
    EXPECT_EQ((Vec{1, 2, 2, 2, 3, 3, 3, 4}), raw(s));
}

TEST(MySetMergeSortTest, OnlyMergesGivenRange) {
    MySet s = make_raw({100, 3, 6, 1, 2, 9, -100});
    s.mergeSort(1, 2, 5); // [3 6] + [1 2 9] -> 1 2 3 6 9
    EXPECT_EQ((Vec{100, 1, 2, 3, 6, 9, -100}), raw(s));
}

TEST(MySetMergeSortTest, RandomMerges) {
    std::mt19937 rng(13);
    for (int trial = 0; trial < 300; ++trial) {
        const int n1 = 1 + static_cast<int>(rng() % 60), n2 = 1 + static_cast<int>(rng() % 60);
        const int pad = static_cast<int>(rng() % 5); // 앞쪽 여백 -> left != 0 인 경우
        Vec v = random_vec(rng, pad, 100);
        const Vec first = sorted_random_vec(rng, n1, 80), second = sorted_random_vec(rng, n2, 80);
        v.insert(v.end(), first.begin(), first.end());
        v.insert(v.end(), second.begin(), second.end());
        const Vec tail = random_vec(rng, static_cast<int>(rng() % 5), 100); // 뒤쪽 여백
        v.insert(v.end(), tail.begin(), tail.end());

        const int left = pad, mid = pad + n1 - 1, right = pad + n1 + n2 - 1;
        MySet s = make_raw(v);
        s.mergeSort(left, mid, right);
        std::inplace_merge(v.begin() + left, v.begin() + mid + 1, v.begin() + right + 1);
        ASSERT_EQ(v, raw(s)) << "trial " << trial;
    }
}

// =====================================================================
// binarySearch(left, right, value)
// =====================================================================

TEST(MySetBinarySearchTest, FindsEveryExistingValue) {
    MySet s = make_raw({1, 3, 5, 7, 9, 11, 13});
    for (int i = 0; i < s.size; ++i) {
        const int idx = s.binarySearch(0, s.size - 1, s.array[i]);
        EXPECT_EQ(i, idx) << "value: " << s.array[i];
    }
}

TEST(MySetBinarySearchTest, FindsFirstAndLast) {
    MySet s = make_raw({2, 4, 6, 8});
    EXPECT_EQ(0, s.binarySearch(0, 3, 2));
    EXPECT_EQ(3, s.binarySearch(0, 3, 8));
}

TEST(MySetBinarySearchTest, SingleElement) {
    MySet s = make_raw({5});
    EXPECT_EQ(0, s.binarySearch(0, 0, 5));
}

TEST(MySetBinarySearchTest, FindsOnlyInsideGivenRange) {
    MySet s = make_raw({1, 2, 3, 4, 5, 6, 7});
    EXPECT_EQ(3, s.binarySearch(2, 5, 4));
    // 값 1 은 배열에는 있지만 [2, 5] 범위 밖 -> 찾으면 안 됨
    const int idx = s.binarySearch(2, 5, 1);
    EXPECT_TRUE(idx < 2 || idx > 5 || s.array[idx] != 1) << "idx: " << idx;
}

// 없는 값: -1 이거나, 정렬을 유지하는 삽입 위치 [left, right+1] 중 하나이면 통과
TEST(MySetBinarySearchTest, MissingValueGivesMinusOneOrInsertionPoint) {
    MySet s = make_raw({10, 20, 30, 40, 50});
    for (int value: {5, 15, 25, 35, 45, 55}) {
        const int idx = s.binarySearch(0, 4, value);
        if (idx == -1) continue;
        ASSERT_GE(idx, 0) << "value: " << value;
        ASSERT_LE(idx, 5) << "value: " << value;
        for (int i = 0; i < idx; ++i) EXPECT_LT(s.array[i], value) << "value: " << value;
        for (int i = idx; i < 5; ++i) EXPECT_GT(s.array[i], value) << "value: " << value;
    }
}

TEST(MySetBinarySearchTest, MatchesStdBinarySearchOnRandomData) {
    std::mt19937 rng(17);
    for (int trial = 0; trial < 200; ++trial) {
        Vec v = sorted_random_vec(rng, 1 + static_cast<int>(rng() % 100), 200);
        v.erase(std::unique(v.begin(), v.end()), v.end());
        MySet s = make_raw(v);
        const int value = 1 + static_cast<int>(rng() % 200);
        const int idx = s.binarySearch(0, s.size - 1, value);
        if (std::binary_search(v.begin(), v.end(), value)) {
            ASSERT_GE(idx, 0) << "trial " << trial;
            ASSERT_LT(idx, s.size) << "trial " << trial;
            ASSERT_EQ(value, s.array[idx]) << "trial " << trial;
        } else if (idx >= 0 && idx < s.size) {
            ASSERT_NE(value, s.array[idx]) << "trial " << trial; // 없는 값을 있다고 하면 안 됨
        }
    }
}

// =====================================================================
// nomarlize() : 정렬 + 중복 제거
// =====================================================================

TEST(MySetNormalizeTest, SortsAndRemovesDuplicates) {
    MySet s = make_raw({2, 3, 1, 2, 4, 3, 1});
    s.nomarlize();
    EXPECT_EQ((Vec{1, 2, 3, 4}), raw(s));
    EXPECT_EQ(4, s.size);
}

TEST(MySetNormalizeTest, EmptyAndSingle) {
    MySet e;
    e.nomarlize();
    EXPECT_EQ(0, e.size);

    MySet s = make_raw({7});
    s.nomarlize();
    EXPECT_EQ(Vec{7}, raw(s));
}

TEST(MySetNormalizeTest, AllEqualBecomesOne) {
    MySet s = make_raw(Vec(50, 3));
    s.nomarlize();
    EXPECT_EQ(Vec{3}, raw(s));
}

TEST(MySetNormalizeTest, AlreadyNormalizedIsUnchanged) {
    MySet s = make_raw({1, 2, 3, 4, 5});
    s.nomarlize();
    EXPECT_EQ((Vec{1, 2, 3, 4, 5}), raw(s));
}

TEST(MySetNormalizeTest, CapacityStaysEnough) {
    std::mt19937 rng(3);
    MySet s = make_raw(random_vec(rng, 200, 20));
    s.nomarlize();
    EXPECT_GE(s.capacity, s.size);
}

TEST(MySetNormalizeTest, RandomMatchesStdSet) {
    std::mt19937 rng(19);
    for (int trial = 0; trial < 200; ++trial) {
        Vec v = random_vec(rng, static_cast<int>(rng() % 300), 1 + static_cast<int>(rng() % 100));
        const std::set<int> expected(v.begin(), v.end());
        MySet s = make_raw(v);
        s.nomarlize();
        ASSERT_EQ(Vec(expected.begin(), expected.end()), raw(s)) << "trial " << trial;
    }
}

TEST(MySetNormalizeTest, LargeInputIsFast) {
    std::mt19937 rng(23);
    Vec v = random_vec(rng, 500000, 100000);
    const std::set<int> expected(v.begin(), v.end());
    MySet s = make_raw(v);
    const long long ms = measure_ms([&] { s.nomarlize(); });
    EXPECT_EQ(Vec(expected.begin(), expected.end()), raw(s));
    EXPECT_LT(ms, 2000) << ms << "ms";
}

// =====================================================================
// resize(newCapacity)
// =====================================================================

TEST(MySetResizeTest, GrowKeepsContent) {
    MySet s = make_raw({1, 2, 3, 4});
    s.resize(100);
    EXPECT_EQ(100, s.capacity);
    EXPECT_EQ(4, s.size);
    EXPECT_EQ((Vec{1, 2, 3, 4}), raw(s));
}

TEST(MySetResizeTest, ResizedArrayIsUsable) {
    MySet s = make_raw({1, 2, 3, 4});
    s.resize(1000);
    for (int i = 4; i < 1000; ++i) s.array[i] = i + 1; // 새 공간 전체 접근 (ASan 으로 범위 검사)
    s.size = 1000;
    for (int i = 0; i < 1000; ++i) EXPECT_EQ(i + 1, s.array[i]);
}

TEST(MySetResizeTest, SameCapacityKeepsContent) {
    MySet s = make_raw({4, 3, 2, 1});
    s.resize(s.capacity);
    EXPECT_EQ((Vec{4, 3, 2, 1}), raw(s));
}

TEST(MySetResizeTest, ShrinkToSizeKeepsContent) {
    MySet s(0, 100);
    for (int i = 0; i < 10; ++i) s.array[i] = i + 1;
    s.size = 10;
    s.resize(10);
    EXPECT_EQ(10, s.capacity);
    EXPECT_EQ(10, s.size);
    EXPECT_EQ((Vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), raw(s));
}

TEST(MySetResizeTest, RepeatedResizeKeepsContent) {
    MySet s = make_raw({1, 2, 3});
    for (int cap = 8; cap <= 4096; cap *= 2) s.resize(cap);
    EXPECT_EQ((Vec{1, 2, 3}), raw(s));
}

TEST(MySetResizeTest, EmptySetResize) {
    MySet s;
    s.resize(50);
    EXPECT_EQ(50, s.capacity);
    EXPECT_EQ(0, s.size);
}

// =====================================================================
// 공개 API 와 연결 (정렬/정규화가 실제 집합 연산에서 올바르게 쓰이는지)
// =====================================================================

namespace {
    MySet read_set(const std::string &text) {
        std::istringstream in(text);
        MySet s;
        in >> s;
        return s;
    }

    Vec public_elements(const MySet &s) {
        Vec v;
        for (int i = 0; i < s.getLength(); ++i) v.push_back(s.get(i));
        std::sort(v.begin(), v.end());
        return v;
    }
}

TEST(MySetSortIntegrationTest, InputWithDuplicatesAndLargeSize) {
    std::mt19937 rng(29);
    Vec v = random_vec(rng, 5000, 1000);
    std::ostringstream text;
    for (int x: v) text << x << ' ';
    text << "-1";

    const std::set<int> expected(v.begin(), v.end());
    MySet s = read_set(text.str());
    EXPECT_EQ(Vec(expected.begin(), expected.end()), public_elements(s));
}

TEST(MySetSortIntegrationTest, SetOperationsAfterManyResizes) {
    std::mt19937 rng(31);
    Vec va = random_vec(rng, 3000, 2000), vb = random_vec(rng, 3000, 2000);
    std::ostringstream ta, tb;
    for (int x: va) ta << x << ' ';
    for (int x: vb) tb << x << ' ';
    ta << "-1";
    tb << "-1";

    const std::set<int> sa(va.begin(), va.end()), sb(vb.begin(), vb.end());
    Vec eu, ei, ed;
    std::set_union(sa.begin(), sa.end(), sb.begin(), sb.end(), std::back_inserter(eu));
    std::set_intersection(sa.begin(), sa.end(), sb.begin(), sb.end(), std::back_inserter(ei));
    std::set_difference(sa.begin(), sa.end(), sb.begin(), sb.end(), std::back_inserter(ed));

    MySet a = read_set(ta.str()), b = read_set(tb.str());
    MySet u = a + b, i = a & b, d = a - b;
    EXPECT_EQ(eu, public_elements(u));
    EXPECT_EQ(ei, public_elements(i));
    EXPECT_EQ(ed, public_elements(d));
}