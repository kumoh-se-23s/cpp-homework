#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "DoubleNode.h"
#include "OOldMyString.h"
#include "NestedDoubleNode.h"
#include "OldNode.h"
#include "OldSingleNode.h"
#include "Student.h"

namespace {
    std::string to_std_string(const OOldMyString &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    template <typename T>
    class TestNode : public OldNode<T> {
    public:
        explicit TestNode(const T &item) : OldNode<T>(item) {}
        TestNode(const TestNode &other) = delete;
        TestNode &operator=(const TestNode &other) = delete;
        TestNode(TestNode &&other) noexcept = default;
        TestNode &operator=(TestNode &&other) noexcept = default;
    };

    struct PrintableItem {
        int value;
        OOldMyString label;

        OOldMyString toString() const {
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
    EXPECT_FALSE((std::is_constructible_v<OldNode<int>, int>));
    EXPECT_FALSE((std::is_default_constructible_v<OldNode<int>>));
    EXPECT_FALSE((std::is_copy_constructible_v<OldNode<int>>));
    EXPECT_FALSE((std::is_copy_assignable_v<OldNode<int>>));
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
    TestNode<OOldMyString> original("a string long enough to need a bigger buffer");
    TestNode<OOldMyString> moved(std::move(original));
    EXPECT_EQ("a string long enough to need a bigger buffer", to_std_string(moved.getItem()));

    TestNode<OOldMyString> target("initial");
    target = std::move(moved);
    EXPECT_EQ("a string long enough to need a bigger buffer", to_std_string(target.getItem()));

    TestNode<OOldMyString> &alias = target;
    target = std::move(alias);
    EXPECT_EQ("a string long enough to need a bigger buffer", to_std_string(target.getItem()));
}

TEST(NodeBasic, DeleteThroughBasePointerIsSafe) {
    OldNode<int> *single = new OldSingleNode<int>(1);
    OldNode<int> *doubled = new DoubleNode<int>(2);
    delete single;
    delete doubled;
}

TEST(NodeToString, DelegatesToItemToString) {
    OldSingleNode<PrintableItem> single(PrintableItem{1, OOldMyString("alpha")});
    DoubleNode<PrintableItem> doubled(PrintableItem{2, OOldMyString("beta")});

    EXPECT_EQ("alpha", to_std_string(single.toString()));
    EXPECT_EQ("beta", to_std_string(doubled.toString()));
}

// ---------------------------------------------------------------------
// SingleNode
// ---------------------------------------------------------------------

TEST(SingleNodeBasic, IsNotCopyable) {
    EXPECT_FALSE((std::is_copy_constructible_v<OldSingleNode<int>>));
    EXPECT_FALSE((std::is_copy_assignable_v<OldSingleNode<int>>));
}

TEST(SingleNodeBasic, ConstructAndSetNext) {
    OldSingleNode<int> second(2);
    OldSingleNode<int> first(1, &second);
    EXPECT_EQ(1, first.getItem());
    EXPECT_EQ(&second, first.getNext());
    EXPECT_EQ(nullptr, second.getNext());

    first.setNext(nullptr);
    EXPECT_EQ(nullptr, first.getNext());
}

TEST(SingleNodeBasic, MoveConstructorAndMoveAssignment) {
    OldSingleNode<OOldMyString> next("next");
    OldSingleNode<OOldMyString> original("original value long enough to allocate", &next);
    OldSingleNode<OOldMyString> moved(std::move(original));
    EXPECT_EQ("original value long enough to allocate", to_std_string(moved.getItem()));
    EXPECT_EQ(&next, moved.getNext());
    EXPECT_EQ(nullptr, original.getNext());

    OldSingleNode<OOldMyString> target("target");
    target = std::move(moved);
    EXPECT_EQ("original value long enough to allocate", to_std_string(target.getItem()));
    EXPECT_EQ(&next, target.getNext());
}

TEST(SingleNodeBasic, ChainTraversal) {
    OldSingleNode<int> third(3);
    OldSingleNode<int> second(2, &third);
    OldSingleNode<int> first(1, &second);
    int expected = 1;
    for (const OldSingleNode<int> *now = &first; now != nullptr; now = now->getNext()) {
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
    DoubleNode<OOldMyString> prev("prev");
    DoubleNode<OOldMyString> next("next");
    DoubleNode<OOldMyString> original("original value long enough to allocate", &prev, &next);
    DoubleNode<OOldMyString> moved(std::move(original));
    EXPECT_EQ("original value long enough to allocate", to_std_string(moved.getItem()));
    EXPECT_EQ(&prev, moved.getPrev());
    EXPECT_EQ(&next, moved.getNext());
    EXPECT_EQ(nullptr, original.getPrev());
    EXPECT_EQ(nullptr, original.getNext());

    DoubleNode<OOldMyString> target("target");
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
    EXPECT_EQ(&inner, nested.getNode());
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
    DoubleNode<OOldMyString> inner(OOldMyString("shared node value"));
    NestedDoubleNode<OOldMyString> original(&inner);
    NestedDoubleNode<OOldMyString> copy(original);
    NestedDoubleNode<OOldMyString> moved(std::move(original));

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
    OldSingleNode<NestedDoubleNode<int>> second{NestedDoubleNode<int>(&inner2)};
    OldSingleNode<NestedDoubleNode<int>> first{NestedDoubleNode<int>(&inner1), &second};

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

TEST(NestedDoubleNodeToString, SingleCircularNodeDelegatesToOwnItem) {
    DoubleNode<PrintableItem> only(PrintableItem{1, OOldMyString("solo")});
    only.setNext(&only);
    only.setPrev(&only);

    const NestedDoubleNode<PrintableItem> nested(&only);
    EXPECT_EQ("solo", to_std_string(nested.toString()));
}

TEST(NestedDoubleNodeToString, CircularGroupDelegatesOnlyToPointedNode) {
    DoubleNode<PrintableItem> a(PrintableItem{1, OOldMyString("Alice")});
    DoubleNode<PrintableItem> b(PrintableItem{2, OOldMyString("Bob")});
    DoubleNode<PrintableItem> c(PrintableItem{3, OOldMyString("Charlie")});

    a.setNext(&b); b.setPrev(&a);
    b.setNext(&c); c.setPrev(&b);
    c.setNext(&a); a.setPrev(&c);

    const NestedDoubleNode<PrintableItem> nested(&b);
    const std::string output = to_std_string(nested.toString());

    EXPECT_EQ("Bob", output);
    EXPECT_EQ(std::string::npos, output.find("Alice"));
    EXPECT_EQ(std::string::npos, output.find("Charlie"));
}

TEST(NestedDoubleNodeToString, NullNodeReturnsEmptyString) {
    const NestedDoubleNode<PrintableItem> nested(nullptr);
    EXPECT_EQ(nullptr, nested.getNode());
    EXPECT_EQ("", to_std_string(nested.toString()));
}