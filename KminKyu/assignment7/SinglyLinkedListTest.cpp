#include <gtest/gtest.h>
#include <cstddef>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "MyString.h"
#include "SinglyLinkedList.h"
#include "Student.h"

// =====================================================================
// SinglyLinkedList 테스트
//
//  - add() 는 오름차순 정렬 삽입이다. 같은 값은 기존 값들 "뒤에" 들어간다.
//  - 마지막 노드의 next 는 nullptr 이다.
//  - find() / remove() 는 같은 값이 여러 개면 "앞쪽 첫 번째" 를 대상으로 한다.
//  - clear() 후에는 head == nullptr, size == 0 이어야 하고, 소멸자가 다시 돌아도 안전해야 한다.
//  - 전제 조건 (위반 시 UB, 테스트 제외):
//      * SingleNode* 생성자에는 nullptr 또는 정렬된 정상 선형 체인(끝이 nullptr)만 전달한다.
//      * add(SingleNode*) 에는 nullptr, 단독 힙 노드, 또는 다른 리스트에 속하지 않은
//        정상 선형 체인(정렬 여부 무관, 끝이 nullptr)만 전달한다.
//        (순환/6자형 체인, 스택 노드, 소유권 중복은 UB)
//
//  메모리 오류(이중 해제, 해제 후 사용, 누수)는 ASan / LSan 으로 실행해 확인한다.
// =====================================================================

namespace {
    template<typename T>
    std::vector<T> to_vector(SinglyLinkedList<T> &list) {
        std::vector<T> result;
        int guard = 0;
        for (SingleNode<T> *now = list.getHead(); now != nullptr; now = now->getNext()) {
            result.push_back(now->getItem());
            if (++guard > 100000) {
                ADD_FAILURE() << "리스트가 끝나지 않는다 (순환?)";
                break;
            }
        }
        return result;
    }

    template<typename T>
    void expect_list(SinglyLinkedList<T> &list, const std::vector<T> &expected) {
        EXPECT_EQ(expected, to_vector(list));
        EXPECT_EQ(static_cast<int>(expected.size()), list.getSize());
        EXPECT_EQ(expected.empty(), list.getHead() == nullptr);
    }

    SinglyLinkedList<int> make_list(const std::vector<int> &items) {
        SinglyLinkedList<int> list;
        for (const int item: items) {
            list.add(item);
        }
        return list;
    }

    // 어떤 리스트에도 속하지 않은 정상 선형 단일 연결 체인을 생성한다.
    SingleNode<int> *make_linear_chain(const std::vector<int> &items) {
        if (items.empty()) {
            return nullptr;
        }
        auto *first = new SingleNode<int>(items.front());
        SingleNode<int> *prev = first;
        for (std::size_t i = 1; i < items.size(); ++i) {
            auto *node = new SingleNode<int>(items[i]);
            prev->setNext(node);
            prev = node;
        }
        return first;
    }
}

// ---------------------------------------------------------------------
// 인터페이스 제약 (복사 금지 / 이동 가능)
// ---------------------------------------------------------------------

TEST(SinglyLinkedListContract, NonCopyableButMovable) {
    static_assert(!std::is_copy_constructible_v<SinglyLinkedList<int> >);
    static_assert(!std::is_copy_assignable_v<SinglyLinkedList<int> >);
    static_assert(std::is_move_constructible_v<SinglyLinkedList<int> >);
    static_assert(std::is_move_assignable_v<SinglyLinkedList<int> >);
    SUCCEED();
}

// ---------------------------------------------------------------------
// 빈 리스트
// ---------------------------------------------------------------------

TEST(SinglyLinkedListEmpty, DefaultConstructed) {
    SinglyLinkedList<int> list;
    EXPECT_EQ(0, list.getSize());
    EXPECT_EQ(nullptr, list.getHead());
}

TEST(SinglyLinkedListEmpty, FindRemoveClearOnEmptyListAreSafe) {
    SinglyLinkedList<int> list;
    EXPECT_EQ(nullptr, list.find(1));
    EXPECT_FALSE(list.remove(1));
    list.clear();
    expect_list(list, std::vector<int>{});
}

// ---------------------------------------------------------------------
// head 포인터 생성자: SinglyLinkedList(SingleNode<T> *head)
// ---------------------------------------------------------------------

TEST(SinglyLinkedListHeadConstructor, NullHeadCreatesEmptyList) {
    SinglyLinkedList<int> list(static_cast<SingleNode<int> *>(nullptr));
    expect_list(list, std::vector<int>{});
}

TEST(SinglyLinkedListHeadConstructor, SingleNode) {
    SingleNode<int> *node = make_linear_chain({42});
    SinglyLinkedList<int> list(node);
    EXPECT_EQ(node, list.getHead());
    EXPECT_EQ(nullptr, list.getHead()->getNext());
    expect_list(list, std::vector<int>{42});
}

TEST(SinglyLinkedListHeadConstructor, LinearChainPreservesNodesAndSize) {
    SingleNode<int> *chain = make_linear_chain({1, 2, 3});
    SinglyLinkedList<int> list(chain);
    EXPECT_EQ(chain, list.getHead());
    expect_list(list, std::vector<int>{1, 2, 3});
}

// ---------------------------------------------------------------------
// add (T item): 정렬 삽입
// ---------------------------------------------------------------------

TEST(SinglyLinkedListAdd, SingleItem) {
    SinglyLinkedList<int> list;
    const SingleNode<int> *node = list.add(5);
    ASSERT_NE(nullptr, node);
    EXPECT_EQ(node, list.getHead());
    EXPECT_EQ(nullptr, node->getNext());
    expect_list(list, std::vector<int>{5});
}

TEST(SinglyLinkedListAdd, SizeIncreasesWithEveryAdd) {
    SinglyLinkedList<int> list;
    for (int count = 1; count <= 10; ++count) {
        list.add(count * 3 % 7);
        EXPECT_EQ(count, list.getSize());
    }
}

TEST(SinglyLinkedListAdd, ReturnsInsertedNode) {
    SinglyLinkedList<int> list;
    const SingleNode<int> *first = list.add(5);
    const SingleNode<int> *second = list.add(3);
    const SingleNode<int> *third = list.add(9);
    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    ASSERT_NE(nullptr, third);
    EXPECT_EQ(5, first->getItem());
    EXPECT_EQ(3, second->getItem());
    EXPECT_EQ(9, third->getItem());
    EXPECT_EQ(second, list.getHead());
}

TEST(SinglyLinkedListAdd, KeepsAscendingOrderForAnyInsertOrder) {
    SinglyLinkedList<int> ascending = make_list({1, 2, 3, 4, 5});
    expect_list(ascending, std::vector<int>{1, 2, 3, 4, 5});

    SinglyLinkedList<int> descending = make_list({5, 4, 3, 2, 1});
    expect_list(descending, std::vector<int>{1, 2, 3, 4, 5});

    SinglyLinkedList<int> mixed = make_list({3, 1, 4, 1, 5, 9, 2, 6});
    expect_list(mixed, std::vector<int>{1, 1, 2, 3, 4, 5, 6, 9});
}

TEST(SinglyLinkedListAdd, InsertAtFrontMiddleAndEnd) {
    SinglyLinkedList<int> list = make_list({10, 20, 30});
    list.add(5);
    expect_list(list, std::vector<int>{5, 10, 20, 30});
    list.add(25);
    expect_list(list, std::vector<int>{5, 10, 20, 25, 30});
    list.add(99);
    expect_list(list, std::vector<int>{5, 10, 20, 25, 30, 99});
}

TEST(SinglyLinkedListAdd, DuplicatesAreKept) {
    SinglyLinkedList<int> list = make_list({2, 2, 2});
    expect_list(list, std::vector<int>{2, 2, 2});
    list.add(1);
    list.add(3);
    expect_list(list, std::vector<int>{1, 2, 2, 2, 3});
}

TEST(SinglyLinkedListAdd, DuplicateIsInsertedAfterExistingEqualNodes) {
    SinglyLinkedList<int> list;
    const SingleNode<int> *first = list.add(2);
    const SingleNode<int> *second = list.add(2);
    const SingleNode<int> *third = list.add(2);

    EXPECT_EQ(first, list.getHead());
    EXPECT_EQ(second, first->getNext());
    EXPECT_EQ(third, second->getNext());
    EXPECT_EQ(nullptr, third->getNext());
    expect_list(list, std::vector<int>{2, 2, 2});
}

TEST(SinglyLinkedListAdd, DuplicateAtHeadKeepsOriginalHead) {
    SinglyLinkedList<int> list;
    const SingleNode<int> *first = list.add(10);
    list.add(20);
    const SingleNode<int> *second = list.add(10);

    EXPECT_EQ(first, list.getHead());
    EXPECT_EQ(second, first->getNext());
    expect_list(list, std::vector<int>{10, 10, 20});
}

TEST(SinglyLinkedListAdd, DuplicateAtTailAppendsToEnd) {
    SinglyLinkedList<int> list;
    list.add(10);
    const SingleNode<int> *first = list.add(20);
    const SingleNode<int> *second = list.add(20);

    EXPECT_EQ(first, list.getHead()->getNext());
    EXPECT_EQ(second, first->getNext());
    EXPECT_EQ(nullptr, second->getNext());
    expect_list(list, std::vector<int>{10, 20, 20});
}

TEST(SinglyLinkedListAdd, NegativeValues) {
    SinglyLinkedList<int> list = make_list({0, -5, 5, -10});
    expect_list(list, std::vector<int>{-10, -5, 0, 5});
}

TEST(SinglyLinkedListAdd, ManyItems) {
    SinglyLinkedList<int> list;
    std::vector<int> expected;
    for (int value = 999; value >= 0; --value) {
        list.add(value);
    }
    for (int value = 0; value < 1000; ++value) {
        expected.push_back(value);
    }
    expect_list(list, expected);
}

// ---------------------------------------------------------------------
// add (SingleNode<T>*) 및 addAll
// ---------------------------------------------------------------------

TEST(SinglyLinkedListAddNode, NullNodeReturnsNullptrAndChangesNothing) {
    SinglyLinkedList<int> list = make_list({1, 2});
    EXPECT_EQ(nullptr, list.add(static_cast<SingleNode<int> *>(nullptr)));
    expect_list(list, std::vector<int>{1, 2});
}

TEST(SinglyLinkedListAddNode, SingleNodeIntoEmptyList) {
    SinglyLinkedList<int> list;
    auto *node = new SingleNode<int>(10);
    EXPECT_EQ(node, list.add(node));
    EXPECT_EQ(node, list.getHead());
    EXPECT_EQ(nullptr, node->getNext());
    expect_list(list, std::vector<int>{10});
}

TEST(SinglyLinkedListAddNode, SingleNodeInsertedAtHeadMiddleAndTail) {
    SinglyLinkedList<int> list = make_list({10, 30});
    auto *headNode = new SingleNode<int>(5);
    auto *midNode = new SingleNode<int>(20);
    auto *tailNode = new SingleNode<int>(40);

    EXPECT_EQ(headNode, list.add(headNode));
    EXPECT_EQ(midNode, list.add(midNode));
    EXPECT_EQ(tailNode, list.add(tailNode));
    expect_list(list, std::vector<int>{5, 10, 20, 30, 40});
}

TEST(SinglyLinkedListAddNode, UnsortedChainAddsAllNodesIntoEmptyList) {
    SinglyLinkedList<int> list;
    SingleNode<int> *chain = make_linear_chain({30, 10, 20});

    EXPECT_NE(nullptr, list.add(chain));
    expect_list(list, std::vector<int>{10, 20, 30});
}

TEST(SinglyLinkedListAddNode, UnsortedChainAddsAllNodesIntoNonEmptyListInOrder) {
    SinglyLinkedList<int> list = make_list({10, 40});
    SingleNode<int> *chain = make_linear_chain({30, 5, 20, 50});

    EXPECT_NE(nullptr, list.add(chain));
    expect_list(list, std::vector<int>{5, 10, 20, 30, 40, 50});
}

TEST(SinglyLinkedListAddNode, ChainWithDuplicatesPreservesAllNodes) {
    SinglyLinkedList<int> list = make_list({10, 20});
    SingleNode<int> *chain = make_linear_chain({20, 10, 20});

    EXPECT_NE(nullptr, list.add(chain));
    expect_list(list, std::vector<int>{10, 10, 20, 20, 20});
}

// ---------------------------------------------------------------------
// find
// ---------------------------------------------------------------------

TEST(SinglyLinkedListFind, SingleNode) {
    SinglyLinkedList<int> list = make_list({7});
    EXPECT_EQ(list.getHead(), list.find(7));
    EXPECT_EQ(nullptr, list.find(8));
}

TEST(SinglyLinkedListFind, FindsExistingItems) {
    SinglyLinkedList<int> list = make_list({10, 20, 30, 40});
    for (const int value: {10, 20, 30, 40}) {
        SingleNode<int> *node = list.find(value);
        ASSERT_NE(nullptr, node) << "value: " << value;
        EXPECT_EQ(value, node->getItem());
    }
}

TEST(SinglyLinkedListFind, ReturnsNodeInsideTheList) {
    SinglyLinkedList<int> list = make_list({10, 20, 30});
    EXPECT_EQ(list.getHead(), list.find(10));
    EXPECT_EQ(list.getHead()->getNext(), list.find(20));
    EXPECT_EQ(list.getHead()->getNext()->getNext(), list.find(30));
}

TEST(SinglyLinkedListFind, MissingItemsReturnNullptr) {
    SinglyLinkedList<int> list = make_list({10, 20, 30});
    EXPECT_EQ(nullptr, list.find(5)); // 맨 앞보다 작음
    EXPECT_EQ(nullptr, list.find(15)); // 사이
    EXPECT_EQ(nullptr, list.find(35)); // 맨 뒤보다 큼
}

TEST(SinglyLinkedListFind, FindDoesNotChangeList) {
    SinglyLinkedList<int> list = make_list({10, 20, 30});
    list.find(20);
    list.find(99);
    expect_list(list, std::vector<int>{10, 20, 30});
}

TEST(SinglyLinkedListFind, DuplicatesReturnFirstOne) {
    SinglyLinkedList<int> list = make_list({1, 2, 2, 2, 3});
    SingleNode<int> *node = list.find(2);
    ASSERT_NE(nullptr, node);
    EXPECT_EQ(list.getHead()->getNext(), node);
}

TEST(SinglyLinkedListFind, DuplicatesAtHeadAndTailReturnFirstOne) {
    SinglyLinkedList<int> list;
    const SingleNode<int> *headFirst = list.add(1);
    list.add(1);
    const SingleNode<int> *tailFirst = list.add(3);
    list.add(3);

    EXPECT_EQ(headFirst, list.find(1));
    EXPECT_EQ(tailFirst, list.find(3));
}

TEST(SinglyLinkedListFind, FindModifiedNodeIsVisibleInList) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    SingleNode<int> *node = list.find(2);
    ASSERT_NE(nullptr, node);
    node->getItem() = 2; // 같은 값으로 갱신해도 리스트는 그대로여야 한다
    expect_list(list, std::vector<int>{1, 2, 3});
}

// ---------------------------------------------------------------------
// remove
// ---------------------------------------------------------------------

TEST(SinglyLinkedListRemove, OnlyElement) {
    SinglyLinkedList<int> list = make_list({7});
    EXPECT_TRUE(list.remove(7));
    expect_list(list, std::vector<int>{});
}

TEST(SinglyLinkedListRemove, OnlyElementNotMatching) {
    SinglyLinkedList<int> list = make_list({7});
    EXPECT_FALSE(list.remove(8));
    expect_list(list, std::vector<int>{7});
}

TEST(SinglyLinkedListRemove, Head) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    EXPECT_TRUE(list.remove(1));
    expect_list(list, std::vector<int>{2, 3});
    EXPECT_EQ(2, list.getHead()->getItem());
}

TEST(SinglyLinkedListRemove, Middle) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    EXPECT_TRUE(list.remove(2));
    expect_list(list, std::vector<int>{1, 3});
}

TEST(SinglyLinkedListRemove, Tail) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    EXPECT_TRUE(list.remove(3));
    expect_list(list, std::vector<int>{1, 2});
}

TEST(SinglyLinkedListRemove, TwoNodesRemoveHeadThenTail) {
    SinglyLinkedList<int> list = make_list({1, 2});
    EXPECT_TRUE(list.remove(1));
    expect_list(list, std::vector<int>{2});
    EXPECT_EQ(nullptr, list.getHead()->getNext());
    EXPECT_TRUE(list.remove(2));
    expect_list(list, std::vector<int>{});
}

TEST(SinglyLinkedListRemove, TwoNodesRemoveTailThenHead) {
    SinglyLinkedList<int> list = make_list({1, 2});
    EXPECT_TRUE(list.remove(2));
    expect_list(list, std::vector<int>{1});
    EXPECT_EQ(nullptr, list.getHead()->getNext());
    EXPECT_TRUE(list.remove(1));
    expect_list(list, std::vector<int>{});
}

TEST(SinglyLinkedListRemove, MissingItemChangesNothing) {
    SinglyLinkedList<int> list = make_list({10, 20, 30});
    EXPECT_FALSE(list.remove(5));
    EXPECT_FALSE(list.remove(15));
    EXPECT_FALSE(list.remove(35));
    expect_list(list, std::vector<int>{10, 20, 30});
}

TEST(SinglyLinkedListRemove, DuplicateRemovesOnlyOne) {
    SinglyLinkedList<int> list = make_list({1, 2, 2, 2, 3});
    EXPECT_TRUE(list.remove(2));
    expect_list(list, std::vector<int>{1, 2, 2, 3});
    EXPECT_TRUE(list.remove(2));
    EXPECT_TRUE(list.remove(2));
    EXPECT_FALSE(list.remove(2));
    expect_list(list, std::vector<int>{1, 3});
}

TEST(SinglyLinkedListRemove, DuplicateRemovesFirstOne) {
    SinglyLinkedList<int> list;
    list.add(1);
    list.add(2);
    const SingleNode<int> *secondTwo = list.add(2);
    const SingleNode<int> *thirdTwo = list.add(2);
    list.add(3);

    EXPECT_TRUE(list.remove(2));
    EXPECT_EQ(secondTwo, list.find(2));
    EXPECT_EQ(thirdTwo, secondTwo->getNext());
    expect_list(list, std::vector<int>{1, 2, 2, 3});
}

TEST(SinglyLinkedListRemove, DuplicateAtHeadMovesHeadToSecondDuplicate) {
    SinglyLinkedList<int> list;
    list.add(1);
    const SingleNode<int> *secondOne = list.add(1);
    list.add(2);

    EXPECT_TRUE(list.remove(1));
    EXPECT_EQ(secondOne, list.getHead());
    expect_list(list, std::vector<int>{1, 2});
}

TEST(SinglyLinkedListRemove, RemovingSameItemTwiceReturnsFalseSecondTime) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    EXPECT_TRUE(list.remove(2));
    EXPECT_FALSE(list.remove(2));
    expect_list(list, std::vector<int>{1, 3});
}

TEST(SinglyLinkedListRemove, RemoveEverythingOneByOne) {
    SinglyLinkedList<int> list = make_list({4, 2, 5, 1, 3});
    for (const int value: {3, 1, 5, 2, 4}) {
        EXPECT_TRUE(list.remove(value)) << "value: " << value;
    }
    expect_list(list, std::vector<int>{});
    list.add(8);
    expect_list(list, std::vector<int>{8});
}

TEST(SinglyLinkedListRemove, RemoveHeadRepeatedly) {
    SinglyLinkedList<int> list = make_list({4, 2, 5, 1, 3});
    for (const int value: {1, 2, 3, 4, 5}) {
        EXPECT_TRUE(list.remove(value)) << "value: " << value;
    }
    expect_list(list, std::vector<int>{});
}

TEST(SinglyLinkedListRemove, RemoveTailRepeatedly) {
    SinglyLinkedList<int> list = make_list({4, 2, 5, 1, 3});
    for (const int value: {5, 4, 3, 2, 1}) {
        EXPECT_TRUE(list.remove(value)) << "value: " << value;
    }
    expect_list(list, std::vector<int>{});
}

TEST(SinglyLinkedListRemove, ListCanBeReusedAfterBecomingEmpty) {
    SinglyLinkedList<int> list = make_list({1});
    list.remove(1);
    list.add(9);
    list.add(3);
    expect_list(list, std::vector<int>{3, 9});
}

// ---------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------

TEST(SinglyLinkedListClear, EmptiesList) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    list.clear();
    expect_list(list, std::vector<int>{});
    EXPECT_EQ(nullptr, list.find(1));
}

TEST(SinglyLinkedListClear, SingleNode) {
    SinglyLinkedList<int> list = make_list({1});
    list.clear();
    expect_list(list, std::vector<int>{});
}

TEST(SinglyLinkedListClear, ListCanBeReusedAfterClear) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    list.clear();
    list.add(5);
    list.add(4);
    expect_list(list, std::vector<int>{4, 5});
}

TEST(SinglyLinkedListClear, ClearTwiceIsSafe) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    list.clear();
    list.clear();
    expect_list(list, std::vector<int>{});
}

TEST(SinglyLinkedListClear, DestructorAfterClearIsSafe) {
    // clear() 가 head 를 nullptr 로 만들지 않으면 소멸자에서 이중 해제가 일어난다 (ASan 으로 확인)
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    list.clear();
}

TEST(SinglyLinkedListClear, DestructorOfNonEmptyListFreesAllNodes) {
    // 마지막 노드까지 누수 없이 해제하는지 (LSan 으로 확인)
    SinglyLinkedList<int> list = make_list({1, 2, 3, 4, 5});
}

// ---------------------------------------------------------------------
// 이동 생성 / 이동 대입
// ---------------------------------------------------------------------

TEST(SinglyLinkedListMove, MoveConstructor) {
    SinglyLinkedList<int> source = make_list({3, 1, 2});
    SinglyLinkedList<int> target(std::move(source));
    expect_list(target, std::vector<int>{1, 2, 3});
    expect_list(source, std::vector<int>{});
}

TEST(SinglyLinkedListMove, MoveConstructorFromEmptyList) {
    SinglyLinkedList<int> source;
    SinglyLinkedList<int> target(std::move(source));
    expect_list(target, std::vector<int>{});
    expect_list(source, std::vector<int>{});
}

TEST(SinglyLinkedListMove, MoveAssignmentReplacesOldContent) {
    SinglyLinkedList<int> source = make_list({3, 1, 2});
    SinglyLinkedList<int> target = make_list({100, 200});
    target = std::move(source);
    expect_list(target, std::vector<int>{1, 2, 3});
    expect_list(source, std::vector<int>{});
}

TEST(SinglyLinkedListMove, MoveAssignmentFromEmptyClearsTarget) {
    SinglyLinkedList<int> source;
    SinglyLinkedList<int> target = make_list({1, 2});
    target = std::move(source);
    expect_list(target, std::vector<int>{});
    expect_list(source, std::vector<int>{});
}

TEST(SinglyLinkedListMove, MoveAssignmentIntoEmptyTarget) {
    SinglyLinkedList<int> source = make_list({3, 1, 2});
    SinglyLinkedList<int> target;
    target = std::move(source);
    expect_list(target, std::vector<int>{1, 2, 3});
    expect_list(source, std::vector<int>{});
}

TEST(SinglyLinkedListMove, SelfMoveAssignmentKeepsContent) {
    SinglyLinkedList<int> list = make_list({1, 2, 3});
    SinglyLinkedList<int> &alias = list;
    list = std::move(alias);
    expect_list(list, std::vector<int>{1, 2, 3});
}

TEST(SinglyLinkedListMove, MovedFromListIsReusable) {
    SinglyLinkedList<int> source = make_list({1, 2, 3});
    SinglyLinkedList<int> target(std::move(source));
    source.add(7);
    expect_list(source, std::vector<int>{7});
    expect_list(target, std::vector<int>{1, 2, 3});
}

// ---------------------------------------------------------------------
// 다른 타입
// ---------------------------------------------------------------------

TEST(SinglyLinkedListGeneric, MyStringItems) {
    SinglyLinkedList<MyString> list;
    list.add(MyString("banana"));
    list.add(MyString("apple"));
    list.add(MyString("cherry"));
    list.add(MyString("apple"));

    std::vector<std::string> actual;
    for (SingleNode<MyString> *now = list.getHead(); now != nullptr; now = now->getNext()) {
        std::ostringstream stream;
        stream << now->getItem();
        actual.push_back(stream.str());
    }
    EXPECT_EQ((std::vector<std::string>{"apple", "apple", "banana", "cherry"}), actual);
    EXPECT_EQ(4, list.getSize());
    EXPECT_NE(nullptr, list.find(MyString("banana")));
    EXPECT_EQ(nullptr, list.find(MyString("durian")));
    EXPECT_TRUE(list.remove(MyString("apple")));
    EXPECT_EQ(3, list.getSize());
}

TEST(SinglyLinkedListGeneric, DoubleItems) {
    SinglyLinkedList<double> list;
    list.add(2.5);
    list.add(-1.5);
    list.add(0.0);
    expect_list(list, std::vector<double>{-1.5, 0.0, 2.5});
}

// ---------------------------------------------------------------------
// std::multiset 과 무작위 연산 비교 (차분 테스트)
// ---------------------------------------------------------------------

TEST(SinglyLinkedListDifferential, RandomOperationsMatchMultiset) {
    std::mt19937 generator(20261119);
    SinglyLinkedList<int> list;
    std::multiset<int> expected;

    for (int round = 0; round < 6000; ++round) {
        const int op = static_cast<int>(generator() % 4);
        const int value = static_cast<int>(generator() % 40);
        SCOPED_TRACE(
            "round: " + std::to_string(round) + ", op: " + std::to_string(op) + ", value: " + std::to_string(value));

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
                SingleNode<int> *node = list.find(value);
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
