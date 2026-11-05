//
// Created by KminKyu on 2026-11-05.
//

#include "ArrayList.h"
template<typename T>
std::istream& operator>>(std::istream &in, ArrayList<T> &arrayList) {
    return in;
}

template<typename T>
std::ostream& operator<<(std::ostream &out, const ArrayList<T> &arrayList) {
    return out;
}
