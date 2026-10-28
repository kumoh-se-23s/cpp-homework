//
// Created by apsode on 26. 10. 28..
//

#ifndef CPP_HOMEWORK_COLLECTION_H
#define CPP_HOMEWORK_COLLECTION_H

template<typename E>
class Collection {
    public:
        virtual ~Collection() = default;

        virtual E get(int index) = 0;

        virtual void set(int index, const E &other) = 0;

        virtual void add(const E &other) = 0;

        virtual void add_all(const E *others, int count) = 0;

        virtual E remove(int index) = 0;

        virtual int size() = 0;

        virtual bool empty() = 0;

        virtual bool contains(const E &object) = 0;
};

#endif //CPP_HOMEWORK_COLLECTION_H
