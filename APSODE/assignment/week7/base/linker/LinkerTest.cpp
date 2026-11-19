#include <gtest/gtest.h>

#include "Linker.h"
#include "RowLinkPolicy.h"
#include "ColumnLinkPolicy.h"
#include "NodeCompare.h"

namespace {
    struct IntTraits {
        static bool less(const int &left, const int &right) {
            return left < right;
        }
    };

    using Node = MultiConnectNode<int>;
    using Chain = Linker<Node, RowLinkPolicy<int>, NodeCompare<int, IntTraits> >;
    using Ring = Linker<Node, ColumnLinkPolicy<int>, NodeCompare<int, IntTraits> >;

    template<typename L>
    std::string dump(const L &linker) {
        std::string result;
        linker.for_each([&result](const Node *node) {
            result += std::to_string(node->get()) + " ";
        });
        return result;
    }

    template<typename L>
    std::string dump_after(const L &linker, const Node *start) {
        std::string result;
        linker.for_each(start, [&result](const Node *node) {
            result += std::to_string(node->get()) + " ";
        });
        return result;
    }
}

template<typename L>
class LinkerTest : public ::testing::Test {
    protected:
        Node nodes[5] = {Node(3), Node(1), Node(4), Node(1), Node(5)};
        L linker;

        void insert_all() {
            for (Node &node : this->nodes) {
                this->linker.insert(&node);
            }
        }
};

using LinkerTypes = ::testing::Types<Chain, Ring>;
TYPED_TEST_SUITE(LinkerTest, LinkerTypes);

TYPED_TEST(LinkerTest, InsertSortedKeepsAscendingAndStableOrder) {
    this->insert_all();

    EXPECT_EQ(dump(this->linker), "1 1 3 4 5 ");
    EXPECT_EQ(this->linker.size(), 5);
    EXPECT_EQ(this->linker.head(), &this->nodes[1]);
}

TYPED_TEST(LinkerTest, RemoveHeadTailAndMiddle) {
    this->insert_all();

    this->linker.remove(&this->nodes[1]);
    this->linker.remove(&this->nodes[4]);
    this->linker.remove(&this->nodes[0]);

    EXPECT_EQ(dump(this->linker), "1 4 ");
    EXPECT_EQ(this->linker.size(), 2);
    EXPECT_FALSE(this->linker.contains(&this->nodes[0]));
    EXPECT_TRUE(this->linker.contains(&this->nodes[2]));
}

TYPED_TEST(LinkerTest, RemoveLastNodeEmptiesLinker) {
    this->linker.insert(&this->nodes[0]);
    this->linker.remove(&this->nodes[0]);

    EXPECT_TRUE(this->linker.empty());
    EXPECT_EQ(this->linker.head(), nullptr);
}

TYPED_TEST(LinkerTest, FindIfReturnsFirstMatch) {
    this->insert_all();

    Node *found = this->linker.find_if([](const Node *node) {
        return node->get() > 3;
    });

    EXPECT_EQ(found, &this->nodes[2]);
}

TYPED_TEST(LinkerTest, ClearResetsAllLinks) {
    this->insert_all();
    this->linker.clear();

    EXPECT_TRUE(this->linker.empty());
    for (const Node &node : this->nodes) {
        EXPECT_EQ(node.row_next(), nullptr);
        EXPECT_EQ(node.column_next(), nullptr);
        EXPECT_EQ(node.column_prev(), nullptr);
    }
}

TYPED_TEST(LinkerTest, TakeMovesAllNodes) {
    this->insert_all();
    TypeParam other;
    other.take(this->linker);

    EXPECT_TRUE(this->linker.empty());
    EXPECT_EQ(dump(other), "1 1 3 4 5 ");
    other.clear();
}

TEST(RowChainTest, ForEachAfterVisitsUntilTail) {
    Node nodes[3] = {Node(1), Node(2), Node(3)};
    Chain chain;
    for (Node &node : nodes) {
        chain.insert(&node);
    }

    EXPECT_EQ(dump_after(chain, &nodes[1]), "3 ");
    chain.clear();
}

TEST(RowChainTest, RemoveUnlinkedNodeThrows) {
    Node linked(1);
    Node unlinked(2);
    Chain chain;
    chain.insert(&linked);

    EXPECT_THROW(chain.remove(&unlinked), std::invalid_argument);
    chain.clear();
}

TEST(ColumnRingTest, ForEachAfterWrapsAround) {
    Node nodes[3] = {Node(1), Node(2), Node(3)};
    Ring ring;
    for (Node &node : nodes) {
        ring.insert(&node);
    }

    EXPECT_EQ(dump_after(ring, &nodes[1]), "3 1 ");
    EXPECT_EQ(ring.head()->column_prev(), &nodes[2]);
    ring.clear();
}

TEST(ColumnRingTest, SingleNodePointsToItself) {
    Node node(1);
    Ring ring;
    ring.insert(&node);

    EXPECT_EQ(node.column_next(), &node);
    EXPECT_EQ(node.column_prev(), &node);
    EXPECT_EQ(dump_after(ring, &node), "");
    ring.clear();
}
