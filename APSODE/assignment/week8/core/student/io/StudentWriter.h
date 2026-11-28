#ifndef STUDENTWRITER_H
#define STUDENTWRITER_H
#include <ostream>
#include "../visitor/StudentVisitor.h"

class StudentWriter final : public StudentVisitor {
    std::ostream &m_output;

    public:
        explicit StudentWriter(std::ostream &output);


        void visit(const Gifted &student) override;

        void visit(const LifeLong &student) override;

        void visit(const EmployeeEdu &student) override;

        void visit(const ShortTermEdu &student) override;
};

#endif //STUDENTWRITER_H
