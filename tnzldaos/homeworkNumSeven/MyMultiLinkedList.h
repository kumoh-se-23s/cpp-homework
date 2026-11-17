//
// Created by 김지환 on 26. 11. 14..
//

#ifndef CPP_HOMEWORK_STUDENTLINKEDLIST_H
#define CPP_HOMEWORK_STUDENTLINKEDLIST_H
#include <string>
using namespace std;

//tpp 사용하길

template<typename T>
class MyMultiLinkedList {
    struct Node {
        T object;

        //팀은 더블리, 학번은 싱글리로

        Node *down = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;

        explicit Node(T object) : object(object) {
        }
    };

    Node *head = nullptr;

public:
    void sortedInsert(T object) {
        Node *inputNode = new Node(object);

        if (head == nullptr) {
            head = inputNode;

            head->left = head;
            head->right = head;

            return;
        }

        Node *prevNode = head;
        Node *currentNode = head;
        Node *prevTeamNumNode = nullptr;

        bool isInsert = false;

        do {
            if (object.getTeamNum() != "-") {
                if (currentNode->object == object) {
                    prevTeamNumNode = currentNode;
                }
            }

            //current가 더 큰 경우
            if (currentNode->object <=> object == 1) {
                inputNode->down = currentNode;
                prevNode->down = inputNode;

                isInsert = true;
            }
            //object가 더 큰 경우
            else if (currentNode->object <=> object == -1) {
                prevNode = currentNode;
                currentNode = currentNode->down;
            }
            //같은 경우
            else {
                delete inputNode;
                return;
            }
        } while (isInsert == false && currentNode != nullptr);

        if (object.getTeamNum() == "-") {
            if (prevTeamNumNode != nullptr) {
                inputNode->left = prevTeamNumNode;
                inputNode->right = prevTeamNumNode->right;

                prevTeamNumNode->right = inputNode;
                prevTeamNumNode->right->left = inputNode;
            } else {
                inputNode->left = inputNode;
                inputNode->right = inputNode;
            }
        }
    }

    void remove(string studentId) {
        if (head == nullptr) {
            return;
        }

        Node *prevNode = head;
        Node* currentNode = head;

        do {
            if (currentNode->object == studentId) {
                currentNode->left->right = currentNode->right;
                currentNode->right->left = currentNode->left;

                prevNode->down = currentNode->down;

                delete currentNode;

            } else {
                prevNode = currentNode;
                currentNode = currentNode->down;
            }
        } while (currentNode != head);
    }
};


#endif //CPP_HOMEWORK_STUDENTLINKEDLIST_H
