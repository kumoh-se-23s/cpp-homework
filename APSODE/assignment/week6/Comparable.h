//
// Created by apsode on 26. 11. 5..
//

#ifndef CPP_HOMEWORK_COMPARABLE_H
#define CPP_HOMEWORK_COMPARABLE_H

template<typename T>
class Comparable {
    public:


        virtual int compare(const T& other) const = 0;


    protected:
        Comparable() = default;

        ~Comparable() = default;
};

#endif //CPP_HOMEWORK_COMPARABLE_H
