#include <gtest/gtest.h>

#include "SinglyLinkedList.h"

namespace {
    class TrackedValue {
        int m_value;

        public:
            static int alive_count;

            TrackedValue() : m_value(0) {
                ++alive_count;
            }

            TrackedValue(int value) : m_value(value) {
                ++alive_count;
            }

            TrackedValue(const TrackedValue &other) : m_value(other.m_value) {
                ++alive_count;
            }

            TrackedValue& operator=(const TrackedValue &other) = default;

            ~TrackedValue() {
                --alive_count;
            }

            bool operator==(const TrackedValue &other) const {
                return this->m_value == other.m_value;
            }

            [[nodiscard]] int value() const {
                return this->m_value;
            }
    };

    int TrackedValue::alive_count = 0;

    template<int LENGTH>
    void expect_elements(const SinglyLinkedList<int> &list, const int (&expected_elements)[LENGTH]) {
        ASSERT_EQ(list.size(), LENGTH);

        for (int element_index = 0; element_index < LENGTH; ++element_index) {
            EXPECT_EQ(list.get(element_index), expected_elements[element_index]) << "index: " << element_index;
        }
    }
}


TEST(SinglyLinkedListTest, DefaultConstructorCreatesEmptyList) {
    SinglyLinkedList<int> list;

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(SinglyLinkedListTest, ArrayConstructorKeepsOrder) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);

    expect_elements(list, {1, 2, 3});
}

TEST(SinglyLinkedListTest, ArrayConstructorAcceptsZeroLengthWithNullptr) {
    SinglyLinkedList<int> list(nullptr, 0);

    EXPECT_TRUE(list.empty());
}

TEST(SinglyLinkedListTest, ArrayConstructorRejectsInvalidArguments) {
    int elements[] = {1};

    EXPECT_THROW(SinglyLinkedList<int>(elements, -1), std::invalid_argument);
    EXPECT_THROW(SinglyLinkedList<int>(nullptr, 1), std::invalid_argument);
}

TEST(SinglyLinkedListTest, AddAppendsToEnd) {
    SinglyLinkedList<int> list;

    list.add(1);
    list.add(2);
    list.add(3);

    expect_elements(list, {1, 2, 3});
}

TEST(SinglyLinkedListTest, AddAllAppendsAfterExistingElements) {
    SinglyLinkedList<int> list;
    int elements[] = {2, 3, 4};

    list.add(1);
    list.add_all(elements, 3);

    expect_elements(list, {1, 2, 3, 4});
}

TEST(SinglyLinkedListTest, AddFirstPrependsElement) {
    SinglyLinkedList<int> list;

    list.add_first(3);
    list.add_first(2);
    list.add_first(1);

    expect_elements(list, {1, 2, 3});
}

TEST(SinglyLinkedListTest, AddLastAfterAddFirstOnEmptyList) {
    SinglyLinkedList<int> list;

    list.add_first(1);
    list.add_last(2);

    expect_elements(list, {1, 2});
}

TEST(SinglyLinkedListTest, InsertAtFrontMiddleAndEnd) {
    SinglyLinkedList<int> list;

    list.insert(0, 2);
    list.insert(0, 0);
    list.insert(1, 1);
    list.insert(3, 3);

    expect_elements(list, {0, 1, 2, 3});
}

TEST(SinglyLinkedListTest, InsertOutOfRangeThrows) {
    SinglyLinkedList<int> list;
    list.add(1);

    EXPECT_THROW(list.insert(-1, 0), std::out_of_range);
    EXPECT_THROW(list.insert(2, 0), std::out_of_range);
}

TEST(SinglyLinkedListTest, GetPeekAndSetAccessElement) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);

    list.set(1, 20);

    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.peek(1), 20);
}

TEST(SinglyLinkedListTest, OperatorBracketReturnsReference) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);

    list[2] = 30;
    const SinglyLinkedList<int> &const_list = list;

    EXPECT_EQ(list.get(2), 30);
    EXPECT_EQ(const_list[2], 30);
}

TEST(SinglyLinkedListTest, InvalidIndexThrows) {
    int elements[] = {1, 2};
    SinglyLinkedList<int> list(elements, 2);
    SinglyLinkedList<int> empty_list;

    EXPECT_THROW(list.get(-1), std::out_of_range);
    EXPECT_THROW(list.get(2), std::out_of_range);
    EXPECT_THROW(list.peek(2), std::out_of_range);
    EXPECT_THROW(list.set(2, 0), std::out_of_range);
    EXPECT_THROW(list[2], std::out_of_range);
    EXPECT_THROW(list.remove(2), std::out_of_range);
    EXPECT_THROW(empty_list.get(0), std::out_of_range);
    EXPECT_THROW(empty_list.remove(0), std::out_of_range);
}

TEST(SinglyLinkedListTest, RemoveFirstMiddleAndLast) {
    int elements[] = {1, 2, 3, 4, 5};
    SinglyLinkedList<int> list(elements, 5);

    EXPECT_EQ(list.remove(0), 1);
    EXPECT_EQ(list.remove(1), 3);
    EXPECT_EQ(list.remove(2), 5);

    expect_elements(list, {2, 4});
}

TEST(SinglyLinkedListTest, AddLastWorksAfterRemovingLastElement) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);

    list.remove(2);
    list.add_last(4);

    expect_elements(list, {1, 2, 4});
}

TEST(SinglyLinkedListTest, ListIsReusableAfterRemovingEveryElement) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);

    while (!list.empty()) {
        list.remove(0);
    }
    list.add_last(1);
    list.add_first(0);

    expect_elements(list, {0, 1});
}

TEST(SinglyLinkedListTest, ClearRemovesEveryElement) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);

    list.clear();

    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.get(0), std::out_of_range);

    list.add(7);
    expect_elements(list, {7});
}

TEST(SinglyLinkedListTest, ContainsFindsOnlyExistingElements) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);
    SinglyLinkedList<int> empty_list;

    EXPECT_TRUE(list.contains(1));
    EXPECT_TRUE(list.contains(3));
    EXPECT_FALSE(list.contains(4));
    EXPECT_FALSE(empty_list.contains(1));
}

TEST(SinglyLinkedListTest, CopyConstructorMakesDeepCopy) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> original(elements, 3);
    SinglyLinkedList<int> copied(original);

    copied.set(0, 10);
    copied.add(4);

    expect_elements(original, {1, 2, 3});
    expect_elements(copied, {10, 2, 3, 4});
}

TEST(SinglyLinkedListTest, CopyAssignmentMakesDeepCopy) {
    int original_elements[] = {1, 2, 3};
    int assigned_elements[] = {9, 9};
    SinglyLinkedList<int> original(original_elements, 3);
    SinglyLinkedList<int> assigned(assigned_elements, 2);

    assigned = original;
    assigned.remove(0);

    expect_elements(original, {1, 2, 3});
    expect_elements(assigned, {2, 3});
}

TEST(SinglyLinkedListTest, CopyAssignmentFromEmptyList) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);
    SinglyLinkedList<int> empty_list;

    list = empty_list;
    EXPECT_TRUE(list.empty());

    list.add(1);
    expect_elements(list, {1});
}

TEST(SinglyLinkedListTest, SelfAssignmentKeepsElements) {
    int elements[] = {1, 2, 3};
    SinglyLinkedList<int> list(elements, 3);
    SinglyLinkedList<int> &same_list = list;

    list = same_list;

    expect_elements(list, {1, 2, 3});
}

TEST(SinglyLinkedListTest, WorksAsListInterface) {
    SinglyLinkedList<int> concrete_list;
    List<int> &list = concrete_list;

    list.add(1);
    list.add_first(0);

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 0);
    EXPECT_EQ(list.remove(1), 1);
}

TEST(SinglyLinkedListTest, ReleasesEveryElement) {
    TrackedValue::alive_count = 0;
    {
        SinglyLinkedList<TrackedValue> list;
        for (int value = 0; value < 5; ++value) {
            list.add(TrackedValue(value));
        }
        EXPECT_EQ(TrackedValue::alive_count, 5);

        EXPECT_EQ(list.remove(2).value(), 2);
        EXPECT_EQ(TrackedValue::alive_count, 4);

        SinglyLinkedList<TrackedValue> copied(list);
        EXPECT_EQ(TrackedValue::alive_count, 8);

        copied = list;
        EXPECT_EQ(TrackedValue::alive_count, 8);

        copied.clear();
        EXPECT_EQ(TrackedValue::alive_count, 4);
    }
    EXPECT_EQ(TrackedValue::alive_count, 0);
}
