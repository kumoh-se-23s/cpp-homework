#ifndef STUDENTFACTORY_H
#define STUDENTFACTORY_H
#include <functional>
#include <istream>
#include "../AdStudent.h"

class StudentFactory {
    struct Maker {
        MyString m_type;
        std::function<AdStudent*(std::istream &)> m_load;
        std::function<AdStudent*(std::istream &)> m_create;
    };

    static constexpr int KIND_COUNT = 4;

    Maker m_makers[KIND_COUNT];

    public:
        StudentFactory();


        AdStudent* load(std::istream &input) const;

        AdStudent* create(std::istream &input) const;

    private:
        const Maker* find(std::istream &input) const;


        template<typename Student>
        static AdStudent* read_with_uid(std::istream &input);

        template<typename Student>
        static AdStudent* read_with_issued_uid(std::istream &input);
};

#endif //STUDENTFACTORY_H
