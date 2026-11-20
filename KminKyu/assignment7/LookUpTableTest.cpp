#include <gtest/gtest.h>
#include <algorithm>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "LookUpTable.h"
#include "MyString.h"

// =====================================================================
// LookUpTable 테스트
//
//  - key 순서로 정렬된 Entry<K, V> 배열 + 이진 탐색으로 동작하는 테이블을 검증한다.
//  - Entry<K, V> 는 힙에 할당된 V* 소유권을 가지며, 소멸/이동 대입 시 delete 한다.
//  - insert(key, V*) 는 중복되지 않은 새 key 를 정렬 위치에 삽입한다.
//  - 없는 key 의 getValue / findEntry 는 nullptr 을 반환하며 테이블에 추가되지 않아야 한다.
//  - removeByKey(key) 는 테이블에 존재하는 key 를 삭제한다.
//  - Entry 가 복사 금지이므로 LookUpTable 은 이동 생성/대입만 사용한다.
//  - 메모리 오류(범위 밖 접근, 해제 후 사용, 누수)는 AddressSanitizer 로 확인한다.
// =====================================================================

namespace {
    struct PrintableValue {
        MyString text;

        MyString toString() const {
            return text;
        }
    };

    std::string to_std_string(const MyString &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    template <typename K, typename V>
    void expect_value(const LookUpTable<K, V> &table, const K &key, const V &expected) {
        const V *actual = table.getValue(key);
        ASSERT_NE(nullptr, actual) << "key: " << key;
        EXPECT_EQ(expected, *actual) << "key: " << key;
    }

    template <typename K, typename V>
    void expect_missing(const LookUpTable<K, V> &table, const K &key) {
        EXPECT_EQ(nullptr, table.getValue(key)) << "key: " << key;
        EXPECT_EQ(nullptr, table.findEntry(key)) << "key: " << key;
    }

    void insert_all(LookUpTable<int, int> &table, const std::vector<int> &keys) {
        for (const int key : keys) {
            table.insert(key, new int(key * 10));
        }
    }

    void expect_all_present(const LookUpTable<int, int> &table, const std::vector<int> &keys) {
        for (const int key : keys) {
            const int *actual = table.getValue(key);
            ASSERT_NE(nullptr, actual) << "key: " << key;
            EXPECT_EQ(key * 10, *actual) << "key: " << key;
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

TEST(LookUpTableBasic, DefaultConstructedIsEmpty) {
    LookUpTable<int, int> table;
    EXPECT_EQ(0, table.getSize());
    EXPECT_EQ(4, table.getCapacity());
    EXPECT_EQ(nullptr, table.getValue(5));
    EXPECT_EQ(nullptr, table.findEntry(5));
    EXPECT_EQ(nullptr, table.at(0));
    EXPECT_FALSE(table.isEntry(5));
}

TEST(LookUpTableBasic, InsertThenGet) {
    LookUpTable<int, int> table;
    table.insert(5, new int(50));
    EXPECT_EQ(1, table.getSize());
    EXPECT_TRUE(table.isEntry(5));
    expect_value(table, 5, 50);
}

TEST(LookUpTableBasic, MultipleKeysAreStoredInSortedOrder) {
    LookUpTable<int, int> table;
    table.insert(5, new int(50));
    table.insert(3, new int(30));
    table.insert(9, new int(90));
    table.insert(1, new int(10));

    EXPECT_EQ(4, table.getSize());
    expect_value(table, 1, 10);
    expect_value(table, 3, 30);
    expect_value(table, 5, 50);
    expect_value(table, 9, 90);

    const int expectedKeys[] = {1, 3, 5, 9};
    for (int index = 0; index < 4; ++index) {
        const Entry<int, int> *entry = table.at(index);
        ASSERT_NE(nullptr, entry);
        EXPECT_EQ(expectedKeys[index], entry->getKey());
        ASSERT_NE(nullptr, entry->getValue());
        EXPECT_EQ(expectedKeys[index] * 10, *entry->getValue());
    }
}

TEST(LookUpTableBasic, MissingKeysBetweenExistingKeysReturnNullptr) {
    LookUpTable<int, int> table;
    insert_all(table, {10, 20, 30});
    expect_missing(table, 5);   // 맨 앞보다 작음
    expect_missing(table, 15);  // 사이 (이웃 값을 돌려주면 안 된다)
    expect_missing(table, 25);
    expect_missing(table, 35);  // 맨 뒤보다 큼
    EXPECT_FALSE(table.isEntry(15));
}

TEST(LookUpTableBasic, LookingUpMissingKeyDoesNotAddIt) {
    LookUpTable<int, int> table;
    insert_all(table, {10, 30});
    EXPECT_EQ(nullptr, table.getValue(20));
    EXPECT_EQ(nullptr, table.findEntry(20));
    EXPECT_FALSE(table.isEntry(20));
    EXPECT_EQ(2, table.getSize());

    table.insert(20, new int(200));
    EXPECT_EQ(3, table.getSize());
    expect_value(table, 20, 200);
    expect_all_present(table, {10, 30});
}

TEST(LookUpTableBasic, NegativeAndZeroKeys) {
    LookUpTable<int, int> table;
    insert_all(table, {0, -5, 5, -100, 100});
    EXPECT_EQ(5, table.getSize());
    expect_all_present(table, {0, -5, 5, -100, 100});

    const int expectedOrder[] = {-100, -5, 0, 5, 100};
    for (int index = 0; index < 5; ++index) {
        ASSERT_NE(nullptr, table.at(index));
        EXPECT_EQ(expectedOrder[index], table.at(index)->getKey());
    }
}

TEST(LookUpTableBasic, AtOutOfRangeReturnsNullptr) {
    LookUpTable<int, int> table;
    insert_all(table, {10, 20});
    EXPECT_EQ(nullptr, table.at(-1));
    EXPECT_EQ(nullptr, table.at(2));
    EXPECT_EQ(nullptr, table.at(100));
}

// ---------------------------------------------------------------------
// findEntry
// ---------------------------------------------------------------------

TEST(LookUpTableFindEntry, ReturnsEntryWithKeyAndValue) {
    LookUpTable<int, int> table;
    insert_all(table, {1, 2, 3});

    Entry<int, int> *entry = table.findEntry(2);
    ASSERT_NE(nullptr, entry);
    EXPECT_EQ(2, entry->getKey());
    ASSERT_NE(nullptr, entry->getValue());
    EXPECT_EQ(20, *entry->getValue());

    const LookUpTable<int, int> &constTable = table;
    const Entry<int, int> *constEntry = constTable.findEntry(2);
    ASSERT_NE(nullptr, constEntry);
    EXPECT_EQ(2, constEntry->getKey());
    ASSERT_NE(nullptr, constEntry->getValue());
    EXPECT_EQ(20, *constEntry->getValue());
}

TEST(LookUpTableFindEntry, ReturnedPointerValueCanBeModified) {
    LookUpTable<int, int> table;
    insert_all(table, {1, 2, 3});
    Entry<int, int> *entry = table.findEntry(2);
    ASSERT_NE(nullptr, entry);
    ASSERT_NE(nullptr, entry->getValue());
    *entry->getValue() = 99;

    expect_value(table, 2, 99);
    expect_value(table, 1, 10);
    expect_value(table, 3, 30);
}

TEST(LookUpTableFindEntry, MissingKeyReturnsNullptr) {
    LookUpTable<int, int> table;
    insert_all(table, {10, 30});
    EXPECT_EQ(nullptr, table.findEntry(20));

    const LookUpTable<int, int> &constTable = table;
    EXPECT_EQ(nullptr, constTable.findEntry(20));
}

// ---------------------------------------------------------------------
// removeByKey
// ---------------------------------------------------------------------

TEST(LookUpTableRemove, RemoveFirstMiddleLastAndSingle) {
    LookUpTable<int, int> table;
    insert_all(table, {10, 20, 30, 40});

    // 중간 삭제
    table.removeByKey(20);
    EXPECT_EQ(3, table.getSize());
    expect_missing(table, 20);
    expect_all_present(table, {10, 30, 40});

    // 맨 앞 삭제
    table.removeByKey(10);
    EXPECT_EQ(2, table.getSize());
    expect_missing(table, 10);
    expect_all_present(table, {30, 40});

    // 맨 뒤 삭제
    table.removeByKey(40);
    EXPECT_EQ(1, table.getSize());
    expect_missing(table, 40);
    expect_all_present(table, {30});

    // 마지막 1개 삭제
    table.removeByKey(30);
    EXPECT_EQ(0, table.getSize());
    expect_missing(table, 30);
    EXPECT_EQ(nullptr, table.at(0));
}

TEST(LookUpTableRemove, RemoveMissingKeyDoesNotCorruptTable) {
    LookUpTable<int, int> table;
    table.removeByKey(10);  // 빈 테이블에서 삭제 시도
    EXPECT_EQ(0, table.getSize());

    insert_all(table, {10, 20, 30});
    table.removeByKey(5);   // 맨 앞보다 작음
    table.removeByKey(15);  // 사이
    table.removeByKey(35);  // 맨 뒤보다 큼
    EXPECT_EQ(3, table.getSize());
    expect_all_present(table, {10, 20, 30});
}

TEST(LookUpTableRemove, RemoveAndReinsertSameKey) {
    LookUpTable<int, int> table;
    insert_all(table, {1, 2, 3});

    table.removeByKey(2);
    EXPECT_FALSE(table.isEntry(2));
    EXPECT_EQ(2, table.getSize());

    table.insert(2, new int(999));
    EXPECT_TRUE(table.isEntry(2));
    EXPECT_EQ(3, table.getSize());
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
    insert_all(table, keys);
    EXPECT_EQ(200, table.getSize());
    expect_all_present(table, keys);
}

TEST(LookUpTableGrowth, DescendingInsertBeyondInitialCapacity) {
    LookUpTable<int, int> table;
    std::vector<int> keys = range(0, 200);
    std::reverse(keys.begin(), keys.end());
    insert_all(table, keys);
    EXPECT_EQ(200, table.getSize());
    expect_all_present(table, keys);
}

TEST(LookUpTableGrowth, ShuffledInsert) {
    LookUpTable<int, int> table;
    std::vector<int> keys = range(0, 300);
    std::mt19937 generator(20261119);
    std::shuffle(keys.begin(), keys.end(), generator);
    insert_all(table, keys);
    EXPECT_EQ(300, table.getSize());
    expect_all_present(table, keys);
    expect_missing(table, 300);
    expect_missing(table, -1);
}

TEST(LookUpTableGrowth, ExactlyAtCapacityBoundary) {
    for (const int count : {1, 2, 3, 4, 5, 7, 8, 9, 16, 17}) {
        LookUpTable<int, int> table;
        std::vector<int> keys = range(0, count);
        std::reverse(keys.begin(), keys.end());
        insert_all(table, keys);
        EXPECT_EQ(count, table.getSize());
        expect_all_present(table, keys);
    }
}

TEST(LookUpTableGrowth, CustomInitialCapacities) {
    const std::vector<int> keys = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    for (const int initialCapacity : {-5, 0, 1, 4, 100}) {
        LookUpTable<int, int> table(initialCapacity);
        EXPECT_GT(table.getCapacity(), 0);
        insert_all(table, keys);
        EXPECT_EQ(static_cast<int>(keys.size()), table.getSize());
        expect_all_present(table, keys);
    }
}

TEST(LookUpTableGrowth, InterleavedAndBoundaryInsertTriggersResize) {
    LookUpTable<int, int> table;
    const std::vector<int> keys = {50, 10, 90, 30, 70, 5, 95, 20, 40, 60, 80, 1, 100, 15, 25, 35, 45, 55, 65, 75};
    insert_all(table, keys);
    EXPECT_EQ(static_cast<int>(keys.size()), table.getSize());
    expect_all_present(table, keys);
}

TEST(LookUpTableGrowth, ExplicitResizePreservesEntries) {
    LookUpTable<int, int> table;
    table.resize();  // 빈 테이블에서 직접 호출
    EXPECT_EQ(8, table.getCapacity());
    insert_all(table, {10, 20, 30});
    table.resize();
    table.resize();
    EXPECT_EQ(32, table.getCapacity());
    expect_all_present(table, {10, 20, 30});
    table.insert(25, new int(250));
    expect_all_present(table, {10, 20, 25, 30});
}

// ---------------------------------------------------------------------
// 이동
// ---------------------------------------------------------------------

TEST(LookUpTableMove, MoveConstructorTransfersAndSourceIsReusable) {
    LookUpTable<int, int> source;
    const std::vector<int> keys = range(0, 30);
    insert_all(source, keys);

    const LookUpTable<int, int> target(std::move(source));
    EXPECT_EQ(30, target.getSize());
    expect_all_present(target, keys);

    EXPECT_EQ(0, source.getSize());
    expect_missing(source, 1);
    source.insert(7, new int(70));
    EXPECT_EQ(1, source.getSize());
    expect_value(source, 7, 70);
    expect_missing(source, 1);
}

TEST(LookUpTableMove, MoveAssignmentReplacesOldEntriesAndSourceIsReusable) {
    LookUpTable<int, int> source;
    insert_all(source, {1, 2, 3});
    LookUpTable<int, int> target;
    insert_all(target, {100, 200});

    target = std::move(source);
    EXPECT_EQ(3, target.getSize());
    expect_all_present(target, {1, 2, 3});
    expect_missing(target, 100);

    EXPECT_EQ(0, source.getSize());
    expect_missing(source, 1);
    source.insert(9, new int(90));
    EXPECT_EQ(1, source.getSize());
    expect_value(source, 9, 90);
}

TEST(LookUpTableMove, SelfMoveAssignmentKeepsEntries) {
    LookUpTable<int, int> table;
    insert_all(table, {1, 2, 3});
    LookUpTable<int, int> &alias = table;
    table = std::move(alias);
    EXPECT_EQ(3, table.getSize());
    expect_all_present(table, {1, 2, 3});
}

// ---------------------------------------------------------------------
// 다른 타입 (MyString / toString / 스트림 출력)
// ---------------------------------------------------------------------

TEST(LookUpTableGeneric, MyStringKeys) {
    LookUpTable<MyString, int> table;
    table.insert(MyString("G3"), new int(3));
    table.insert(MyString("G1"), new int(1));
    table.insert(MyString("G2"), new int(2));
    table.insert(MyString("G5"), new int(5));
    table.insert(MyString("G4"), new int(4));

    expect_value(table, MyString("G1"), 1);
    expect_value(table, MyString("G2"), 2);
    expect_value(table, MyString("G3"), 3);
    expect_value(table, MyString("G4"), 4);
    expect_value(table, MyString("G5"), 5);
    expect_missing(table, MyString("G9"));
}

TEST(LookUpTableGeneric, MyStringValues) {
    LookUpTable<int, MyString> table;
    table.insert(2022, new MyString("Harley"));
    table.insert(1998, new MyString("Jenny"));
    table.insert(2024, new MyString("Nora"));

    ASSERT_NE(nullptr, table.getValue(1998));
    ASSERT_NE(nullptr, table.getValue(2022));
    ASSERT_NE(nullptr, table.getValue(2024));
    EXPECT_TRUE(*table.getValue(1998) == "Jenny");
    EXPECT_TRUE(*table.getValue(2022) == "Harley");
    EXPECT_TRUE(*table.getValue(2024) == "Nora");
    EXPECT_EQ(nullptr, table.getValue(1999));
}

TEST(LookUpTableGeneric, MyStringKeysBeyondInitialCapacity) {
    LookUpTable<MyString, int> table;
    for (int index = 0; index < 60; ++index) {
        table.insert(MyString(("key" + std::to_string(index)).c_str()), new int(index));
    }
    EXPECT_EQ(60, table.getSize());
    for (int index = 0; index < 60; ++index) {
        const MyString key(("key" + std::to_string(index)).c_str());
        const int *actual = table.getValue(key);
        ASSERT_NE(nullptr, actual) << "index: " << index;
        EXPECT_EQ(index, *actual) << "index: " << index;
    }
}

TEST(LookUpTableGeneric, ToStringConcatenatesValuesInSortedKeyOrder) {
    LookUpTable<int, PrintableValue> table;
    table.insert(30, new PrintableValue{MyString("C\n")});
    table.insert(10, new PrintableValue{MyString("A\n")});
    table.insert(20, new PrintableValue{MyString("B\n")});

    EXPECT_EQ("A\nB\nC\n", to_std_string(table.toString()));
}

TEST(LookUpTableGeneric, StreamOutputPrintsValuesInSortedKeyOrder) {
    LookUpTable<int, MyString> table;
    table.insert(3, new MyString("three "));
    table.insert(1, new MyString("one "));
    table.insert(2, new MyString("two "));

    std::ostringstream oss;
    oss << table;
    EXPECT_EQ("one two three ", oss.str());
}

// ---------------------------------------------------------------------
// std::map 과 무작위 연산 비교 (차분 테스트)
// ---------------------------------------------------------------------

TEST(LookUpTableDifferential, RandomInsertAndRemoveMatchStdMap) {
    std::mt19937 generator(20261119);
    LookUpTable<int, int> table;
    std::map<int, int> expected;

    for (int round = 0; round < 3000; ++round) {
        const int key = static_cast<int>(generator() % 120) - 20;
        const auto iterator = expected.find(key);

        if (iterator == expected.end()) {
            const int value = static_cast<int>(generator() % 1000) + 1;
            table.insert(key, new int(value));
            expected[key] = value;
        } else if (generator() % 2 == 0) {
            table.removeByKey(key);
            expected.erase(iterator);
        } else {
            const int value = static_cast<int>(generator() % 1000) + 1;
            int *existing = table.getValue(key);
            ASSERT_NE(nullptr, existing) << "round: " << round << ", key: " << key;
            *existing = value;
            iterator->second = value;
        }

        if (round % 50 == 0) {
            ASSERT_EQ(static_cast<int>(expected.size()), table.getSize()) << "round: " << round;

            int index = 0;
            for (const auto &[k, v] : expected) {
                const Entry<int, int> *entry = table.at(index++);
                ASSERT_NE(nullptr, entry);
                EXPECT_EQ(k, entry->getKey());
                ASSERT_NE(nullptr, entry->getValue());
                EXPECT_EQ(v, *entry->getValue());
            }

            for (int probe = -25; probe < 105; ++probe) {
                const auto found = expected.find(probe);
                const int *actual = table.getValue(probe);
                if (found == expected.end()) {
                    ASSERT_EQ(nullptr, actual) << "round: " << round << ", key: " << probe;
                } else {
                    ASSERT_NE(nullptr, actual) << "round: " << round << ", key: " << probe;
                    ASSERT_EQ(found->second, *actual) << "round: " << round << ", key: " << probe;
                }
            }
        }
    }
}
