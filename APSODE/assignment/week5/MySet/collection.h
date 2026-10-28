//
// Created by apsode on 26. 10. 28..
//

#ifndef CPP_HOMEWORK_COLLECTION_H
#define CPP_HOMEWORK_COLLECTION_H
template<typename E>
class Collection {
    public:
        virtual E get(int index) = 0;

        virtual void set(int index, const E &other) = 0;

        virtual void add(const E &other) = 0;
};

#endif //CPP_HOMEWORK_COLLECTION_H
