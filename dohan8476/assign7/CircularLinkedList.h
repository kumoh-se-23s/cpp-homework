#pragma once

template <typename T>
class CircularLinkedList {
    T* head;
    int size;

    public:
        CircularLinkedList();
        ~CircularLinkedList();
        void add(T* newNode);
        void append(T* newNode);
        void insert(int index, T* newNode);
        void insertAscend(T* newNode);

        T* remove(T* target);
        template <typename KeyType>
        T* remove(const KeyType& element);
        T* remove(int index);
        T* removeFirst();
        void clear();

        int getSize() const;
        bool isEmpty() const;
        T* get(int index) const;
        T* getHead() const;

        void moveFrom(CircularLinkedList& other);
};

template <typename T>
CircularLinkedList<T>::CircularLinkedList() : head(nullptr), size(0) {}

template <typename T>
CircularLinkedList<T>::~CircularLinkedList() {
    clear();
}

template<typename T>
void CircularLinkedList<T>::add(T* newNode) {
    if (newNode == nullptr) return;

    //nextTeam >> StudentNode 전용인데 템플릿 의미가 잇나;
    //종속성 해결해야할듯
    if (isEmpty()) {
        head = newNode;
        newNode->nextTeam = newNode;
        newNode->prevTeam = newNode;
    } else {
        T* tail = head->prevTeam;

        newNode->nextTeam = head;
        newNode->prevTeam = tail;
        tail->nextTeam = newNode;
        head->prevTeam = newNode;

        head = newNode;
    }
    ++size;
}

template<typename T>
void CircularLinkedList<T>::append(T* newNode) {
    if (newNode == nullptr) return;

    if (isEmpty()) {
        add(newNode);
        return;
    }

    T* tail = head->prevTeam;

    newNode->nextTeam = head;
    newNode->prevTeam = tail;
    tail->nextTeam = newNode;
    head->prevTeam = newNode;

    ++size;
}

template<typename T>
void CircularLinkedList<T>::insert(int index, T* newNode) {
    if (newNode == nullptr || index < 0 || index > size) return;

    if (index == 0) {
        add(newNode);
    } else if (index == size) {
        append(newNode);
    } else {
        T* prevNode = head;
        for (int i = 0; i < index - 1; i++) {
            prevNode = prevNode->nextTeam;
        }
        T* nextNode = prevNode->nextTeam;

        newNode->nextTeam = nextNode;
        newNode->prevTeam = prevNode;
        prevNode->nextTeam = newNode;
        nextNode->prevTeam = newNode;
        ++size;
    }
}

template<typename T>
void CircularLinkedList<T>::insertAscend(T *newNode) {
    if (newNode == nullptr) return;

    if (isEmpty() || *newNode < *head) {
        add(newNode);
        return;
    }

    T* curr = head;
    while (curr->nextTeam != head && *(curr->nextTeam) < *newNode) {
        curr = curr->nextTeam;
    }

    T* nextNode = curr->nextTeam;
    newNode->nextTeam = nextNode;
    newNode->prevTeam = curr;
    curr->nextTeam = newNode;
    nextNode->prevTeam = newNode;
    ++size;
}

// 노드 포인터를 넘겨서 포인터 끊는 역할
template<typename T>
T *CircularLinkedList<T>::remove(T *target) {
    if (isEmpty() || target == nullptr || target->nextTeam == nullptr || target->prevTeam == nullptr) {
        return nullptr;
    }

    if (size == 1 && target == head) {
        head = nullptr;
    } else {
        T* prevNode = target->prevTeam;
        T* nextNode = target->nextTeam;

        prevNode->nextTeam = nextNode;
        nextNode->prevTeam = prevNode;

        if (target == head) {
            head = nextNode;
        }
    }

    target->nextTeam = nullptr;
    target->prevTeam = nullptr;
    --size;
    return target;
}

template<typename T> template<typename KeyType>
T *CircularLinkedList<T>::remove(const KeyType &element) {
    if (isEmpty()) return nullptr;

    T* curr = head;
    for (int i = 0; i < size; i++) {
        if (*curr == element) {
            return remove(curr);
        }
        curr = curr->nextTeam;
    }
    return nullptr;
}

template<typename T>
T* CircularLinkedList<T>::remove(int index) {
    if (index < 0 || index >= size || isEmpty()) return nullptr;
    return remove(get(index));
}

template<typename T>
int CircularLinkedList<T>::getSize() const{
    return size;
}

template<typename T>
bool CircularLinkedList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
T* CircularLinkedList<T>::get(int index) const {
    if (index < 0 || index >= size || isEmpty()) return nullptr;

    T* curr = head;
    for (int i = 0; i < index; i++) {
        curr = curr->nextTeam;
    }
    return curr;
}

template<typename T>
T* CircularLinkedList<T>::getHead() const {
    return head;
}

template<typename T>
T* CircularLinkedList<T>::removeFirst() {
    if (isEmpty()) return nullptr;
    return remove(head);
}

template <typename T>
void CircularLinkedList<T>::clear() {
    head = nullptr;
    size = 0;
}

template<typename T>
void CircularLinkedList<T>::moveFrom(CircularLinkedList &other) {
    this->head = other.head;
    this->size = other.size;
    other.head = nullptr;
    other.size = 0;
}
