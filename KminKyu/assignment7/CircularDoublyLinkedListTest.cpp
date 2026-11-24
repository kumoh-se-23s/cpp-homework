#include <gtest/gtest.h>
#include <cstddef>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "CircularDoublyLinkedList.h"
#include "OOldMyString.h"
#include "Student.h"

// =====================================================================
// CircularDoublyLinkedList 테스트
//
//  - add() 는 오름차순 정렬 삽입이고, head 는 항상 가장 작은 값이다.
//    같은 값은 기존 값들 "뒤에" 들어간다.
//  - 원형이므로 마지막 노드의 next 는 head, head 의 prev 는 마지막 노드이다.
//  - 노드가 1개일 때는 next, prev 모두 자기 자신이다.
//  - find() / remove() 는 같은 값이 여러 개면 head 쪽에서 가장 가까운 것을 대상으로 한다.
//  - 전제 조건 (위반 시 UB, 테스트 제외):
//      * DoubleNode* 생성자에는 nullptr 또는 정렬된 정상 원형 체인만 전달한다.
//      * add(DoubleNode*) 에는 nullptr, 단독 힙 노드, 또는 다른 리스트에 속하지 않은
//        정상 원형 체인만 전달한다. (선형 체인, 6자형 체인, 스택 노드, 소유권 중복은 UB)
//
//  clear() 가 무한 루프에 빠지는 버그는 테스트가 "끝나지 않는" 형태로 나타난다.
//  ctest 의 --timeout 이나 `timeout 60 ./test` 로 실행하는 것을 권장한다.
// =====================================================================

namespace {
    // head 에서 next 방향으로 순회한 값
    template <typename T>
    std::vector<T> to_vector(const CircularDoublyLinkedList<T> &list) {
        std::vector<T> result;
        const DoubleNode<T> *head = list.getHead();
        if (head == nullptr) {
            return result;
        }
        const DoubleNode<T> *now = head;
        do {
            result.push_back(now->getItem());
            now = now->getNext();
            if (result.size() > 100000) {
                ADD_FAILURE() << "순회가 head 로 돌아오지 않는다";
                break;
            }
        } while (now != head && now != nullptr);
        return result;
    }

    // head 에서 prev 방향으로 순회한 값 (head 부터 시작해서 반대로 한 바퀴)
    template <typename T>
    std::vector<T> to_vector_backward(const CircularDoublyLinkedList<T> &list) {
        std::vector<T> result;
        const DoubleNode<T> *head = list.getHead();
        if (head == nullptr) {
            return result;
        }
        const DoubleNode<T> *now = head;
        do {
            result.push_back(now->getItem());
            now = now->getPrev();
            if (result.size() > 100000) {
                ADD_FAILURE() << "역방향 순회가 head 로 돌아오지 않는다";
                break;
            }
        } while (now != head && now != nullptr);
        return result;
    }

    // 모든 노드에서 next->prev == 자기 자신, prev->next == 자기 자신 이어야 한다.
    template <typename T>
    void expect_links_consistent(const CircularDoublyLinkedList<T> &list) {
        const DoubleNode<T> *head = list.getHead();
        if (head == nullptr) {
            return;
        }
        const DoubleNode<T> *now = head;
        int count = 0;
        do {
            ASSERT_NE(nullptr, now->getNext());
            ASSERT_NE(nullptr, now->getPrev());
            EXPECT_EQ(now, now->getNext()->getPrev());
            EXPECT_EQ(now, now->getPrev()->getNext());
            now = now->getNext();
        } while (now != head && ++count <= 100000);
    }

    template <typename T>
    void expect_list(const CircularDoublyLinkedList<T> &list, const std::vector<T> &expected) {
        EXPECT_EQ(expected, to_vector(list));
        EXPECT_EQ(static_cast<int>(expected.size()), list.getSize());
        EXPECT_EQ(expected.empty(), list.getHead() == nullptr);

        // 역방향 순회는 head 다음에 마지막 노드부터 나온다: [head, last, ..., second]
        std::vector<T> expectedBackward;
        if (!expected.empty()) {
            expectedBackward.push_back(expected.front());
            for (auto iterator = expected.rbegin(); iterator != expected.rend() - 1; ++iterator) {
                expectedBackward.push_back(*iterator);
            }
        }
        EXPECT_EQ(expectedBackward, to_vector_backward(list));
        expect_links_consistent(list);
    }

    CircularDoublyLinkedList<int> make_list(const std::vector<int> &items) {
        CircularDoublyLinkedList<int> list;
        for (const int item : items) {
            list.add(item);
        }
        return list;
    }

    // 어떤 리스트에도 속하지 않은 정상 원형 이중 연결 체인을 생성한다.
    DoubleNode<int> *make_circular_chain(const std::vector<int> &items) {
        if (items.empty()) {
            return nullptr;
        }
        auto *first = new DoubleNode<int>(items.front());
        DoubleNode<int> *prev = first;
        for (std::size_t i = 1; i < items.size(); ++i) {
            auto *node = new DoubleNode<int>(items[i]);
            prev->setNext(node);
            node->setPrev(prev);
            prev = node;
        }
        prev->setNext(first);
        first->setPrev(prev);
        return first;
    }
}

// ---------------------------------------------------------------------
// 인터페이스 제약 (복사 금지 / const 메서드)
// ---------------------------------------------------------------------

TEST(CircularListContract, NonCopyableButMovable) {
    static_assert(!std::is_copy_constructible_v<CircularDoublyLinkedList<int>>);
    static_assert(!std::is_copy_assignable_v<CircularDoublyLinkedList<int>>);
    static_assert(std::is_move_constructible_v<CircularDoublyLinkedList<int>>);
    static_assert(std::is_move_assignable_v<CircularDoublyLinkedList<int>>);
    SUCCEED();
}

TEST(CircularListContract, ConstObjectSupportsReadOperations) {
    const CircularDoublyLinkedList<int> list = make_list({10, 20, 30});
    expect_list(list, std::vector<int>{10, 20, 30});
    ASSERT_NE(nullptr, list.find(20));
    EXPECT_EQ(20, list.find(20)->getItem());
    EXPECT_EQ(nullptr, list.find(99));
}

// ---------------------------------------------------------------------
// 빈 리스트
// ---------------------------------------------------------------------

TEST(CircularListEmpty, DefaultConstructed) {
    CircularDoublyLinkedList<int> list;
    EXPECT_EQ(0, list.getSize());
    EXPECT_EQ(nullptr, list.getHead());
}

TEST(CircularListEmpty, FindRemoveClearOnEmptyListAreSafe) {
    CircularDoublyLinkedList<int> list;
    EXPECT_EQ(nullptr, list.find(1));
    EXPECT_FALSE(list.remove(1));
    list.clear();
    expect_list(list, std::vector<int>{});
}

// ---------------------------------------------------------------------
// head 포인터 생성자: CircularDoublyLinkedList(DoubleNode<T> *head)
// ---------------------------------------------------------------------

TEST(CircularListHeadConstructor, NullHeadCreatesEmptyList) {
    CircularDoublyLinkedList<int> list(nullptr);
    expect_list(list, std::vector<int>{});
}

TEST(CircularListHeadConstructor, SingleSelfLinkedNode) {
    DoubleNode<int> *node = make_circular_chain({42});
    CircularDoublyLinkedList<int> list(node);
    EXPECT_EQ(node, list.getHead());
    expect_list(list, std::vector<int>{42});
}

TEST(CircularListHeadConstructor, CircularChainPreservesNodesAndSize) {
    DoubleNode<int> *chain = make_circular_chain({1, 2, 3});
    CircularDoublyLinkedList<int> list(chain);
    EXPECT_EQ(chain, list.getHead());
    expect_list(list, std::vector<int>{1, 2, 3});
}

// ---------------------------------------------------------------------
// add (const T&)
// ---------------------------------------------------------------------

TEST(CircularListAdd, FirstNodePointsToItself) {
    CircularDoublyLinkedList<int> list;
    const DoubleNode<int> *node = list.add(5);
    ASSERT_NE(nullptr, node);
    EXPECT_EQ(node, list.getHead());
    EXPECT_EQ(node, node->getNext());
    EXPECT_EQ(node, node->getPrev());
    EXPECT_EQ(1, list.getSize());
    EXPECT_EQ(5, node->getItem());
}

TEST(CircularListAdd, SizeIncreasesWithEveryAdd) {
    CircularDoublyLinkedList<int> list;
    for (int count = 1; count <= 10; ++count) {
        list.add(count * 3 % 7);
        EXPECT_EQ(count, list.getSize());
    }
}

TEST(CircularListAdd, ReturnsInsertedNode) {
    CircularDoublyLinkedList<int> list;
    list.add(10);
    const DoubleNode<int> *middle = list.add(5);
    const DoubleNode<int> *tail = list.add(20);
    ASSERT_NE(nullptr, middle);
    ASSERT_NE(nullptr, tail);
    EXPECT_EQ(5, middle->getItem());
    EXPECT_EQ(20, tail->getItem());
}

TEST(CircularListAdd, KeepsAscendingOrderForAnyInsertOrder) {
    CircularDoublyLinkedList<int> ascending = make_list({1, 2, 3, 4, 5});
    expect_list(ascending, std::vector<int>{1, 2, 3, 4, 5});

    CircularDoublyLinkedList<int> descending = make_list({5, 4, 3, 2, 1});
    expect_list(descending, std::vector<int>{1, 2, 3, 4, 5});

    CircularDoublyLinkedList<int> mixed = make_list({5, 3, 9, 7, 1});
    expect_list(mixed, std::vector<int>{1, 3, 5, 7, 9});
}

TEST(CircularListAdd, SmallerThanHeadBecomesNewHead) {
    CircularDoublyLinkedList<int> list = make_list({10, 20});
    list.add(5);
    ASSERT_NE(nullptr, list.getHead());
    EXPECT_EQ(5, list.getHead()->getItem());
    expect_list(list, std::vector<int>{5, 10, 20});
}

TEST(CircularListAdd, LargerThanAllGoesToTail) {
    CircularDoublyLinkedList<int> list = make_list({10, 20});
    list.add(30);
    expect_list(list, std::vector<int>{10, 20, 30});
    EXPECT_EQ(10, list.getHead()->getItem());
    EXPECT_EQ(30, list.getHead()->getPrev()->getItem());
}

TEST(CircularListAdd, TwoNodesAreLinkedBothWays) {
    CircularDoublyLinkedList<int> list = make_list({2, 1});
    const DoubleNode<int> *head = list.getHead();
    ASSERT_NE(nullptr, head);
    EXPECT_EQ(1, head->getItem());
    EXPECT_EQ(2, head->getNext()->getItem());
    EXPECT_EQ(head, head->getNext()->getNext());
    EXPECT_EQ(head->getNext(), head->getPrev());
}

TEST(CircularListAdd, DuplicatesAreKept) {
    CircularDoublyLinkedList<int> list = make_list({2, 2, 2});
    expect_list(list, std::vector<int>{2, 2, 2});
    list.add(1);
    list.add(3);
    expect_list(list, std::vector<int>{1, 2, 2, 2, 3});
}

TEST(CircularListAdd, DuplicateIsInsertedAfterExistingEqualNodes) {
    CircularDoublyLinkedList<int> list;
    const DoubleNode<int> *first = list.add(2);
    const DoubleNode<int> *second = list.add(2);
    const DoubleNode<int> *third = list.add(2);

    EXPECT_EQ(first, list.getHead());
    EXPECT_EQ(second, first->getNext());
    EXPECT_EQ(third, second->getNext());
    EXPECT_EQ(first, third->getNext());
    expect_list(list, std::vector<int>{2, 2, 2});
}

TEST(CircularListAdd, DuplicateAtHeadKeepsOriginalHead) {
    CircularDoublyLinkedList<int> list;
    const DoubleNode<int> *first = list.add(10);
    list.add(20);
    const DoubleNode<int> *second = list.add(10);

    EXPECT_EQ(first, list.getHead());
    EXPECT_EQ(second, first->getNext());
    expect_list(list, std::vector<int>{10, 10, 20});
}

TEST(CircularListAdd, DuplicateAtTailAppendsToEnd) {
    CircularDoublyLinkedList<int> list;
    list.add(10);
    const DoubleNode<int> *first = list.add(20);
    const DoubleNode<int> *second = list.add(20);

    EXPECT_EQ(first, list.getHead()->getNext());
    EXPECT_EQ(second, list.getHead()->getPrev());
    expect_list(list, std::vector<int>{10, 20, 20});
}

TEST(CircularListAdd, ManyItems) {
    CircularDoublyLinkedList<int> list;
    std::vector<int> expected;
    for (int value = 499; value >= 0; --value) {
        list.add(value);
    }
    for (int value = 0; value < 500; ++value) {
        expected.push_back(value);
    }
    expect_list(list, expected);
}

TEST(CircularListAdd, NegativeValues) {
    CircularDoublyLinkedList<int> list = make_list({0, -5, 5, -10});
    expect_list(list, std::vector<int>{-10, -5, 0, 5});
}

// ---------------------------------------------------------------------
// add (DoubleNode<T>*) 및 addAll
// ---------------------------------------------------------------------

TEST(CircularListAddNode, NullNodeReturnsNullptrAndChangesNothing) {
    CircularDoublyLinkedList<int> list = make_list({1, 2});
    EXPECT_EQ(nullptr, list.add(static_cast<DoubleNode<int> *>(nullptr)));
    expect_list(list, std::vector<int>{1, 2});
}

TEST(CircularListAddNode, SingleUnlinkedNodeInsertedInOrder) {
    CircularDoublyLinkedList<int> list = make_list({10, 30});
    auto *node = new DoubleNode<int>(20);
    EXPECT_EQ(node, list.add(node));
    expect_list(list, std::vector<int>{10, 20, 30});
}

TEST(CircularListAddNode, SingleSelfLinkedNodeInsertedIntoEmptyAndNonEmpty) {
    CircularDoublyLinkedList<int> list;
    DoubleNode<int> *first = make_circular_chain({20});
    EXPECT_EQ(first, list.add(first));

    DoubleNode<int> *second = make_circular_chain({10});
    EXPECT_EQ(second, list.add(second));

    expect_list(list, std::vector<int>{10, 20});
}

TEST(CircularListAddNode, CircularChainAddsAllNodesIntoEmptyList) {
    CircularDoublyLinkedList<int> list;
    DoubleNode<int> *chain = make_circular_chain({30, 10, 20});

    EXPECT_NE(nullptr, list.add(chain));
    expect_list(list, std::vector<int>{10, 20, 30});
}

TEST(CircularListAddNode, CircularChainAddsAllNodesIntoNonEmptyListInOrder) {
    CircularDoublyLinkedList<int> list = make_list({10, 40});
    DoubleNode<int> *chain = make_circular_chain({30, 5, 20, 50});

    EXPECT_NE(nullptr, list.add(chain));
    expect_list(list, std::vector<int>{5, 10, 20, 30, 40, 50});
}

TEST(CircularListAddNode, CircularChainWithDuplicatesPreservesAllNodes) {
    CircularDoublyLinkedList<int> list = make_list({10, 20});
    DoubleNode<int> *chain = make_circular_chain({20, 10, 20});

    EXPECT_NE(nullptr, list.add(chain));
    expect_list(list, std::vector<int>{10, 10, 20, 20, 20});
}

// ---------------------------------------------------------------------
// find
// ---------------------------------------------------------------------

TEST(CircularListFind, FindsEveryExistingItem) {
    CircularDoublyLinkedList<int> list = make_list({10, 20, 30, 40});
    for (const int value : {10, 20, 30, 40}) {
        DoubleNode<int> *node = list.find(value);
        ASSERT_NE(nullptr, node) << "value: " << value;
        EXPECT_EQ(value, node->getItem());
    }
}

TEST(CircularListFind, FindsHeadAndTail) {
    CircularDoublyLinkedList<int> list = make_list({10, 20, 30});
    EXPECT_EQ(list.getHead(), list.find(10));
    EXPECT_EQ(list.getHead()->getPrev(), list.find(30));
}

TEST(CircularListFind, SingleNode) {
    CircularDoublyLinkedList<int> list = make_list({7});
    EXPECT_EQ(list.getHead(), list.find(7));
    EXPECT_EQ(nullptr, list.find(8));
}

TEST(CircularListFind, MissingItemsReturnNullptr) {
    CircularDoublyLinkedList<int> list = make_list({10, 20, 30});
    EXPECT_EQ(nullptr, list.find(5));
    EXPECT_EQ(nullptr, list.find(15));
    EXPECT_EQ(nullptr, list.find(35));
}

TEST(CircularListFind, DuplicatesReturnFirstOneFromHead) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 2, 2, 3});
    DoubleNode<int> *node = list.find(2);
    ASSERT_NE(nullptr, node);
    EXPECT_EQ(list.getHead()->getNext(), node);
}

TEST(CircularListFind, DuplicatesAtHeadAndTailReturnClosestToHead) {
    CircularDoublyLinkedList<int> list;
    const DoubleNode<int> *headFirst = list.add(1);
    list.add(1);
    const DoubleNode<int> *tailFirst = list.add(3);
    list.add(3);

    EXPECT_EQ(headFirst, list.find(1));
    EXPECT_EQ(tailFirst, list.find(3));
}

TEST(CircularListFind, FindDoesNotChangeList) {
    CircularDoublyLinkedList<int> list = make_list({10, 20, 30});
    list.find(20);
    list.find(99);
    expect_list(list, std::vector<int>{10, 20, 30});
}

// ---------------------------------------------------------------------
// remove
// ---------------------------------------------------------------------

TEST(CircularListRemove, OnlyElement) {
    CircularDoublyLinkedList<int> list = make_list({7});
    EXPECT_TRUE(list.remove(7));
    expect_list(list, std::vector<int>{});
    EXPECT_EQ(nullptr, list.getHead());
}

TEST(CircularListRemove, OnlyElementNotMatching) {
    CircularDoublyLinkedList<int> list = make_list({7});
    EXPECT_FALSE(list.remove(8));
    expect_list(list, std::vector<int>{7});
}

TEST(CircularListRemove, HeadMovesToNextNode) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3});
    EXPECT_TRUE(list.remove(1));
    expect_list(list, std::vector<int>{2, 3});
    EXPECT_EQ(2, list.getHead()->getItem());
}

TEST(CircularListRemove, Middle) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3});
    EXPECT_TRUE(list.remove(2));
    expect_list(list, std::vector<int>{1, 3});
}

TEST(CircularListRemove, Tail) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3});
    EXPECT_TRUE(list.remove(3));
    expect_list(list, std::vector<int>{1, 2});
}

TEST(CircularListRemove, TwoNodesRemoveHeadThenLast) {
    CircularDoublyLinkedList<int> list = make_list({1, 2});
    EXPECT_TRUE(list.remove(1));
    expect_list(list, std::vector<int>{2});
    EXPECT_EQ(list.getHead(), list.getHead()->getNext());
    EXPECT_EQ(list.getHead(), list.getHead()->getPrev());
    EXPECT_TRUE(list.remove(2));
    expect_list(list, std::vector<int>{});
}

TEST(CircularListRemove, TwoNodesRemoveTailThenHead) {
    CircularDoublyLinkedList<int> list = make_list({1, 2});
    EXPECT_TRUE(list.remove(2));
    expect_list(list, std::vector<int>{1});
    EXPECT_EQ(list.getHead(), list.getHead()->getNext());
    EXPECT_EQ(list.getHead(), list.getHead()->getPrev());
    EXPECT_TRUE(list.remove(1));
    expect_list(list, std::vector<int>{});
}

TEST(CircularListRemove, MissingItemChangesNothing) {
    CircularDoublyLinkedList<int> list = make_list({10, 20, 30});
    EXPECT_FALSE(list.remove(5));
    EXPECT_FALSE(list.remove(15));
    EXPECT_FALSE(list.remove(35));
    expect_list(list, std::vector<int>{10, 20, 30});
}

TEST(CircularListRemove, DuplicateRemovesOnlyOne) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 2, 2, 3});
    EXPECT_TRUE(list.remove(2));
    expect_list(list, std::vector<int>{1, 2, 2, 3});
    EXPECT_TRUE(list.remove(2));
    EXPECT_TRUE(list.remove(2));
    EXPECT_FALSE(list.remove(2));
    expect_list(list, std::vector<int>{1, 3});
}

TEST(CircularListRemove, DuplicateRemovesClosestToHead) {
    CircularDoublyLinkedList<int> list;
    list.add(1);
    list.add(2);
    const DoubleNode<int> *secondTwo = list.add(2);
    const DoubleNode<int> *thirdTwo = list.add(2);
    list.add(3);

    EXPECT_TRUE(list.remove(2));
    EXPECT_EQ(secondTwo, list.find(2));
    EXPECT_EQ(thirdTwo, secondTwo->getNext());
    expect_list(list, std::vector<int>{1, 2, 2, 3});
}

TEST(CircularListRemove, DuplicateAtHeadMovesHeadToSecondDuplicate) {
    CircularDoublyLinkedList<int> list;
    list.add(1);
    const DoubleNode<int> *secondOne = list.add(1);
    list.add(2);

    EXPECT_TRUE(list.remove(1));
    EXPECT_EQ(secondOne, list.getHead());
    expect_list(list, std::vector<int>{1, 2});
}

TEST(CircularListRemove, RemovingSameItemTwiceReturnsFalseSecondTime) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3});
    EXPECT_TRUE(list.remove(2));
    EXPECT_FALSE(list.remove(2));
    expect_list(list, std::vector<int>{1, 3});
}

TEST(CircularListRemove, RemoveEverythingFromHeadRepeatedly) {
    CircularDoublyLinkedList<int> list = make_list({4, 2, 5, 1, 3});
    for (const int value : {1, 2, 3, 4, 5}) {
        EXPECT_TRUE(list.remove(value)) << "value: " << value;
        expect_links_consistent(list);
    }
    expect_list(list, std::vector<int>{});
}

TEST(CircularListRemove, RemoveEverythingFromTailRepeatedly) {
    CircularDoublyLinkedList<int> list = make_list({4, 2, 5, 1, 3});
    for (const int value : {5, 4, 3, 2, 1}) {
        EXPECT_TRUE(list.remove(value)) << "value: " << value;
        expect_links_consistent(list);
    }
    expect_list(list, std::vector<int>{});
}

TEST(CircularListRemove, ListCanBeReusedAfterBecomingEmpty) {
    CircularDoublyLinkedList<int> list = make_list({1});
    list.remove(1);
    list.add(9);
    list.add(3);
    expect_list(list, std::vector<int>{3, 9});
}

// ---------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------

TEST(CircularListClear, EmptiesList) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3, 4});
    list.clear();
    expect_list(list, std::vector<int>{});
    EXPECT_EQ(nullptr, list.find(1));
}

TEST(CircularListClear, SingleNode) {
    CircularDoublyLinkedList<int> list = make_list({1});
    list.clear();
    expect_list(list, std::vector<int>{});
}

TEST(CircularListClear, ClearTwiceIsSafe) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3});
    list.clear();
    list.clear();
    expect_list(list, std::vector<int>{});
}

TEST(CircularListClear, ListCanBeReusedAfterClear) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3});
    list.clear();
    list.add(5);
    list.add(4);
    expect_list(list, std::vector<int>{4, 5});
}

TEST(CircularListClear, DestructorAfterClearIsSafe) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3});
    list.clear();
}

TEST(CircularListClear, DestructorOfNonEmptyListFreesAllNodes) {
    // 소멸자가 원형 리스트를 한 바퀴만 돌며 모두 해제하는지 (LeakSanitizer 로 확인)
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3, 4, 5});
}

// ---------------------------------------------------------------------
// 이동 생성 / 이동 대입
// ---------------------------------------------------------------------

TEST(CircularListMove, MoveConstructor) {
    CircularDoublyLinkedList<int> source = make_list({3, 1, 2});
    CircularDoublyLinkedList<int> target(std::move(source));
    expect_list(target, std::vector<int>{1, 2, 3});
    expect_list(source, std::vector<int>{});
}

TEST(CircularListMove, MoveConstructorFromEmptyList) {
    CircularDoublyLinkedList<int> source;
    CircularDoublyLinkedList<int> target(std::move(source));
    expect_list(target, std::vector<int>{});
    expect_list(source, std::vector<int>{});
}

TEST(CircularListMove, MoveAssignmentReplacesOldContent) {
    CircularDoublyLinkedList<int> source = make_list({3, 1, 2});
    CircularDoublyLinkedList<int> target = make_list({100, 200});
    target = std::move(source);
    expect_list(target, std::vector<int>{1, 2, 3});
    expect_list(source, std::vector<int>{});
}

TEST(CircularListMove, MoveAssignmentFromEmptyClearsTarget) {
    CircularDoublyLinkedList<int> source;
    CircularDoublyLinkedList<int> target = make_list({10, 20});
    target = std::move(source);
    expect_list(target, std::vector<int>{});
    expect_list(source, std::vector<int>{});
}

TEST(CircularListMove, MoveAssignmentIntoEmptyTarget) {
    CircularDoublyLinkedList<int> source = make_list({3, 1, 2});
    CircularDoublyLinkedList<int> target;
    target = std::move(source);
    expect_list(target, std::vector<int>{1, 2, 3});
    expect_list(source, std::vector<int>{});
}

TEST(CircularListMove, SelfMoveAssignmentKeepsContent) {
    CircularDoublyLinkedList<int> list = make_list({1, 2, 3});
    CircularDoublyLinkedList<int> &alias = list;
    list = std::move(alias);
    expect_list(list, std::vector<int>{1, 2, 3});
}

TEST(CircularListMove, MovedFromListIsReusable) {
    CircularDoublyLinkedList<int> source = make_list({1, 2, 3});
    CircularDoublyLinkedList<int> target(std::move(source));
    source.add(7);
    expect_list(source, std::vector<int>{7});
    expect_list(target, std::vector<int>{1, 2, 3});
}

// ---------------------------------------------------------------------
// 다른 타입
// ---------------------------------------------------------------------

TEST(CircularListGeneric, MyStringItems) {
    CircularDoublyLinkedList<OOldMyString> list;
    list.add(OOldMyString("banana"));
    list.add(OOldMyString("apple"));
    list.add(OOldMyString("cherry"));

    std::vector<std::string> actual;
    const DoubleNode<OOldMyString> *head = list.getHead();
    ASSERT_NE(nullptr, head);
    const DoubleNode<OOldMyString> *now = head;
    do {
        std::ostringstream stream;
        stream << now->getItem();
        actual.push_back(stream.str());
        now = now->getNext();
    } while (now != head);

    EXPECT_EQ((std::vector<std::string>{"apple", "banana", "cherry"}), actual);
    EXPECT_NE(nullptr, list.find(OOldMyString("banana")));
    EXPECT_EQ(nullptr, list.find(OOldMyString("durian")));
    EXPECT_TRUE(list.remove(OOldMyString("apple")));
    EXPECT_EQ(2, list.getSize());
}

// ---------------------------------------------------------------------
// std::multiset 과 무작위 연산 비교 (차분 테스트)
// ---------------------------------------------------------------------

TEST(CircularListDifferential, RandomOperationsMatchMultiset) {
    std::mt19937 generator(20261119);
    CircularDoublyLinkedList<int> list;
    std::multiset<int> expected;

    for (int round = 0; round < 3000; ++round) {
        const int op = static_cast<int>(generator() % 4);
        const int value = static_cast<int>(generator() % 40);
        SCOPED_TRACE("round: " + std::to_string(round) + ", op: " + std::to_string(op) + ", value: " + std::to_string(value));

        switch (op) {
            case 0:
            case 1:
                list.add(value);
                expected.insert(value);
                break;
            case 2: {
                const auto iterator = expected.find(value);
                const bool exists = iterator != expected.end();
                EXPECT_EQ(exists, list.remove(value));
                if (exists) {
                    expected.erase(iterator);
                }
                break;
            }
            default: {
                DoubleNode<int> *node = list.find(value);
                EXPECT_EQ(expected.count(value) > 0, node != nullptr);
                if (node != nullptr) {
                    EXPECT_EQ(value, node->getItem());
                }
                break;
            }
        }

        expect_list(list, std::vector<int>(expected.begin(), expected.end()));
        if (HasFailure()) {
            return;
        }
    }
}
