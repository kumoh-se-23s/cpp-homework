#pragma once

template<typename T>
MyDoublyLinkedList<T>::MyDoublyLinkedList(T *inputNode) : head(inputNode) {
}

template<typename T>
T *MyDoublyLinkedList<T>::getHead() const { return head; }

template<typename T>
template<typename U>
void MyDoublyLinkedList<T>::sortedInsert(T *input, Comparator<U> &comparator) {
    //헤더가 없는 경우에 삽입하는 경우
    if (head == nullptr) {
        head = input;
        return;
    }

    //맨 앞에 넣는 경우
    if (comparator.columnCompare(head->getObject(), input->getObject()) > 0) {
        input->linkNextTeamMember(head);
        head->linkPrevTeamMember(input);
        head = input;
        return;
    }

    bool isRun = true;

    T *prev = nullptr;
    T *current = head;

    while (isRun && current != nullptr) {
        if (comparator.columnCompare(current->getObject(), input->getObject()) > 0) {
            input->linkPrevTeamMember(prev);
            input->linkNextTeamMember(current);
            prev->linkNextTeamMember(input);
            current->linkPrevTeamMember(input);

            isRun = false;
        } else if (comparator.columnCompare(current->getObject(), input->getObject()) < 0) {
            prev = current;
            current = current->getNextTeamMember();
        } else {
            isRun = false;
        }
    }

    if (current == nullptr) {
        prev->linkNextTeamMember(input);
        input->linkPrevTeamMember(prev);
    }
}

template<typename T>
void MyDoublyLinkedList<T>::remove(T *target) {
    if (head == nullptr || target == nullptr) {
        return;
    }

    T *prev = target->getPrevTeamMember();
    T *next = target->getNextTeamMember();

    if (prev != nullptr) {
        prev->linkNextTeamMember(next);
    }
    if (next != nullptr) {
        next->linkPrevTeamMember(prev);
    }

    if (target == head) {
        head = next;
    }

    target->linkPrevTeamMember(nullptr);
    target->linkNextTeamMember(nullptr);
}
