#include <gtest/gtest.h>

#include <string>

#include "MultiLinkedList.h"

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

    using List = MultiLinkedList<Item, ItemTraits>;

    std::string dump(const List &list) {
        std::string result;
        list.for_each([&result](const Item &item) {
            result += std::to_string(item.id) + " ";
        });
        return result;
    }

    std::string dump_mates(const List &list, int id) {
        std::string result;
        list.for_each(Item{id, ""}, [&result](const Item &mate) {
            result += std::to_string(mate.id) + " ";
        });
        return result;
    }
}

TEST(MultiLinkedListTest, InsertKeepsRowOrderAndColumnRing) {
    List list(10, 12);
    list.insert(Item{1201, "A"});
    list.insert(Item{1001, "A"});
    list.insert(Item{1101, "-"});
    list.insert(Item{1102, "A"});

    EXPECT_EQ(dump(list), "1001 1101 1102 1201 ");
    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list.column_count(), 1);
    EXPECT_EQ(dump_mates(list, 1102), "1201 1001 ");
    EXPECT_EQ(dump_mates(list, 1101), "");
}

TEST(MultiLinkedListTest, InsertDuplicateFails) {
    List list(10, 12);

    EXPECT_TRUE(list.insert(Item{1001, "A"}));
    EXPECT_FALSE(list.insert(Item{1001, "B"}));
    EXPECT_EQ(list.size(), 1);
}

TEST(MultiLinkedListTest, RemoveUnlinksFromBothDirections) {
    List list(10, 12);
    list.insert(Item{1001, "A"});
    list.insert(Item{1101, "A"});
    list.insert(Item{1201, "B"});

    EXPECT_TRUE(list.remove(Item{1001, ""}));
    EXPECT_TRUE(list.remove(Item{1201, ""}));
    EXPECT_FALSE(list.remove(Item{9999, ""}));

    EXPECT_EQ(dump(list), "1101 ");
    EXPECT_EQ(list.column_count(), 1);
    EXPECT_EQ(dump_mates(list, 1101), "");
    EXPECT_EQ(list.min_row_key(), 11);
    EXPECT_EQ(list.max_row_key(), 11);
}

TEST(MultiLinkedListTest, FindReturnsStoredItem) {
    List list(10, 12);
    list.insert(Item{1001, "A"});

    const Item *found = list.find(Item{1001, ""});

    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->team, "A");
    EXPECT_EQ(list.find(Item{1002, ""}), nullptr);
}

TEST(MultiLinkedListTest, InsertOutOfRangeGrowsRows) {
    List list(10, 10);
    list.insert(Item{1001, "A"});
    list.insert(Item{801, "A"});
    list.insert(Item{1301, "A"});

    EXPECT_EQ(dump(list), "801 1001 1301 ");
    EXPECT_EQ(list.min_row_key(), 8);
    EXPECT_EQ(list.max_row_key(), 13);
    EXPECT_EQ(dump_mates(list, 801), "1001 1301 ");
}
