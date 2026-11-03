//
// Created by apsode on 26. 11. 3..
//

#ifndef CPP_HOMEWORK_WEEK6_COMPARABLE_H
#define CPP_HOMEWORK_WEEK6_COMPARABLE_H

template<typename T>
class Comparable {
    public:
        virtual ~Comparable() = default;

        virtual int compare_to(const T &other) const = 0;
};

#endif //CPP_HOMEWORK_WEEK6_COMPARABLE_H
