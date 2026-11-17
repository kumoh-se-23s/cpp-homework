//
// Created by 김지환 on 26. 11. 17..
//

#ifndef CPP_HOMEWORK_MYSINGLYLINKEDLIST_H
#define CPP_HOMEWORK_MYSINGLYLINKEDLIST_H

template<typename T>
class MySinglyLinkedList {
    struct Node {
        T object;

        Node *next = nullptr;

        explicit Node(T object) : object(object) {
        };
    };

    Node *head = nullptr;

public:
    MySinglyLinkedList() = default;

    void sortedInsert(T object) {
        Node *newNode = new Node(object);

        if (head == nullptr) {
            head = newNode;

            return;
        }

        Node *prevNode = head;
        Node *currentNode = head;

        while (currentNode->next != nullptr) {
            switch (currentNode->object <=> object == 1) {
                case 1:
                    newNode->next = currentNode;
                    prevNode->next = newNode;
                    break;

                case -1:
                    prevNode = currentNode;
                    currentNode = currentNode->next;
                    break;

                default:
                    break;
            }
        }

        currentNode->next = newNode;
    }

    void remove(T object) {
        Node *prevNode = head;
        Node *currentNode = head;

        while (currentNode != nullptr) {
            switch (currentNode->object <=> object) {
                case 0:
                    prevNode->next = currentNode->next;
                    delete currentNode;
                    return;

                default:
                    prevNode = currentNode;
                    currentNode = currentNode->next;
                    break;
            }
        }
    }
};


#endif //CPP_HOMEWORK_MYSINGLYLINKEDLIST_H
