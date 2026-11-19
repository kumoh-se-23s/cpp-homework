#include <gtest/gtest.h>

#include <string>

#include "RowTable.h"
#include "ColumnTable.h"

namespace {
    struct Item {
        int id;
        MyString team;
    };

    struct ItemTraits {
        static int row_key(const Item &item) {
            return item.id / 100;
        }

        static MyString column_key(const Item &item) {
            return item.team;
        }

        static bool has_column(const Item &item) {
            return item.team != "-";
        }

        static bool less(const Item &left, const Item &right) {
            return left.id < right.id;
        }

        static bool same(const Item &left, const Item &right) {
            return left.id == right.id;
        }
    };

    using Node = MultiConnectNode<Item>;

    std::string dump_rows(const RowTable<Item, ItemTraits> &table) {
        std::string result;
        table.for_each([&result](const Node *node) {
            result += std::to_string(node->get().id) + " ";
        });
        return result;
    }

    std::string dump_mates(const ColumnTable<Item, ItemTraits> &table, const Node *node) {
        std::string result;
        table.for_each(node, [&result](const Node *mate) {
            result += std::to_string(mate->get().id) + " ";
        });
        return result;
    }
}

TEST(RowTableTest, ForEachVisitsBucketsInAscendingOrder) {
    Node nodes[4] = {Node(Item{1205, "-"}), Node(Item{1001, "-"}), Node(Item{1203, "-"}), Node(Item{1100, "-"})};
    RowTable<Item, ItemTraits> table(10, 12);
    for (Node &node : nodes) {
        table.link(&node);
    }

    EXPECT_EQ(dump_rows(table), "1001 1100 1203 1205 ");
    EXPECT_EQ(table.min_key(), 10);
    EXPECT_EQ(table.max_key(), 12);
}

TEST(RowTableTest, GrowsWhenKeyIsOutOfRange) {
    Node nodes[3] = {Node(Item{1001, "-"}), Node(Item{805, "-"}), Node(Item{1502, "-"})};
    RowTable<Item, ItemTraits> table(10, 10);
    for (Node &node : nodes) {
        table.link(&node);
    }

    EXPECT_EQ(dump_rows(table), "805 1001 1502 ");
    EXPECT_EQ(table.min_key(), 8);
    EXPECT_EQ(table.max_key(), 15);
}

TEST(RowTableTest, MinMaxKeyShrinkAfterUnlink) {
    Node nodes[3] = {Node(Item{1001, "-"}), Node(Item{1101, "-"}), Node(Item{1201, "-"})};
    RowTable<Item, ItemTraits> table(10, 12);
    for (Node &node : nodes) {
        table.link(&node);
    }

    table.unlink(&nodes[0]);
    table.unlink(&nodes[2]);

    EXPECT_EQ(table.min_key(), 11);
    EXPECT_EQ(table.max_key(), 11);
}

TEST(RowTableTest, FindIfSearchesOnlyThatBucket) {
    Node node(Item{1001, "-"});
    RowTable<Item, ItemTraits> table(10, 12);
    table.link(&node);

    auto is_1001 = [](const Node *current_node) {
        return current_node->get().id == 1001;
    };

    EXPECT_EQ(table.find_if(10, is_1001), &node);
    EXPECT_EQ(table.find_if(11, is_1001), nullptr);
    EXPECT_EQ(table.find_if(99, is_1001), nullptr);
}

TEST(ColumnTableTest, MatesAreVisitedCircularly) {
    Node nodes[3] = {Node(Item{3, "A"}), Node(Item{1, "A"}), Node(Item{2, "A"})};
    ColumnTable<Item, ItemTraits> table;
    for (Node &node : nodes) {
        table.link(&node);
    }

    EXPECT_EQ(table.count(), 1);
    EXPECT_EQ(dump_mates(table, &nodes[2]), "3 1 ");
}

TEST(ColumnTableTest, NodeWithoutColumnIsIgnored) {
    Node node(Item{1, "-"});
    ColumnTable<Item, ItemTraits> table;
    table.link(&node);

    EXPECT_EQ(table.count(), 0);
    EXPECT_EQ(dump_mates(table, &node), "");
    table.unlink(&node);
}

TEST(ColumnTableTest, EntryIsRemovedWhenRingBecomesEmpty) {
    Node nodes[3] = {Node(Item{1, "A"}), Node(Item{2, "B"}), Node(Item{3, "B"})};
    ColumnTable<Item, ItemTraits> table;
    for (Node &node : nodes) {
        table.link(&node);
    }

    EXPECT_EQ(table.count(), 2);

    table.unlink(&nodes[0]);
    EXPECT_EQ(table.count(), 1);

    table.unlink(&nodes[1]);
    EXPECT_EQ(table.count(), 1);
    EXPECT_EQ(dump_mates(table, &nodes[2]), "");
}
