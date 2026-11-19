#ifndef LINKER_CONCEPT_H
#define LINKER_CONCEPT_H
#include <concepts>

template<typename Policy, typename Node>
concept LinkPolicyFor = requires(Node *&head, Node *node, const Node *const_node) {
    { Policy::next(const_node) } -> std::convertible_to<Node*>;
    Policy::link_front(head, node);
    Policy::link_after(node, node);
    Policy::unlink(head, node);
    Policy::reset(node);
};

template<typename Compare, typename Node>
concept NodeComparator = requires(const Node *left, const Node *right) {
    { Compare::less(left, right) } -> std::convertible_to<bool>;
};

#endif //LINKER_CONCEPT_H
