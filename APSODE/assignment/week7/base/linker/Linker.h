#ifndef LINKER_H
#define LINKER_H
#include <functional>

#include "../../core/LinkerConcept.h"

template<
    typename Node,
    LinkPolicyFor<Node> LinkPolicy,
    NodeComparator<Node> ComparePolicy
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

        Node* find_if(std::function<bool(const Node *node)> pred) const;

        void for_each(std::function<void(Node *node)> visit) const;

        void for_each(const Node* start, std::function<void(const Node *node)> visit) const;


    private:
        Node* find_insert_position(const Node* node) const;

        static Node* step(const Node* node, const Node* stop);
};

#include "Linker.tpp"

#endif //LINKER_H
