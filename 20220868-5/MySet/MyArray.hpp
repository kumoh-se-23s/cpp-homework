#include "MyArray.h"

template <typename T> requires std::is_trivially_copyable_v<T>
MyArray<T>::MyArray(uint64_t capacity) : raw(new T[capacity])
{
}

template <typename T> requires std::is_trivially_copyable_v<T>
MyArray<T>::~MyArray(){
    delete[] raw;
}

template <typename T> requires std::is_trivially_copyable_v<T>
MyArray<T>::MyArray(const MyArray &other) noexcept
{
    raw = new T[other.capacity];
    size = other.size;
    capacity = other.capacity;
    for (uint64_t i = 0; i < size; ++i)
    {
        raw[i] = T(other.raw[i]);
    }
}
template <typename T> requires std::is_trivially_copyable_v<T>
MyArray<T> MyArray<T>::operator=(const MyArray &other) noexcept
{
    if(&other == this){
        return *this;
    }

    delete[] raw;
    raw = new T[other.capacity];
    size = other.size;
    capacity = other.capacity;
    for (uint64_t i = 0; i < size; ++i)
    {
        raw[i] = T(other.raw[i]);
    }
    return *this;
}

template <typename T> requires std::is_trivially_copyable_v<T>
MyArray<T>::MyArray(MyArray &&other) noexcept
{
    std::swap(raw, other.raw);
    std::swap(size, other.size);
    std::swap(capacity, other.capacity);
}
template <typename T> requires std::is_trivially_copyable_v<T>
MyArray<T> MyArray<T>::operator=(MyArray &&other) noexcept
{
    if(&other == this){
        return *this;
    }
    std::swap(raw, other.raw);
    std::swap(size, other.size);
    std::swap(capacity, other.capacity);
    return *this;
}

template <typename T> requires std::is_trivially_copyable_v<T>
void MyArray<T>::resize(uint64_t newSize, T setter)
{
    resizeCapacity(newSize);
    for (uint64_t i = size; i < newSize; ++i)
    {
        raw[i] = setter;
    }
}

template <typename T> requires std::is_trivially_copyable_v<T>
void MyArray<T>::resizeCapacity(uint64_t newCapacity) noexcept
{

    T *toMove = nullptr;
    if (newCapacity > 0)
    {
        toMove = new T[newCapacity];
        capacity = newCapacity;
        for (uint64_t i = 0; i < std::min(newCapacity, size); ++i)
        {
            toMove[i] = std::move(raw[i]);
        }
    }

    if (raw)
    {
        delete[] raw;
    }
    raw = toMove;
}

template <typename T> requires std::is_trivially_copyable_v<T>
void MyArray<T>::insert(int placementIndex, T v)
{
    if (size == capacity)
    {
        resizeCapacity(std::max(static_cast<uint64_t>(16), capacity * 2));
    }

    for(int i = size - 1; i >= placementIndex; --i){
        raw[i + 1] = std::move(raw[i]);
    }

    ++size;
    
    raw[placementIndex] = v;
}


template <typename T> requires std::is_trivially_copyable_v<T>
void MyArray<T>::add(T v)
{
    if (size == capacity)
    {
        resizeCapacity(std::max(static_cast<uint64_t>(16), capacity * 2));
    }
    ++size;
    raw[size - 1] = std::move(v);

}

template <typename T> requires std::is_trivially_copyable_v<T>
bool MyArray<T>::contains(T v) { 
    for(int i = 0; i < size; ++i){
        if(raw[i] == v) return true;
    }
    return false;
}

template<typename T> requires std::is_trivially_copyable_v<T>
void MyArray<T>::merge(const int left, const int mid, const int right) {
    MyArray temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if (raw[i] <= raw[j]) {
            temp.add(raw[i++]);
        } else {
            temp.add(raw[j++]);
        }
    }

    for (;i <= mid; ++i) {
        temp.add(raw[i]);
    }

    for (;j <= right; ++j) {
        temp.add(raw[j]);
    }

    for (int k = 0; k < temp.getSize(); k++) {
        raw[left + k] = temp[k];
    }
}
template<typename T> requires std::is_trivially_copyable_v<T>
void MyArray<T>::mergeSort() {
    mergeSort(0, size - 1);
}

template<typename T> requires std::is_trivially_copyable_v<T>
void MyArray<T>::mergeSort(const int left, const int right) {
    if (left >= right) {
        return;
    }

    const int mid = (left + right) / 2;
    mergeSort(left, mid);
    mergeSort(mid + 1, right);
    merge(left, mid, right);
}


template <typename T> requires std::is_trivially_copyable_v<T>
T MyArray<T>::remove(uint64_t index)
{
    T extracted = std::move(raw[index]);
    
    for (uint64_t i = index; i < size - 1; ++i)
    {
        raw[i] = std::move(raw[i + 1]);
    }

    --size;
    return extracted;
}

template <typename T> requires std::is_trivially_copyable_v<T>
T &MyArray<T>::operator[](uint64_t index)
{
    return raw[index];
}

template <typename T> requires std::is_trivially_copyable_v<T>
const T &MyArray<T>::operator[](uint64_t index) const
{
    return raw[index];
}