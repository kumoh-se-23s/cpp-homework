#ifndef GIFTEDFORMAT_H
#define GIFTEDFORMAT_H
#include <ostream>
#include "StudentFormat.h"
#include "../Gifted.h"

class GiftedFormat final : public StudentFormat {
    public:
        static constexpr const char *TAG = "Gifted";


        [[nodiscard]] const char* tag() const override;

        AdStudent* load(std::istream &input_stream) const override;

        AdStudent* create(std::istream &input_stream) const override;


        static Gifted* modify(const Gifted &original, std::istream &input_stream);

        static void write(std::ostream &output_stream, const Gifted &student);

    private:
        static Gifted* read(const MyString &uid, const MyString &name, std::istream &input_stream);
};

#endif //GIFTEDFORMAT_H
