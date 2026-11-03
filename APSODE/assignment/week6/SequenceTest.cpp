#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

#include "Sequence.h"

namespace {
    // Sequence의 생성자와 수정 함수들은 protected라서 테스트용 자식 클래스로 열어준다.
    template<typename T, int TAIL = 1>
    class TestSequence : public Sequence<T, TAIL> {
        using Base = Sequence<T, TAIL>;

    public:
        TestSequence() : Base() {}

        TestSequence(const T *elements, int size) : Base(elements, size) {}

        TestSequence(const TestSequence &other) : Base(other) {}

        TestSequence& operator=(const TestSequence &other) {
            Base::operator=(other);
            return *this;
        }

        using Base::insert;
        using Base::append;
        using Base::erase;
        using Base::clear;
    };

    using IntSequence = TestSequence<int>;

    // 0, 1, 2, ..., count - 1 을 순서대로 append 한 시퀀스
    IntSequence make_range(int count) {
        IntSequence sequence;
        for (int value = 0; value < count; ++value) {
            sequence.append(value);
        }
        return sequence;
    }

    // 시퀀스의 [0, size) 내용을 기대값 배열과 비교한다
    template<typename T, int TAIL>
    void expect_elements(const Sequence<T, TAIL> &sequence, std::initializer_list<T> expected) {
        ASSERT_EQ(static_cast<int>(expected.size()), sequence.size());
        int index = 0;
        for (const T &value : expected) {
            EXPECT_EQ(value, sequence[index]) << "index " << index;
            ++index;
        }
    }

    // size 바로 뒤의 TAIL 칸이 모두 T()로 채워져 있는지 확인한다
    template<typename T, int TAIL>
    void expect_tail_cleared(const Sequence<T, TAIL> &sequence) {
        for (int offset = 0; offset < TAIL; ++offset) {
            EXPECT_EQ(T(), sequence[sequence.size() + offset]) << "tail offset " << offset;
        }
    }
}

// =====================================================================
// 생성자
// =====================================================================

TEST(SequenceConstructor, DefaultIsEmptyWithDefaultCapacity) {
    IntSequence sequence;

    EXPECT_EQ(0, sequence.size());
    EXPECT_TRUE(sequence.empty());
    EXPECT_EQ(8, sequence.capacity());
}

TEST(SequenceConstructor, DefaultHasClearedTail) {
    IntSequence sequence;

    expect_tail_cleared(sequence);
}

TEST(SequenceConstructor, FromArrayCopiesElements) {
    const int values[] = {10, 20, 30};
    IntSequence sequence(values, 3);

    expect_elements(sequence, {10, 20, 30});
    expect_tail_cleared(sequence);
}

TEST(SequenceConstructor, FromArrayRoundsCapacityToMultipleOfDefault) {
    int values[20] = {};
    IntSequence small(values, 3);
    IntSequence large(values, 20);

    EXPECT_EQ(8, small.capacity());
    EXPECT_EQ(24, large.capacity());
}

TEST(SequenceConstructor, CopyIsDeep) {
    IntSequence original = make_range(3);
    IntSequence copy(original);

    copy[0] = 100;

    EXPECT_EQ(0, original[0]);
    EXPECT_EQ(100, copy[0]);
    EXPECT_EQ(original.size(), copy.size());
}

TEST(SequenceConstructor, CopyOfGrownSequenceCanKeepAppending) {
    IntSequence original = make_range(10);   // capacity 16
    original.erase(8, -1);                   // size 8, capacity 16 (shrink 조건은 size <= 4)
    ASSERT_EQ(16, original.capacity());
    IntSequence copy(original);

    // 복사본의 capacity와 실제 버퍼 크기가 맞지 않으면 여기서 버퍼 밖에 쓰게 된다
    for (int value = 8; value < 15; ++value) {
        copy.append(value);
    }

    ASSERT_EQ(15, copy.size());
    for (int index = 0; index < 15; ++index) {
        EXPECT_EQ(index, copy[index]);
    }
    expect_tail_cleared(copy);
}

// =====================================================================
// 대입 연산자
// =====================================================================

TEST(SequenceAssignment, CopiesContent) {
    IntSequence source = make_range(5);
    IntSequence target = make_range(2);

    target = source;

    expect_elements(target, {0, 1, 2, 3, 4});
}

TEST(SequenceAssignment, IsDeep) {
    IntSequence source = make_range(3);
    IntSequence target;

    target = source;
    target[0] = 100;

    EXPECT_EQ(0, source[0]);
}

TEST(SequenceAssignment, SelfAssignmentKeepsContent) {
    IntSequence sequence = make_range(3);
    IntSequence &alias = sequence;

    sequence = alias;

    expect_elements(sequence, {0, 1, 2});
}

// =====================================================================
// 원소 접근
// =====================================================================

TEST(SequenceAccess, AtReturnsElement) {
    IntSequence sequence = make_range(3);

    EXPECT_EQ(2, sequence.at(2));
}

TEST(SequenceAccess, AtIsWritable) {
    IntSequence sequence = make_range(3);

    sequence.at(1) = 42;

    EXPECT_EQ(42, sequence[1]);
}

TEST(SequenceAccess, AtThrowsOutOfRange) {
    IntSequence sequence = make_range(3);
    const IntSequence &constant = sequence;

    EXPECT_THROW(sequence.at(-1), std::out_of_range);
    EXPECT_THROW(sequence.at(3), std::out_of_range);
    EXPECT_THROW(constant.at(3), std::out_of_range);
}

TEST(SequenceAccess, AtOnEmptyThrows) {
    IntSequence sequence;

    EXPECT_THROW(sequence.at(0), std::out_of_range);
}

// =====================================================================
// append
// =====================================================================

TEST(SequenceAppend, SingleElement) {
    IntSequence sequence;

    sequence.append(7);

    expect_elements(sequence, {7});
    expect_tail_cleared(sequence);
}

TEST(SequenceAppend, MultipleElements) {
    IntSequence sequence = make_range(2);
    const int values[] = {5, 6, 7};

    sequence.append(values, 3);

    expect_elements(sequence, {0, 1, 5, 6, 7});
    expect_tail_cleared(sequence);
}

TEST(SequenceAppend, ZeroAmountChangesNothing) {
    IntSequence sequence = make_range(2);
    const int values[] = {9};

    sequence.append(values, 0);

    expect_elements(sequence, {0, 1});
}

TEST(SequenceAppend, GrowsCapacityByGrowthFactor) {
    IntSequence sequence = make_range(7);
    EXPECT_EQ(8, sequence.capacity());

    sequence.append(7);

    EXPECT_EQ(16, sequence.capacity());
    for (int index = 0; index < 8; ++index) {
        EXPECT_EQ(index, sequence[index]);
    }
}

TEST(SequenceAppend, GrowsToRequiredWhenGrowthIsNotEnough) {
    IntSequence sequence;
    int values[100];
    for (int index = 0; index < 100; ++index) {
        values[index] = index;
    }

    sequence.append(values, 100);

    ASSERT_EQ(100, sequence.size());
    EXPECT_GE(sequence.capacity(), 100);
    EXPECT_EQ(0, sequence.capacity() % 8);
    for (int index = 0; index < 100; ++index) {
        EXPECT_EQ(index, sequence[index]);
    }
    expect_tail_cleared(sequence);
}

TEST(SequenceAppend, ManySingleAppendsKeepOrder) {
    IntSequence sequence = make_range(1000);

    ASSERT_EQ(1000, sequence.size());
    for (int index = 0; index < 1000; ++index) {
        ASSERT_EQ(index, sequence[index]);
    }
    expect_tail_cleared(sequence);
}

TEST(SequenceAppend, NegativeAmountThrows) {
    IntSequence sequence;
    const int values[] = {1};

    EXPECT_THROW(sequence.append(values, -1), std::invalid_argument);
}

TEST(SequenceAppend, NullElementsThrows) {
    IntSequence sequence;

    EXPECT_THROW(sequence.append(nullptr, 1), std::invalid_argument);
}

// =====================================================================
// insert
// =====================================================================

TEST(SequenceInsert, AtFront) {
    IntSequence sequence = make_range(3);

    sequence.insert(0, 100);

    expect_elements(sequence, {100, 0, 1, 2});
    expect_tail_cleared(sequence);
}

TEST(SequenceInsert, InMiddle) {
    IntSequence sequence = make_range(4);

    sequence.insert(2, 100);

    expect_elements(sequence, {0, 1, 100, 2, 3});
}

TEST(SequenceInsert, MultipleInMiddle) {
    IntSequence sequence = make_range(4);
    const int values[] = {7, 8, 9};

    sequence.insert(1, values, 3);

    expect_elements(sequence, {0, 7, 8, 9, 1, 2, 3});
    expect_tail_cleared(sequence);
}

TEST(SequenceInsert, AtEndBehavesLikeAppend) {
    IntSequence sequence = make_range(3);

    sequence.insert(3, 100);

    expect_elements(sequence, {0, 1, 2, 100});
}

TEST(SequenceInsert, IntoEmpty) {
    IntSequence sequence;

    sequence.insert(0, 100);

    expect_elements(sequence, {100});
}

TEST(SequenceInsert, GrowsCapacity) {
    IntSequence sequence = make_range(7);
    const int values[] = {100, 101, 102};

    sequence.insert(3, values, 3);

    EXPECT_GE(sequence.capacity(), 10);
    expect_elements(sequence, {0, 1, 2, 100, 101, 102, 3, 4, 5, 6});
    expect_tail_cleared(sequence);
}

TEST(SequenceInsert, InvalidIndexThrows) {
    IntSequence sequence = make_range(3);

    EXPECT_THROW(sequence.insert(-1, 100), std::out_of_range);
    EXPECT_THROW(sequence.insert(4, 100), std::out_of_range);
}

TEST(SequenceInsert, NullElementsThrows) {
    IntSequence sequence = make_range(3);

    EXPECT_THROW(sequence.insert(0, nullptr, 1), std::invalid_argument);
}

// =====================================================================
// erase
// =====================================================================

TEST(SequenceErase, SingleDefaultAmount) {
    IntSequence sequence = make_range(5);

    sequence.erase(1);

    expect_elements(sequence, {0, 2, 3, 4});
    expect_tail_cleared(sequence);
}

TEST(SequenceErase, Range) {
    IntSequence sequence = make_range(6);

    sequence.erase(2, 3);

    expect_elements(sequence, {0, 1, 5});
    expect_tail_cleared(sequence);
}

TEST(SequenceErase, AmountPastEndIsClamped) {
    IntSequence sequence = make_range(5);

    sequence.erase(3, 100);

    expect_elements(sequence, {0, 1, 2});
}

TEST(SequenceErase, NegativeAmountErasesToEnd) {
    IntSequence sequence = make_range(5);

    sequence.erase(2, -1);

    expect_elements(sequence, {0, 1});
}

TEST(SequenceErase, NegativeStartIsTreatedAsZero) {
    IntSequence sequence = make_range(5);

    sequence.erase(-3, 2);

    expect_elements(sequence, {2, 3, 4});
}

TEST(SequenceErase, StartPastEndChangesNothing) {
    IntSequence sequence = make_range(3);

    sequence.erase(10, 2);

    expect_elements(sequence, {0, 1, 2});
}

TEST(SequenceErase, ShrinksWhenMostlyEmpty) {
    IntSequence sequence = make_range(40);
    const int grown = sequence.capacity();
    ASSERT_GT(grown, 8);

    sequence.erase(4, -1);

    EXPECT_LT(sequence.capacity(), grown);
    EXPECT_GE(sequence.capacity(), 8);
    expect_elements(sequence, {0, 1, 2, 3});
    expect_tail_cleared(sequence);
}

TEST(SequenceErase, DoesNotShrinkBelowDefaultCapacity) {
    IntSequence sequence = make_range(5);

    sequence.erase(0, -1);

    EXPECT_EQ(8, sequence.capacity());
    EXPECT_TRUE(sequence.empty());
}

TEST(SequenceErase, CanAppendAfterShrink) {
    IntSequence sequence = make_range(40);
    sequence.erase(2, -1);

    for (int value = 2; value < 30; ++value) {
        sequence.append(value);
    }

    ASSERT_EQ(30, sequence.size());
    for (int index = 0; index < 30; ++index) {
        EXPECT_EQ(index, sequence[index]);
    }
}

// =====================================================================
// clear
// =====================================================================

TEST(SequenceClear, ResetsToInitialState) {
    IntSequence sequence = make_range(40);

    sequence.clear();

    EXPECT_TRUE(sequence.empty());
    EXPECT_EQ(8, sequence.capacity());
    expect_tail_cleared(sequence);
}

TEST(SequenceClear, CanBeReusedAfterClear) {
    IntSequence sequence = make_range(40);
    sequence.clear();

    sequence.append(1);
    sequence.append(2);

    expect_elements(sequence, {1, 2});
}

// =====================================================================
// TAIL / 다른 타입
// =====================================================================

TEST(SequenceTail, CharSequenceIsNullTerminated) {
    TestSequence<char> sequence("abc", 3);

    EXPECT_EQ('\0', sequence[3]);
    EXPECT_STREQ("abc", &sequence[0]);

    sequence.append('d');
    EXPECT_STREQ("abcd", &sequence[0]);

    sequence.erase(0, 2);
    EXPECT_STREQ("cd", &sequence[0]);
}

TEST(SequenceTail, LargerTailIsCleared) {
    TestSequence<int, 3> sequence;
    for (int value = 1; value <= 20; ++value) {
        sequence.append(value);
        expect_tail_cleared(sequence);
    }

    sequence.erase(5, -1);
    expect_tail_cleared(sequence);
}

TEST(SequenceTail, ZeroTailWorks) {
    TestSequence<int, 0> sequence;
    for (int value = 0; value < 20; ++value) {
        sequence.append(value);
    }

    ASSERT_EQ(20, sequence.size());
    EXPECT_EQ(19, sequence[19]);
}

TEST(SequenceType, WorksWithStdString) {
    TestSequence<std::string> sequence;

    sequence.append(std::string("hello"));
    sequence.append(std::string("world"));
    sequence.insert(1, std::string("big"));

    expect_elements(sequence, {std::string("hello"), std::string("big"), std::string("world")});
}
