#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "DoubleNode.h"
#include "MyString.h"
#include "NestedDoubleNode.h"
#include "Node.h"
#include "SingleNode.h"
#include "Student.h"

namespace {
    std::string to_std_string(const MyString &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    template <typename T>
    class TestNode : public Node<T> {
    public:
        explicit TestNode(const T &item) : Node<T>(item) {}
        TestNode(const TestNode &other) = delete;
        TestNode &operator=(const TestNode &other) = delete;
        TestNode(TestNode &&other) noexcept = default;
        TestNode &operator=(TestNode &&other) noexcept = default;
    };

    struct PrintableItem {
        int value;
        MyString label;

        MyString toString() const {
            return label;
        }
        bool operator==(const PrintableItem &other) const { return value == other.value; }
        bool operator!=(const PrintableItem &other) const { return value != other.value; }
        bool operator<(const PrintableItem &other) const { return value < other.value; }
        bool operator>(const PrintableItem &other) const { return value > other.value; }
        bool operator<=(const PrintableItem &other) const { return value <= other.value; }
        bool operator>=(const PrintableItem &other) const { return value >= other.value; }
    };
}

// ---------------------------------------------------------------------
// Node
// ---------------------------------------------------------------------

TEST(NodeBasic, CannotInstantiateDirectlyAndNotCopyable) {
    EXPECT_FALSE((std::is_constructible_v<Node<int>, int>));
    EXPECT_FALSE((std::is_default_constructible_v<Node<int>>));
    EXPECT_FALSE((std::is_copy_constructible_v<Node<int>>));
    EXPECT_FALSE((std::is_copy_assignable_v<Node<int>>));
}

TEST(NodeBasic, GetItemAndSetItem) {
    TestNode<int> node(42);
    EXPECT_EQ(42, node.getItem());

    node.getItem() = 99;
    EXPECT_EQ(99, node.getItem());

    int value = 7;
    node.setItem(value);
    EXPECT_EQ(7, node.getItem());
}

TEST(NodeBasic, MoveConstructorAndMoveAssignment) {
    TestNode<MyString> original("a string long enough to need a bigger buffer");
    TestNode<MyString> moved(std::move(original));
    EXPECT_EQ("a string long enough to need a bigger buffer", to_std_string(moved.getItem()));

    TestNode<MyString> target("initial");
    target = std::move(moved);
    EXPECT_EQ("a string long enough to need a bigger buffer", to_std_string(target.getItem()));

    TestNode<MyString> &alias = target;
    target = std::move(alias);
    EXPECT_EQ("a string long enough to need a bigger buffer", to_std_string(target.getItem()));
}

TEST(NodeBasic, DeleteThroughBasePointerIsSafe) {
    Node<int> *single = new SingleNode<int>(1);
    Node<int> *doubled = new DoubleNode<int>(2);
    delete single;
    delete doubled;
}

TEST(NodeToString, DelegatesToItemToString) {
    SingleNode<PrintableItem> single(PrintableItem{1, MyString("alpha")});
    DoubleNode<PrintableItem> doubled(PrintableItem{2, MyString("beta")});

    EXPECT_EQ("alpha", to_std_string(single.toString()));
    EXPECT_EQ("beta", to_std_string(doubled.toString()));
}

// ---------------------------------------------------------------------
// SingleNode
// ---------------------------------------------------------------------

TEST(SingleNodeBasic, IsNotCopyable) {
    EXPECT_FALSE((std::is_copy_constructible_v<SingleNode<int>>));
    EXPECT_FALSE((std::is_copy_assignable_v<SingleNode<int>>));
}

TEST(SingleNodeBasic, ConstructAndSetNext) {
    SingleNode<int> second(2);
    SingleNode<int> first(1, &second);
    EXPECT_EQ(1, first.getItem());
    EXPECT_EQ(&second, first.getNext());
    EXPECT_EQ(nullptr, second.getNext());

    first.setNext(nullptr);
    EXPECT_EQ(nullptr, first.getNext());
}

TEST(SingleNodeBasic, MoveConstructorAndMoveAssignment) {
    SingleNode<MyString> next("next");
    SingleNode<MyString> original("original value long enough to allocate", &next);
    SingleNode<MyString> moved(std::move(original));
    EXPECT_EQ("original value long enough to allocate", to_std_string(moved.getItem()));
    EXPECT_EQ(&next, moved.getNext());
    EXPECT_EQ(nullptr, original.getNext());

    SingleNode<MyString> target("target");
    target = std::move(moved);
    EXPECT_EQ("original value long enough to allocate", to_std_string(target.getItem()));
    EXPECT_EQ(&next, target.getNext());
}

TEST(SingleNodeBasic, ChainTraversal) {
    SingleNode<int> third(3);
    SingleNode<int> second(2, &third);
    SingleNode<int> first(1, &second);
    int expected = 1;
    for (const SingleNode<int> *now = &first; now != nullptr; now = now->getNext()) {
        EXPECT_EQ(expected++, now->getItem());
    }
    EXPECT_EQ(4, expected);
}

// ---------------------------------------------------------------------
// DoubleNode
// ---------------------------------------------------------------------

TEST(DoubleNodeBasic, IsNotCopyable) {
    EXPECT_FALSE((std::is_copy_constructible_v<DoubleNode<int>>));
    EXPECT_FALSE((std::is_copy_assignable_v<DoubleNode<int>>));
}

TEST(DoubleNodeBasic, ConstructAndSetPrevNext) {
    DoubleNode<int> prev(1);
    DoubleNode<int> next(3);
    DoubleNode<int> node(2, &prev, &next);
    EXPECT_EQ(2, node.getItem());
    EXPECT_EQ(&prev, node.getPrev());
    EXPECT_EQ(&next, node.getNext());

    node.setPrev(nullptr);
    node.setNext(nullptr);
    EXPECT_EQ(nullptr, node.getPrev());
    EXPECT_EQ(nullptr, node.getNext());
}

TEST(DoubleNodeBasic, MoveConstructorAndMoveAssignment) {
    DoubleNode<MyString> prev("prev");
    DoubleNode<MyString> next("next");
    DoubleNode<MyString> original("original value long enough to allocate", &prev, &next);
    DoubleNode<MyString> moved(std::move(original));
    EXPECT_EQ("original value long enough to allocate", to_std_string(moved.getItem()));
    EXPECT_EQ(&prev, moved.getPrev());
    EXPECT_EQ(&next, moved.getNext());
    EXPECT_EQ(nullptr, original.getPrev());
    EXPECT_EQ(nullptr, original.getNext());

    DoubleNode<MyString> target("target");
    target = std::move(moved);
    EXPECT_EQ("original value long enough to allocate", to_std_string(target.getItem()));
    EXPECT_EQ(&prev, target.getPrev());
    EXPECT_EQ(&next, target.getNext());
}

// ---------------------------------------------------------------------
// NestedDoubleNode
// ---------------------------------------------------------------------

TEST(NestedDoubleNodeBasic, ConstructFromDoubleNodePointerAndUnravel) {
    DoubleNode<int> inner(42);
    NestedDoubleNode<int> nested(&inner);
    EXPECT_EQ(42, nested.getUnravelItem());
    EXPECT_EQ(&inner.getItem(), &nested.getUnravelItem());
}

TEST(NestedDoubleNodeBasic, ReflectsChangesToPointedNode) {
    DoubleNode<int> inner(10);
    NestedDoubleNode<int> nested(&inner);
    inner.getItem() = 99;
    EXPECT_EQ(99, nested.getUnravelItem());
}

TEST(NestedDoubleNodeBasic, CopyAndMoveShareSameTargetNode) {
    DoubleNode<MyString> inner(MyString("shared node value"));
    NestedDoubleNode<MyString> original(&inner);
    NestedDoubleNode<MyString> copy(original);
    NestedDoubleNode<MyString> moved(std::move(original));

    EXPECT_EQ("shared node value", to_std_string(copy.getUnravelItem()));
    EXPECT_EQ("shared node value", to_std_string(moved.getUnravelItem()));
    EXPECT_EQ(&inner.getItem(), &copy.getUnravelItem());
    EXPECT_EQ(&inner.getItem(), &moved.getUnravelItem());
}

TEST(NestedDoubleNodeBasic, DestructorDoesNotDeletePointedNode) {
    DoubleNode<int> inner(77);
    {
        NestedDoubleNode<int> nested(&inner);
        EXPECT_EQ(77, nested.getUnravelItem());
    }
    EXPECT_EQ(77, inner.getItem());
}

TEST(NestedDoubleNodeBasic, WorksInsideSingleNode) {
    DoubleNode<int> inner1(10);
    DoubleNode<int> inner2(20);
    SingleNode<NestedDoubleNode<int>> second{NestedDoubleNode<int>(&inner2)};
    SingleNode<NestedDoubleNode<int>> first{NestedDoubleNode<int>(&inner1), &second};

    EXPECT_EQ(10, first.getItem().getUnravelItem());
    EXPECT_EQ(20, first.getNext()->getItem().getUnravelItem());
    EXPECT_TRUE(first.getItem() < first.getNext()->getItem());
    EXPECT_TRUE(first.getNext()->getItem() > first.getItem());
    EXPECT_TRUE(first.getItem() != first.getNext()->getItem());
}

TEST(NestedDoubleNodeCompare, OrderingAndEquality) {
    DoubleNode<int> nSmall(1);
    DoubleNode<int> nSame(1);
    DoubleNode<int> nBig(2);
    const NestedDoubleNode<int> small(&nSmall);
    const NestedDoubleNode<int> same(&nSame);
    const NestedDoubleNode<int> big(&nBig);

    EXPECT_TRUE(small == same);
    EXPECT_FALSE(small != same);
    EXPECT_TRUE(small != big);
    EXPECT_TRUE(small < big);
    EXPECT_TRUE(big > small);
    EXPECT_TRUE(small <= same);
    EXPECT_TRUE(small >= same);
}

TEST(NestedDoubleNodeToString, SingleCircularNodePrintsOnlySelf) {
    DoubleNode<PrintableItem> only(PrintableItem{1, MyString("solo")});
    only.setNext(&only);
    only.setPrev(&only);

    const NestedDoubleNode<PrintableItem> nested(&only);
    const std::string output = to_std_string(nested.toString());
    EXPECT_NE(std::string::npos, output.find("solo"));
}

TEST(NestedDoubleNodeToString, CircularGroupPrintsAllMembersWithoutInfiniteLoop) {
    DoubleNode<PrintableItem> a(PrintableItem{1, MyString("Alice")});
    DoubleNode<PrintableItem> b(PrintableItem{2, MyString("Bob")});
    DoubleNode<PrintableItem> c(PrintableItem{3, MyString("Charlie")});

    a.setNext(&b); b.setPrev(&a);
    b.setNext(&c); c.setPrev(&b);
    c.setNext(&a); a.setPrev(&c);

    const NestedDoubleNode<PrintableItem> nested(&b);
    const std::string output = to_std_string(nested.toString());

    EXPECT_NE(std::string::npos, output.find("Bob"));
    EXPECT_NE(std::string::npos, output.find("Charlie"));
    EXPECT_NE(std::string::npos, output.find("Alice"));
}