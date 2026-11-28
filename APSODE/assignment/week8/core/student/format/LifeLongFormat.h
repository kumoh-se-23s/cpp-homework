#ifndef LIFELONGFORMAT_H
#define LIFELONGFORMAT_H
#include <ostream>
#include "StudentFormat.h"
#include "../LifeLong.h"

class LifeLongFormat final : public StudentFormat {
    public:
        static constexpr const char *TAG = "LifeLong";


        [[nodiscard]] const char* tag() const override;

        AdStudent* load(std::istream &input_stream) const override;

        AdStudent* create(std::istream &input_stream) const override;


        static LifeLong* modify(const LifeLong &original, std::istream &input_stream);

        static void write(std::ostream &output_stream, const LifeLong &student);

    private:
        static LifeLong* read(const MyString &uid, const MyString &name, std::istream &input_stream);
};

#endif //LIFELONGFORMAT_H
