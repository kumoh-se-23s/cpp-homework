#include <gtest/gtest.h>

#include "DoublyLinkedList.h"

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
    void expect_elements(const DoublyLinkedList<int> &list, const int (&expected_elements)[LENGTH]) {
        ASSERT_EQ(list.size(), LENGTH);

        for (int element_index = 0; element_index < LENGTH; ++element_index) {
            EXPECT_EQ(list.get(element_index), expected_elements[element_index]) << "index: " << element_index;
        }
    }
}


TEST(DoublyLinkedListTest, DefaultConstructorCreatesEmptyList) {
    DoublyLinkedList<int> list;

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(DoublyLinkedListTest, ArrayConstructorKeepsOrder) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);

    expect_elements(list, {1, 2, 3});
}

TEST(DoublyLinkedListTest, ArrayConstructorAcceptsZeroLengthWithNullptr) {
    DoublyLinkedList<int> list(nullptr, 0);

    EXPECT_TRUE(list.empty());
}

TEST(DoublyLinkedListTest, ArrayConstructorRejectsInvalidArguments) {
    int elements[] = {1};

    EXPECT_THROW(DoublyLinkedList<int>(elements, -1), std::invalid_argument);
    EXPECT_THROW(DoublyLinkedList<int>(nullptr, 1), std::invalid_argument);
}

TEST(DoublyLinkedListTest, AddAppendsToEnd) {
    DoublyLinkedList<int> list;

    list.add(1);
    list.add(2);
    list.add(3);

    expect_elements(list, {1, 2, 3});
}

TEST(DoublyLinkedListTest, AddAllAppendsAfterExistingElements) {
    DoublyLinkedList<int> list;
    int elements[] = {2, 3, 4};

    list.add(1);
    list.add_all(elements, 3);

    expect_elements(list, {1, 2, 3, 4});
}

TEST(DoublyLinkedListTest, AddFirstPrependsElement) {
    DoublyLinkedList<int> list;

    list.add_first(3);
    list.add_first(2);
    list.add_first(1);

    expect_elements(list, {1, 2, 3});
}

TEST(DoublyLinkedListTest, AddLastAfterAddFirstOnEmptyList) {
    DoublyLinkedList<int> list;

    list.add_first(1);
    list.add_last(2);

    expect_elements(list, {1, 2});
}

TEST(DoublyLinkedListTest, InsertAtFrontMiddleAndEnd) {
    DoublyLinkedList<int> list;

    list.insert(0, 2);
    list.insert(0, 0);
    list.insert(1, 1);
    list.insert(3, 3);

    expect_elements(list, {0, 1, 2, 3});
}

TEST(DoublyLinkedListTest, InsertOutOfRangeThrows) {
    DoublyLinkedList<int> list;
    list.add(1);

    EXPECT_THROW(list.insert(-1, 0), std::out_of_range);
    EXPECT_THROW(list.insert(2, 0), std::out_of_range);
}

TEST(DoublyLinkedListTest, GetPeekAndSetAccessElement) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);

    list.set(1, 20);

    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.peek(1), 20);
}

TEST(DoublyLinkedListTest, OperatorBracketReturnsReference) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);

    list[2] = 30;
    const DoublyLinkedList<int> &const_list = list;

    EXPECT_EQ(list.get(2), 30);
    EXPECT_EQ(const_list[2], 30);
}

TEST(DoublyLinkedListTest, InvalidIndexThrows) {
    int elements[] = {1, 2};
    DoublyLinkedList<int> list(elements, 2);
    DoublyLinkedList<int> empty_list;

    EXPECT_THROW(list.get(-1), std::out_of_range);
    EXPECT_THROW(list.get(2), std::out_of_range);
    EXPECT_THROW(list.peek(2), std::out_of_range);
    EXPECT_THROW(list.set(2, 0), std::out_of_range);
    EXPECT_THROW(list[2], std::out_of_range);
    EXPECT_THROW(list.remove(2), std::out_of_range);
    EXPECT_THROW(empty_list.get(0), std::out_of_range);
    EXPECT_THROW(empty_list.remove(0), std::out_of_range);
}

TEST(DoublyLinkedListTest, RemoveFirstMiddleAndLast) {
    int elements[] = {1, 2, 3, 4, 5};
    DoublyLinkedList<int> list(elements, 5);

    EXPECT_EQ(list.remove(0), 1);
    EXPECT_EQ(list.remove(1), 3);
    EXPECT_EQ(list.remove(2), 5);

    expect_elements(list, {2, 4});
}

TEST(DoublyLinkedListTest, AddLastWorksAfterRemovingLastElement) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);

    list.remove(2);
    list.add_last(4);

    expect_elements(list, {1, 2, 4});
}

TEST(DoublyLinkedListTest, ListIsReusableAfterRemovingEveryElement) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);

    while (!list.empty()) {
        list.remove(0);
    }
    list.add_last(1);
    list.add_first(0);

    expect_elements(list, {0, 1});
}

TEST(DoublyLinkedListTest, ClearRemovesEveryElement) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);

    list.clear();

    EXPECT_TRUE(list.empty());
    EXPECT_THROW(list.get(0), std::out_of_range);

    list.add(7);
    expect_elements(list, {7});
}

TEST(DoublyLinkedListTest, ContainsFindsOnlyExistingElements) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);
    DoublyLinkedList<int> empty_list;

    EXPECT_TRUE(list.contains(1));
    EXPECT_TRUE(list.contains(3));
    EXPECT_FALSE(list.contains(4));
    EXPECT_FALSE(empty_list.contains(1));
}

TEST(DoublyLinkedListTest, CopyConstructorMakesDeepCopy) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> original(elements, 3);
    DoublyLinkedList<int> copied(original);

    copied.set(0, 10);
    copied.add(4);

    expect_elements(original, {1, 2, 3});
    expect_elements(copied, {10, 2, 3, 4});
}

TEST(DoublyLinkedListTest, CopyAssignmentMakesDeepCopy) {
    int original_elements[] = {1, 2, 3};
    int assigned_elements[] = {9, 9};
    DoublyLinkedList<int> original(original_elements, 3);
    DoublyLinkedList<int> assigned(assigned_elements, 2);

    assigned = original;
    assigned.remove(0);

    expect_elements(original, {1, 2, 3});
    expect_elements(assigned, {2, 3});
}

TEST(DoublyLinkedListTest, CopyAssignmentFromEmptyList) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);
    DoublyLinkedList<int> empty_list;

    list = empty_list;
    EXPECT_TRUE(list.empty());

    list.add(1);
    expect_elements(list, {1});
}

TEST(DoublyLinkedListTest, SelfAssignmentKeepsElements) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);
    DoublyLinkedList<int> &same_list = list;

    list = same_list;

    expect_elements(list, {1, 2, 3});
}

TEST(DoublyLinkedListTest, WorksAsListInterface) {
    DoublyLinkedList<int> concrete_list;
    List<int> &list = concrete_list;

    list.add(1);
    list.add_first(0);

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 0);
    EXPECT_EQ(list.remove(1), 1);
}

TEST(DoublyLinkedListTest, ReleasesEveryElement) {
    TrackedValue::alive_count = 0;
    {
        DoublyLinkedList<TrackedValue> list;
        for (int value = 0; value < 5; ++value) {
            list.add(TrackedValue(value));
        }
        EXPECT_EQ(TrackedValue::alive_count, 5);

        EXPECT_EQ(list.remove(2).value(), 2);
        EXPECT_EQ(TrackedValue::alive_count, 4);

        DoublyLinkedList<TrackedValue> copied(list);
        EXPECT_EQ(TrackedValue::alive_count, 8);

        copied = list;
        EXPECT_EQ(TrackedValue::alive_count, 8);

        copied.clear();
        EXPECT_EQ(TrackedValue::alive_count, 4);
    }
    EXPECT_EQ(TrackedValue::alive_count, 0);
}

TEST(DoublyLinkedListTest, AccessesEveryIndexFromBothHalves) {
    for (int list_size = 1; list_size <= 9; ++list_size) {
        DoublyLinkedList<int> list;
        for (int value = 0; value < list_size; ++value) {
            list.add(value);
        }

        for (int element_index = 0; element_index < list_size; ++element_index) {
            EXPECT_EQ(list.get(element_index), element_index) << "size: " << list_size;
        }
    }
}

TEST(DoublyLinkedListTest, SingleElementListCanBeRemovedAndRefilled) {
    DoublyLinkedList<int> list;

    list.add(1);
    EXPECT_EQ(list.remove(0), 1);
    EXPECT_TRUE(list.empty());

    list.add_first(2);
    list.add_last(3);
    expect_elements(list, {2, 3});
}

TEST(DoublyLinkedListTest, AddFirstAndAddLastKeepBothEndsConnected) {
    DoublyLinkedList<int> list;

    list.add_last(2);
    list.add_first(1);
    list.add_last(3);
    list.add_first(0);

    expect_elements(list, {0, 1, 2, 3});
    EXPECT_EQ(list.get(list.size() - 1), 3);
}

TEST(DoublyLinkedListTest, RemovingHeadMovesHeadToNextNode) {
    int elements[] = {1, 2, 3};
    DoublyLinkedList<int> list(elements, 3);

    list.remove(0);
    list.add_first(0);
    list.add_last(4);

    expect_elements(list, {0, 2, 3, 4});
}

TEST(DoublyLinkedListTest, MixedOperationsMatchArrayModel) {
    constexpr int MODEL_CAPACITY = 64;
    int model[MODEL_CAPACITY];
    int model_size = 0;
    DoublyLinkedList<int> list;

    for (int step = 0; step < 40; ++step) {
        int insert_index = model_size == 0 ? 0 : (step * 7) % (model_size + 1);
        for (int shift_index = model_size; shift_index > insert_index; --shift_index) {
            model[shift_index] = model[shift_index - 1];
        }
        model[insert_index] = step;
        ++model_size;
        list.insert(insert_index, step);

        if (step % 3 == 0) {
            int remove_index = (step * 5) % model_size;
            int removed_value = model[remove_index];
            for (int shift_index = remove_index; shift_index < model_size - 1; ++shift_index) {
                model[shift_index] = model[shift_index + 1];
            }
            --model_size;
            EXPECT_EQ(list.remove(remove_index), removed_value);
        }

        ASSERT_EQ(list.size(), model_size);
        for (int element_index = 0; element_index < model_size; ++element_index) {
            ASSERT_EQ(list.get(element_index), model[element_index]) << "step: " << step;
        }
    }
}
