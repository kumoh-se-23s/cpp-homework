#ifndef EMPLOYEEEDUFORMAT_H
#define EMPLOYEEEDUFORMAT_H
#include <ostream>
#include "StudentFormat.h"
#include "../EmployeeEdu.h"

class EmployeeEduFormat final : public StudentFormat {
    public:
        static constexpr const char *TAG = "EmployeeEdu";


        [[nodiscard]] const char* tag() const override;

        AdStudent* load(std::istream &input_stream) const override;

        AdStudent* create(std::istream &input_stream) const override;


        static EmployeeEdu* modify(const EmployeeEdu &original, std::istream &input_stream);

        static void write(std::ostream &output_stream, const EmployeeEdu &student);

    private:
        static EmployeeEdu* read(const MyString &uid, const MyString &name, std::istream &input_stream);
};

#endif //EMPLOYEEEDUFORMAT_H
