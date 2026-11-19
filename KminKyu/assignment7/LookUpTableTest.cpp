#include <gtest/gtest.h>
#include <algorithm>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "LookUpTable.h"
#include "MyString.h"

// =====================================================================
// LookUpTable 테스트
//
//  - key 순서로 정렬된 배열 + 이진 탐색으로 동작하는 테이블을 검증한다.
//  - 이미 있는 key 에 putIfAbsent 를 호출하면 현재 구현(.tpp)대로 값을 덮어쓴다고 가정한다.
//    (이름대로 기존 값을 유지하도록 바꾼다면 ExistingKey / Differential 테스트의 기대값만 수정)
//  - 없는 key 의 getValue / getEntry 는 기본값(V(), K())을 반환하며 테이블에 추가되지 않아야 한다.
//  - 메모리 오류(범위 밖 접근, 해제 후 사용)는 AddressSanitizer(-fsanitize=address,undefined) 로 확인한다.
// =====================================================================

namespace {
    template <typename K, typename V>
    void expect_value(const LookUpTable<K, V> &table, const K &key, const V &value) {
        EXPECT_EQ(value, table.getValue(key)) << "key: " << key;
    }

    void put_all(LookUpTable<int, int> &table, const std::vector<int> &keys) {
        for (const int key : keys) {
            table.putIfAbsent(key, key * 10);
        }
    }

    void expect_all_present(const LookUpTable<int, int> &table, const std::vector<int> &keys) {
        for (const int key : keys) {
            EXPECT_EQ(key * 10, table.getValue(key)) << "key: " << key;
        }
    }

    std::vector<int> range(const int from, const int to) {
        std::vector<int> result;
        for (int value = from; value < to; ++value) {
            result.push_back(value);
        }
        return result;
    }
}

// ---------------------------------------------------------------------
// 기본 동작
// ---------------------------------------------------------------------

TEST(LookUpTableBasic, PutThenGet) {
    LookUpTable<int, int> table;
    table.putIfAbsent(5, 50);
    expect_value(table, 5, 50);
}

TEST(LookUpTableBasic, MissingKeyOnEmptyTableReturnsDefault) {
    const LookUpTable<int, int> table;
    EXPECT_EQ(0, table.getValue(5));
}

TEST(LookUpTableBasic, MultipleKeys) {
    LookUpTable<int, int> table;
    table.putIfAbsent(5, 50);
    table.putIfAbsent(3, 30);
    table.putIfAbsent(9, 90);
    table.putIfAbsent(1, 10);
    expect_value(table, 5, 50);
    expect_value(table, 3, 30);
    expect_value(table, 9, 90);
    expect_value(table, 1, 10);
}

TEST(LookUpTableBasic, MissingKeysBetweenExistingKeysReturnDefault) {
    LookUpTable<int, int> table;
    put_all(table, {10, 20, 30});
    EXPECT_EQ(0, table.getValue(5));   // 맨 앞보다 작음
    EXPECT_EQ(0, table.getValue(15));  // 사이 (이웃 값을 돌려주면 안 된다)
    EXPECT_EQ(0, table.getValue(25));
    EXPECT_EQ(0, table.getValue(35));  // 맨 뒤보다 큼
}

TEST(LookUpTableBasic, LookingUpMissingKeyDoesNotAddIt) {
    LookUpTable<int, int> table;
    put_all(table, {10, 30});
    EXPECT_EQ(0, table.getValue(20));
    EXPECT_EQ(0, table.getEntry(20).value);
    table.putIfAbsent(20, 200);
    expect_value(table, 20, 200);
    expect_all_present(table, {10, 30});
}

TEST(LookUpTableBasic, NegativeAndZeroKeys) {
    LookUpTable<int, int> table;
    put_all(table, {0, -5, 5, -100, 100});
    expect_all_present(table, {0, -5, 5, -100, 100});
}

// ---------------------------------------------------------------------
// 이미 있는 key
// ---------------------------------------------------------------------

TEST(LookUpTableExistingKey, ValueIsOverwritten) {
    LookUpTable<int, int> table;
    table.putIfAbsent(5, 50);
    table.putIfAbsent(5, 99);
    expect_value(table, 5, 99);
}

TEST(LookUpTableExistingKey, OtherKeysAreNotAffected) {
    LookUpTable<int, int> table;
    put_all(table, {1, 2, 3});
    table.putIfAbsent(2, 999);
    expect_value(table, 1, 10);
    expect_value(table, 2, 999);
    expect_value(table, 3, 30);
}

// ---------------------------------------------------------------------
// 삽입 순서, 초기 용량, resize
// ---------------------------------------------------------------------

TEST(LookUpTableGrowth, AscendingInsertBeyondInitialCapacity) {
    LookUpTable<int, int> table;
    const std::vector<int> keys = range(0, 200);
    put_all(table, keys);
    expect_all_present(table, keys);
}

TEST(LookUpTableGrowth, DescendingInsertBeyondInitialCapacity) {
    LookUpTable<int, int> table;
    std::vector<int> keys = range(0, 200);
    std::reverse(keys.begin(), keys.end());
    put_all(table, keys);
    expect_all_present(table, keys);
}

TEST(LookUpTableGrowth, ShuffledInsert) {
    LookUpTable<int, int> table;
    std::vector<int> keys = range(0, 300);
    std::mt19937 generator(20261119);
    std::shuffle(keys.begin(), keys.end(), generator);
    put_all(table, keys);
    expect_all_present(table, keys);
    EXPECT_EQ(0, table.getValue(300));
    EXPECT_EQ(0, table.getValue(-1));
}

TEST(LookUpTableGrowth, ExactlyAtCapacityBoundary) {
    for (const int count : {1, 2, 3, 4, 5, 7, 8, 9, 16, 17}) {
        LookUpTable<int, int> table;
        std::vector<int> keys = range(0, count);
        std::reverse(keys.begin(), keys.end());
        put_all(table, keys);
        expect_all_present(table, keys);
    }
}

TEST(LookUpTableGrowth, CustomInitialCapacities) {
    const std::vector<int> keys = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    for (const int initialCapacity : {0, 1, 100}) {
        LookUpTable<int, int> table(initialCapacity);
        put_all(table, keys);
        expect_all_present(table, keys);
    }
}

TEST(LookUpTableGrowth, InterleavedAndBoundaryInsertTriggersResize) {
    LookUpTable<int, int> table;
    const std::vector<int> keys = {50, 10, 90, 30, 70, 5, 95, 20, 40, 60, 80, 1, 100, 15, 25, 35, 45, 55, 65, 75};
    put_all(table, keys);
    expect_all_present(table, keys);
}

TEST(LookUpTableGrowth, ExplicitResizePreservesEntries) {
    LookUpTable<int, int> table;
    table.resize();  // 빈 테이블에서 직접 호출
    put_all(table, {10, 20, 30});
    table.resize();
    table.resize();
    expect_all_present(table, {10, 20, 30});
    table.putIfAbsent(25, 250);
    expect_all_present(table, {10, 20, 25, 30});
}

// ---------------------------------------------------------------------
// getEntry
// ---------------------------------------------------------------------

TEST(LookUpTableGetEntry, ReturnsEntryWithKeyAndValue) {
    LookUpTable<int, int> table;
    put_all(table, {1, 2, 3});
    const Entry<int, int> &entry = table.getEntry(2);
    EXPECT_EQ(2, entry.key);
    EXPECT_EQ(20, entry.value);
}

TEST(LookUpTableGetEntry, ReturnedReferenceCanModifyValue) {
    LookUpTable<int, int> table;
    put_all(table, {1, 2, 3});
    table.getEntry(2).value = 99;
    expect_value(table, 2, 99);
    expect_value(table, 1, 10);
    expect_value(table, 3, 30);
}

TEST(LookUpTableGetEntry, MissingKeyReturnsDefaultEntry) {
    LookUpTable<int, int> table;
    put_all(table, {10, 30});
    const Entry<int, int> &entry = table.getEntry(20);
    EXPECT_EQ(0, entry.key);
    EXPECT_EQ(0, entry.value);
}

// ---------------------------------------------------------------------
// 복사
// ---------------------------------------------------------------------

TEST(LookUpTableCopy, CopyConstructorCopiesAllEntriesAndIsIndependent) {
    LookUpTable<int, int> original;
    const std::vector<int> keys = range(0, 50);
    put_all(original, keys);

    LookUpTable<int, int> copy(original);
    expect_all_present(copy, keys);

    original.putIfAbsent(2, 999);
    original.putIfAbsent(100, 1000);
    copy.putIfAbsent(200, 2000);

    expect_value(copy, 2, 20);
    EXPECT_EQ(0, copy.getValue(100));
    expect_value(copy, 200, 2000);
    EXPECT_EQ(0, original.getValue(200));
    expect_value(original, 2, 999);
}

TEST(LookUpTableCopy, CopyAssignmentToSmallerAndLargerTable) {
    LookUpTable<int, int> big;
    const std::vector<int> bigKeys = range(0, 40);
    put_all(big, bigKeys);

    LookUpTable<int, int> small;
    put_all(small, {100, 200});

    // 작은 테이블 -> 큰 테이블 대입 (재할당 발생)
    small = big;
    expect_all_present(small, bigKeys);
    EXPECT_EQ(0, small.getValue(100));
    EXPECT_EQ(0, small.getValue(200));

    // 큰 테이블 -> 작은 테이블 대입 (기존 버퍼 재사용)
    LookUpTable<int, int> tiny;
    put_all(tiny, {1, 2});
    big = tiny;
    expect_all_present(big, {1, 2});
    EXPECT_EQ(0, big.getValue(3));
    EXPECT_EQ(0, big.getValue(39));
    big.putIfAbsent(3, 30);
    expect_all_present(big, {1, 2, 3});
}

TEST(LookUpTableCopy, CopyAssignmentChainAndSelfAssignment) {
    LookUpTable<int, int> source;
    put_all(source, {1, 2, 3});

    LookUpTable<int, int> first;
    LookUpTable<int, int> second;
    first = second = source;
    expect_all_present(first, {1, 2, 3});
    expect_all_present(second, {1, 2, 3});

    LookUpTable<int, int> &alias = first;
    first = alias;
    expect_all_present(first, {1, 2, 3});
}

// ---------------------------------------------------------------------
// 이동
// ---------------------------------------------------------------------

TEST(LookUpTableMove, MoveConstructorTransfersAndSourceIsReusable) {
    LookUpTable<int, int> source;
    const std::vector<int> keys = range(0, 30);
    put_all(source, keys);

    const LookUpTable<int, int> target(std::move(source));
    expect_all_present(target, keys);

    EXPECT_EQ(0, source.getValue(1));
    source.putIfAbsent(7, 70);
    expect_value(source, 7, 70);
    EXPECT_EQ(0, source.getValue(1));
}

TEST(LookUpTableMove, MoveAssignmentReplacesOldEntriesAndSourceIsReusable) {
    LookUpTable<int, int> source;
    put_all(source, {1, 2, 3});
    LookUpTable<int, int> target;
    put_all(target, {100, 200});

    target = std::move(source);
    expect_all_present(target, {1, 2, 3});
    EXPECT_EQ(0, target.getValue(100));

    EXPECT_EQ(0, source.getValue(1));
    source.putIfAbsent(9, 90);
    expect_value(source, 9, 90);
}

TEST(LookUpTableMove, SelfMoveAssignmentKeepsEntries) {
    LookUpTable<int, int> table;
    put_all(table, {1, 2, 3});
    LookUpTable<int, int> &alias = table;
    table = std::move(alias);
    expect_all_present(table, {1, 2, 3});
}

// ---------------------------------------------------------------------
// 다른 타입 (MyString / 포인터 값 - Level 4 해시 테이블 패턴)
// ---------------------------------------------------------------------

TEST(LookUpTableGeneric, MyStringKeys) {
    LookUpTable<MyString, int> table;
    table.putIfAbsent(MyString("G3"), 3);
    table.putIfAbsent(MyString("G1"), 1);
    table.putIfAbsent(MyString("G2"), 2);
    table.putIfAbsent(MyString("G5"), 5);
    table.putIfAbsent(MyString("G4"), 4);

    EXPECT_EQ(1, table.getValue(MyString("G1")));
    EXPECT_EQ(2, table.getValue(MyString("G2")));
    EXPECT_EQ(3, table.getValue(MyString("G3")));
    EXPECT_EQ(4, table.getValue(MyString("G4")));
    EXPECT_EQ(5, table.getValue(MyString("G5")));
    EXPECT_EQ(0, table.getValue(MyString("G9")));
}

TEST(LookUpTableGeneric, MyStringValues) {
    LookUpTable<int, MyString> table;
    table.putIfAbsent(2022, MyString("Harley"));
    table.putIfAbsent(1998, MyString("Jenny"));
    table.putIfAbsent(2024, MyString("Nora"));
    EXPECT_TRUE(table.getValue(1998) == "Jenny");
    EXPECT_TRUE(table.getValue(2022) == "Harley");
    EXPECT_TRUE(table.getValue(2024) == "Nora");
    EXPECT_TRUE(table.getValue(1999) == "");
}

TEST(LookUpTableGeneric, PointerValuesReturnNullptrWhenMissing) {
    int a = 1998;
    int b = 2024;
    LookUpTable<MyString, int *> table;
    table.putIfAbsent(MyString("G1"), &a);
    table.putIfAbsent(MyString("G2"), &b);

    EXPECT_EQ(&a, table.getValue(MyString("G1")));
    EXPECT_EQ(&b, table.getValue(MyString("G2")));
    EXPECT_EQ(nullptr, table.getValue(MyString("G3")));
}

TEST(LookUpTableGeneric, MyStringKeysBeyondInitialCapacity) {
    LookUpTable<MyString, int> table;
    for (int index = 0; index < 60; ++index) {
        table.putIfAbsent(MyString(("key" + std::to_string(index)).c_str()), index);
    }
    for (int index = 0; index < 60; ++index) {
        EXPECT_EQ(index, table.getValue(MyString(("key" + std::to_string(index)).c_str()))) << "index: " << index;
    }
}

// ---------------------------------------------------------------------
// std::map 과 무작위 연산 비교 (차분 테스트)
// ---------------------------------------------------------------------

TEST(LookUpTableDifferential, RandomPutsMatchStdMap) {
    std::mt19937 generator(20261119);
    LookUpTable<int, int> table;
    std::map<int, int> expected;

    for (int round = 0; round < 3000; ++round) {
        const int key = static_cast<int>(generator() % 120) - 20;
        const int value = static_cast<int>(generator() % 1000) + 1;
        table.putIfAbsent(key, value);
        expected[key] = value;

        if (round % 50 == 0) {
            for (int probe = -25; probe < 105; ++probe) {
                const auto iterator = expected.find(probe);
                const int expectedValue = iterator == expected.end() ? 0 : iterator->second;
                ASSERT_EQ(expectedValue, table.getValue(probe)) << "round: " << round << ", key: " << probe;
            }
        }
    }
}