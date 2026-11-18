#ifndef LINKER_H
#define LINKER_H
#include <functional>

#include "../list/linked_list/node/AbstractNode.h"

template<
    typename Node,
    typename LinkPolicy,
    typename ComparePolicy
>
class Linker {
    Node* m_head;
    int m_size;

    public:
        Linker();

        ~Linker();

        Linker(const Linker& linker) = delete;

        Linker& operator=(const Linker& linker) = delete;

        Node* head() const;

        int size() const;

        bool empty() const;

        bool contains(const Node* node) const;

        void insert(Node* node);

        void remove(Node* node);

        void clear();

        void take(Linker& other);

        Node* find(std::function<bool(Node* node)> predicate) const;

        void for_each(std::function<void(Node* node)> consumer) const;

        void for_each(const Node* start, std::function<bool(Node* node)> consumer) const;


    private:
        Node* find_insert_pos(const Node* target) const;

        static Node* step(const Node* node, const Node* stop);
};

#include "Linker.tpp"

#endif //LINKER_H
