#include <iostream>
#include <string>
using namespace std;

class Money {
    public:
    private:
        int dollar;
        int cent;

    public:
        Money();
        Money(int dollar, int cent);

        int getDollar() const;
        void setDollar(int dollar);

        int getCent() const;
        void setCent(int cent);

        void calculateDollarCent();
        //toString 내에서의 getter
        
        const Money operator+(const Money& money);
        const Money operator-(const Money& money);
        
        bool operator==(const Money& money);
        bool operator!=(const Money& money);
        bool operator>=(const Money& money);
        bool operator<=(const Money& money);
        bool operator>(const Money& money);
        bool operator<(const Money& money);
};

ostream& operator<<(ostream& outputStream, const Money& money);
istream& operator>>(istream& inputStream, Money& money);