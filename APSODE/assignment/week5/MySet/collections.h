//
// Created by leegu on 26. 9. 7..
//

#ifndef CPP_HOMEWORK_COLLECTION_H
#define CPP_HOMEWORK_COLLECTION_H\


#include "iters.h"
using namespace iter;


namespace collections {
    template <typename E>
    class Collection : public Iterable<E>{
        public:
            virtual int get_size() const = 0;

            virtual int get_capacity() const = 0;

            virtual int add(const E &value) = 0;

            virtual int insert(int index, const E &value) = 0;

            virtual int remove_at(int index) = 0;

            virtual bool is_empty() = 0;

            virtual bool is_contains(const E &value) = 0;
    };

    template <typename E>
    class List : public Collection<E> {
        public:
            virtual const E& get(int index) = 0;

            virtual int set(int index, const E &value) = 0;

            virtual int append(const E &value) = 0;

            virtual int add_first(const E &value) = 0;

            virtual int add_last(const E &value) = 0;
    };
}

#endif //CPP_HOMEWORK_COLLECTION_H
