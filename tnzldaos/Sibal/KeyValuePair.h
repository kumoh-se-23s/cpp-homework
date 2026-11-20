#pragma once
#include <string>
using namespace std;

template<class K, class V>
class KeyValuePair {
    K key{};
    V *value = nullptr;

public:
    KeyValuePair() = default;

    KeyValuePair(const K &key, V *value);

    ~KeyValuePair() = default;

    const K &getKey() const;
    V *getValue() const;

    void setKey(const K &key);
    void setValue(V *value);

    // 문자열과 key 비교 (pair == "A팀")
    bool operator==(const K &input) const;
};

#include "KeyValuePair.tpp"

