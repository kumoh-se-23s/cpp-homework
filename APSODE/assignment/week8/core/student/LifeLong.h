#ifndef LIFELONG_H
#define LIFELONG_H
#include "visitor/Visitable.h"

class LifeLong final : public Visitable<LifeLong> {
    MyString m_uid;
    int m_tuition;

    public:
        LifeLong(const MyString &uid, const MyString &name, std::istream &input);


        const MyString& uid() const override;

        int tuition() const;


        void read_details(std::istream &input) override;

        void write(std::ostream &output) const override;
};

#endif //LIFELONG_H
