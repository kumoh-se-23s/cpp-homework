#ifndef SHORTTERMEDU_H
#define SHORTTERMEDU_H
#include "visitor/Visitable.h"
#include "mixin/AutoUID.h"

class ShortTermEdu final : public Visitable<ShortTermEdu>, AutoUID<ShortTermEdu, 'S'> {
    MyString m_uid;
    MyString m_host_depart;

    public:
        ShortTermEdu(const MyString &name, std::istream &input);

        ShortTermEdu(const MyString &uid, const MyString &name, std::istream &input);


        const MyString& uid() const override;

        const MyString& host_depart() const;


        void read_details(std::istream &input) override;

        void write(std::ostream &output) const override;

        void release_uid() const override;
};

#endif //SHORTTERMEDU_H
