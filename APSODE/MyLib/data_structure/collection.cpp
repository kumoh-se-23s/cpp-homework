//
// Created by apsode on 26. 9. 3..
//

#include "collection.h"


namespace collection {
    template<typename E>
    Collection<E>::Collection() {
        capacity = 0;
    }

    template<typename E>
    Collection<E>::Collection(int manual_size) {
        container = new E[manual_size];
        capacity = 0;
    }

    template<typename E>
    bool Collection<E>::is_empty() {
    }

    template<typename E>
    int Collection<E>::size() {
    }
} // collection
