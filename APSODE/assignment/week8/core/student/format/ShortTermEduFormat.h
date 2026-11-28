#ifndef SHORTTERMEDUFORMAT_H
#define SHORTTERMEDUFORMAT_H
#include <ostream>
#include "StudentFormat.h"
#include "../ShortTermEdu.h"

class ShortTermEduFormat final : public StudentFormat {
    public:
        static constexpr const char *TAG = "ShortTermEdu";


        [[nodiscard]] const char* tag() const override;

        AdStudent* load(std::istream &input_stream) const override;

        AdStudent* create(std::istream &input_stream) const override;


        static ShortTermEdu* modify(const ShortTermEdu &original, std::istream &input_stream);

        static void write(std::ostream &output_stream, const ShortTermEdu &student);

    private:
        static ShortTermEdu* read(const MyString &uid, const MyString &name, std::istream &input_stream);
};

#endif //SHORTTERMEDUFORMAT_H
