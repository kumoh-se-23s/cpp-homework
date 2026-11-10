//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "DoubleNode.h"

template<typename NextGen, typename RAIINode>
class SingleLinkedList {
    RAIINode head = nullptr;
    int size = 0;

public:

    class Iterator {
        const SingleLinkedList *parent;
        RAIINode curr;
        int count = 0;
        int size;

    public:
        explicit Iterator(const SingleLinkedList *parent, RAIINode head, const int size) : parent(parent), curr(std::move(head)), size(size) {
        }

        RAIINode &operator*() {
            return curr;
        }

        Iterator &operator++() {
#ifndef NDEBUG
            if (count == size) throw std::runtime_error("Iterator::operator++() called on end");
#endif
            curr = NextGen::nextNode(curr);
            ++count;
            return *this;
        }

        [[nodiscard]] bool hasNext() const {
            return count < size;
        }

    };

    explicit SingleLinkedList() = default;

    ~SingleLinkedList() = default; // delete chaining

    [[nodiscard]] const RAIINode &getHead() const {
        return head;
    }

    SingleLinkedList(const SingleLinkedList &other) = delete;
    SingleLinkedList &operator=(const SingleLinkedList &other) = delete;
    SingleLinkedList(SingleLinkedList &&other) noexcept {
        std::swap(head, other.head);
        std::swap(size, other.size);
    };
    SingleLinkedList &operator=(SingleLinkedList &&other) = delete;

    void addFirst(const RAIINode &node) {
        if (!head) {
            head = node;
        } else {
            RAIINode existingHead = head;
            head = node;
            NextGen::nextNode(head) = existingHead;
        }
        ++size;
    }


    void insertAfter(RAIINode &loc, RAIINode &node) {
        NextGen::nextNode(node) = NextGen::nextNode(loc);
        NextGen::nextNode(loc) = node;
        ++size;
    }

    void remove(const RAIINode &student) {

        if (head == student) {
            removeFirst();
            return;
        }
        for (auto it = begin(); it.hasNext(); ++it) {
            if (NextGen::nextNode(*it) == student) {
                removeNext(*it);
                break;
            }
        }
    }

    void removeFirst() {
        if (!head) return;
        // head destructor is called first
        // if head = nextNode(head), nextNode also be deleted after head destructor is called(chaining)
        // nextNode will be dangling
        auto temp = NextGen::nextNode(head);
        head = temp;
        --size;
    }
    void removeNext(RAIINode &node) {
        if (!head || !node) return;

        RAIINode next = NextGen::nextNode(node);
        if (!next) return;

        RAIINode nextNext = NextGen::nextNode(next);
        NextGen::nextNode(node) = nextNext;

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
