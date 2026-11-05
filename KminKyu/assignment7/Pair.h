#pragma once


template <typename K, typename V>
class Pair {
    K key;
    V value;
public:
    Pair() = default;

    Pair(const K &key, const V &value): key(key),value(value) {
    }

    ~Pair();

    Pair(const Pair &other);

    Pair(Pair &&other) noexcept;

    Pair & operator=(const Pair &other);

    Pair & operator=(Pair &&other) noexcept;

    K getKey() const;

    V getValue() const;


};



