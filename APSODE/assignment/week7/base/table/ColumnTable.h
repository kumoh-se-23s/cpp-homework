#ifndef COLUMN_TABLE_H
#define COLUMN_TABLE_H
#include <functional>

#include "../MyString.h"
#include "../linker/Linker.h"
#include "../linker/ColumnLinkPolicy.h"
#include "../linker/NodeCompare.h"
#include "../list/linked_list/node/MultiConnectNode.h"
#include "../../core/TraitsConcept.h"

template<typename T, ColumnTraits<T> Traits>
using ColumnRing = Linker<MultiConnectNode<T>, ColumnLinkPolicy<T>, NodeCompare<T, Traits>>;

template<typename T, ColumnTraits<T> Traits>
class ColumnTable {
    static constexpr int DEFAULT_CAPACITY = 13;

    struct Entry {
        MyString key;
        ColumnRing<T, Traits> ring;
        Entry *next;

        explicit Entry(const MyString &key);
    };

    Entry **m_buckets;
    int m_capacity;
    int m_entry_count;

    public:
        ColumnTable();

        ~ColumnTable();

        ColumnTable(const ColumnTable &other) = delete;

        ColumnTable& operator=(const ColumnTable &other) = delete;


        void link(MultiConnectNode<T> *node);

        void unlink(MultiConnectNode<T> *node);

        void for_each(const MultiConnectNode<T> *node, std::function<void(const MultiConnectNode<T> *mate)> visit) const;

        int count() const;

    private:
        int hash(const MyString &key) const;

        Entry* find_entry(const MyString &key) const;

        Entry* find_or_create_entry(const MyString &key);

        void remove_entry(const MyString &key);
};

#include "ColumnTable.tpp"

#endif //COLUMN_TABLE_H
