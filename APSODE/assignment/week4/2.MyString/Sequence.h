//
// Created by leegu on 26. 10. 11..
//

#ifndef CPP_HOMEWORK_SEQUENCE_H
#define CPP_HOMEWORK_SEQUENCE_H

template <typename E>
class Sequence {
    public:
        virtual ~Sequence() = default;

        virtual int length() const = 0;

        virtual E at(int pos) const = 0;

        virtual bool set(E object, int pos) = 0;

        virtual void add_last(E object) = 0;

        virtual bool empty() const = 0;
};

#endif //CPP_HOMEWORK_SEQUENCE_H
