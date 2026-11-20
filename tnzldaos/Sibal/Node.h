#pragma once

template<typename T>
class Node {
    Node *nextId = nullptr;
    Node *nextTeamMember = nullptr;
    Node *prevTeamMember = nullptr;

    T* object;

public:
    explicit Node(T* object);

    ~Node();

    const T &getObject() const;

    Node *getNextNode() const;
    void linkNextNode(Node *node);

    Node *getNextTeamMember() const;
    Node *getPrevTeamMember() const;
    void linkNextTeamMember(Node *node);
    void linkPrevTeamMember(Node *node);
};

#include "Node.tpp"
