#ifndef STUDENTVISITOR_H
#define STUDENTVISITOR_H

class Gifted;
class LifeLong;
class EmployeeEdu;
class ShortTermEdu;

class StudentVisitor {
    public:
        virtual ~StudentVisitor() = default;


        virtual void visit(const Gifted &student) = 0;

        virtual void visit(const LifeLong &student) = 0;

        virtual void visit(const EmployeeEdu &student) = 0;

        virtual void visit(const ShortTermEdu &student) = 0;
};

#endif //STUDENTVISITOR_H
