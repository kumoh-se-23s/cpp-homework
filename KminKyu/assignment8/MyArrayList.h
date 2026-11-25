#ifndef CPP_HOMEWORK_GIT_MYARRAYLIST_H
#define CPP_HOMEWORK_GIT_MYARRAYLIST_H

/**
 * String split한 정보를 저장할때 사용하는 ArrayList
 *
 */
template <typename T>
class MyArrayList {
    constexpr static int DEFAULT_CAPACITY = 8;

    T *items = new T[DEFAULT_CAPACITY];

    int size = 0;

    int capacity = DEFAULT_CAPACITY;
public:
    MyArrayList() = default;

    MyArrayList(int capacity);

    MyArrayList(const MyArrayList& other);

    MyArrayList(MyArrayList &&other) noexcept;

    ~MyArrayList();

    MyArrayList & operator=(const MyArrayList &other);

    MyArrayList & operator=(MyArrayList &&other) noexcept;

    const T& at(int pos) const;

    int getSize() const;

    int getCapacity() const;

    void append(const T& item);

    void clear();

private:
    void resize();
};


#include "MyArrayList.tpp"
#endif //CPP_HOMEWORK_GIT_MYARRAYLIST_H
