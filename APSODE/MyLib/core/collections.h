//
// Created by leegu on 26. 9. 7..
//

#ifndef CPP_HOMEWORK_COLLECTION_H
#define CPP_HOMEWORK_COLLECTION_H\


#include "iters.h"


namespace collections {
    template <typename E>
    class Collection : public iter::Iterable<E>{
        public:
            virtual int getSize() const = 0;

            virtual int getCapacity() const = 0;

            virtual int add(const E &value) = 0;

            virtual int insert(int index, const E &value) = 0;

            virtual int removeAt(int index) = 0;

            virtual bool isEmpty() = 0;

            virtual bool isContains(const E &value) = 0;
    };

    template <typename E>
    class List : public Collection<E> {
        public:
            virtual const E& get(int index) = 0;

            virtual int set(int index, const E &value) = 0;

            virtual int append(const E &value) = 0;

            virtual int addFirst(const E &value) = 0;

            virtual int addLast(const E &value) = 0;
    };
}

#endif //CPP_HOMEWORK_COLLECTION_H
