//
// Created by 김지환 on 26. 11. 17..
//

#ifndef CPP_HOMEWORK_KEYVALUENODE_H
#define CPP_HOMEWORK_KEYVALUENODE_H

#include <string>
using namespace std;

template<typename T>
class KeyValueNode {
    string key;
    T value;

public:
    KeyValueNode() = default;

    KeyValueNode(string key, T value) : key(key), value(value) {
    }

    string getKey() const { return key; }

    void setKey(const string &key) { this->key = key; }

    T &getValue() const { return value; }

    void setValue(const T &value) { this->value = value; }
};


#endif //CPP_HOMEWORK_KEYVALUENODE_H
