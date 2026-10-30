//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "Node.h"

template<typename PrevNextGen, typename RAIINode>
class CircularLinkedList {
    RAIINode head = nullptr;
    int size = 0;

public:

    class Iterator {
        const CircularLinkedList *parent;
        RAIINode curr;
        int count = 0;
        int size;

    public:
        explicit Iterator(const CircularLinkedList *parent, RAIINode head, const int size) : parent(parent), curr(std::move(head)), size(size) {
        }

        RAIINode &operator*() {
            return curr;
        }

        Iterator &operator--() {
#ifndef NDEBUG
            if (count == 0) throw std::runtime_error("Iterator::operator--() called on begin");
#endif
            curr = PrevNextGen::prevNode(curr);
            --count;
            return *this;
        }

        Iterator &operator++() {
#ifndef NDEBUG
            if (count == size) throw std::runtime_error("Iterator::operator++() called on end");
#endif
            curr = PrevNextGen::nextNode(curr);
            ++count;
            return *this;
        }

        [[nodiscard]] bool hasNext() const {
            return count < size;
        }

    };

    explicit CircularLinkedList() = default;

    ~CircularLinkedList() {
        if (!head) return;
        for (auto it = begin(); it.hasNext(); ++it) {
            PrevNextGen::nextNode(PrevNextGen::prevNode(*it)) = nullptr;
            PrevNextGen::prevNode(*it) = nullptr;
        }
    }

    [[nodiscard]] const RAIINode &getHead() const {
        return head;
    }

    CircularLinkedList(const CircularLinkedList &other) = delete;
    CircularLinkedList &operator=(const CircularLinkedList &other) = delete;
    CircularLinkedList(CircularLinkedList &&other) noexcept {
        std::swap(head, other.head);
        std::swap(size, other.size);
    };
    CircularLinkedList &operator=(CircularLinkedList &&other) = delete;

    void addFirst(const RAIINode &node) {
        if (!head) {
            head = node;
            PrevNextGen::nextNode(head) = head;
            PrevNextGen::prevNode(head) = head;
        } else {
            RAIINode existingHead = head;
            head = node;
            PrevNextGen::prevNode(head) = PrevNextGen::prevNode(existingHead);
            PrevNextGen::nextNode(head) = existingHead;
            PrevNextGen::nextNode(PrevNextGen::prevNode(existingHead)) = head;
            PrevNextGen::prevNode(existingHead) = head;
        }
        ++size;
    }

    void insertBefore(RAIINode &loc, RAIINode &node) {
        PrevNextGen::prevNode(node) = PrevNextGen::prevNode(loc);
        PrevNextGen::nextNode(node) = loc;
        PrevNextGen::nextNode(PrevNextGen::prevNode(loc)) = node;
        PrevNextGen::prevNode(loc) = node;
        if (loc == head) {
            head = node;
        }
        ++size;
    }

    void insertAfter(RAIINode &loc, RAIINode &node) {
        PrevNextGen::nextNode(node) = PrevNextGen::nextNode(loc);
        PrevNextGen::prevNode(node) = loc;
        PrevNextGen::prevNode(PrevNextGen::nextNode(loc)) = node;
        PrevNextGen::nextNode(loc) = node;
        ++size;
    }

    void remove(const Student &student) {
        for (auto it = begin(); it.hasNext(); ++it) {
            if ((*it)->data == student) {
                remove(*it);
                break;
            }
        }
    }

    void remove(RAIINode &node) {
        if (!head || !node) return;
        if (node == head && size == 1) {
            head = nullptr;
            size = 0;
            return;
        }

        RAIINode next = PrevNextGen::nextNode(node);
        RAIINode prev = PrevNextGen::prevNode(node);

        PrevNextGen::nextNode(prev) = next;
        PrevNextGen::prevNode(next) = prev;

        if (node == head) head = next;

        --size;
    }


    [[nodiscard]] Iterator begin() const {
        return Iterator(this, head, size);
    }

    [[nodiscard]] int getSize() const {
        return size;
    }
};
