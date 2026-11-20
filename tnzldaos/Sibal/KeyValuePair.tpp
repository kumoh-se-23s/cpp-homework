#pragma once

template<class K, class V>
KeyValuePair<K, V>::KeyValuePair(const K &key, V *value) : key(key), value(value) {
}

template<class K, class V>
const K &KeyValuePair<K, V>::getKey() const { return key; }

template<class K, class V>
V *KeyValuePair<K, V>::getValue() const { return value; }

template<class K, class V>
void KeyValuePair<K, V>::setKey(const K &key) { this->key = key; }

template<class K, class V>
void KeyValuePair<K, V>::setValue(V *value) { this->value = value; }

// 문자열과 key 비교 (pair == "A팀")
template<class K, class V>
bool KeyValuePair<K, V>::operator==(const K &input) const { return key == input; }
