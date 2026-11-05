#pragma once
#include<iostream>

template <typename T>
class ArrayList {

    T *array;

    int capacity;

    int size;

    constexpr static int DEFAULT_CAPACITY = 4;

public:

    ArrayList(int c = DEFAULT_CAPACITY) : array(new T[std::max(c, 0)]), capacity(std::max(c, 0)), size(0) {}

    ArrayList(const ArrayList &otherArray);

    ArrayList(ArrayList &&otherArray) noexcept;

    ArrayList(const T *objectArray);

    ArrayList(T *&&objectArray);

    ~ArrayList();

    ArrayList& operator =(const ArrayList &otherArray);

    ArrayList& operator =(ArrayList &&otherSet) noexcept;

    ArrayList operator +(const ArrayList &otherSet) const;

    ArrayList operator +(const T *objectArray) const;

    ArrayList operator +(T &object) const;

    int getLength() const;

    int getCapacity() const;

    int get(int index) const;

    void clear();

    void resize();

private:
    void append(const T appendValue);

    void append(const T *appendValue);

    void append(const ArrayList &objectArray);
};
template <typename T>
std::istream& operator>>(std::istream& in, ArrayList<T>& arrayList);

template <typename T>
std::ostream& operator<<(std::ostream& out, const ArrayList<T>& arrayList);


