//
// Created by apsode on 26. 9. 4..
//

#include "ScopedPointer.h"

namespace ScopedPointer {
    template<typename T>
    T* ScopedPointer<T>::get() const {
        return this->target;
    }


    template<typename T>
    T& ScopedPointer<T>::operator*() const {
        return *this->target;
    }

    template<typename T>
    T* ScopedPointer<T>::operator->() const {
        return this->target;
    }

} // ScopedPointer