#ifndef STUDENTFORMAT_H
#define STUDENTFORMAT_H
#include <istream>
#include "../AdStudent.h"

class StudentFormat {
    public:
        virtual ~StudentFormat() = default;


        [[nodiscard]] virtual const char* tag() const = 0;

        virtual AdStudent* load(std::istream &input_stream) const = 0;

        virtual AdStudent* create(std::istream &input_stream) const = 0;
};

#endif //STUDENTFORMAT_H
