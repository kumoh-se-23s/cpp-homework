#ifndef ROW_TABLE_H
#define ROW_TABLE_H
#include <functional>

#include "../linker/Linker.h"
#include "../linker/RowLinkPolicy.h"
#include "../linker/NodeCompare.h"
#include "../list/linked_list/node/MultiConnectNode.h"
#include "../../core/TraitsConcept.h"

template<typename T, RowTraits<T> Traits>
using RowChain = Linker<MultiConnectNode<T>, RowLinkPolicy<T>, NodeCompare<T, Traits> >;

template<typename T, RowTraits<T> Traits>
class RowTable {
    RowChain<T, Traits> *m_buckets;
    int m_base_key;
    int m_bucket_count;

    public:
        RowTable(int min_key, int max_key);

        ~RowTable();

        RowTable(const RowTable &other) = delete;

        RowTable& operator=(const RowTable &other) = delete;


        void link(MultiConnectNode<T> *node);

        void unlink(MultiConnectNode<T> *node);

        MultiConnectNode<T>* find_if(int row_key, std::function<bool(const MultiConnectNode<T> *node)> pred) const;

        int min_key() const;

        int max_key() const;

        void for_each(std::function<void(MultiConnectNode<T> *node)> visit) const;

    private:
        bool in_range(int row_key) const;

        RowChain<T, Traits>& bucket_of(int row_key) const;

        int last_key() const;

        void grow_to(int row_key);
};

#include "RowTable.tpp"

#endif //ROW_TABLE_H
