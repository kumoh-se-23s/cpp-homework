#ifndef STUDENTMODIFIER_H
#define STUDENTMODIFIER_H
#include <istream>
#include "../AdStudent.h"

class StudentModifier final : public StudentVisitor {
    std::istream &m_input_stream;
    AdStudent *m_modified;

    public:
        explicit StudentModifier(std::istream &input_stream);


        void visit(const Gifted &student) override;

        void visit(const LifeLong &student) override;

        void visit(const EmployeeEdu &student) override;

        void visit(const ShortTermEdu &student) override;


        [[nodiscard]] AdStudent* modified() const;
};

#endif //STUDENTMODIFIER_H
