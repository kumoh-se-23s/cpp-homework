#include <ostream>
using namespace std;

class Fraction {
    public:
        Fraction();
        Fraction(int numerator, int denominator);

    private:
        int numerator; //분자
        int denominator; //분모

    public: 
        int getNumerator() const;
        void setNumerator(int numerator);

        int getDenominator() const;
        void setDenominator(int denominator);

        const Fraction operator+(const Fraction& f1) const;

        void set(int numerator, int denominator);
        // Fraction add(Fraction fraction);
        // void print();
};

ostream& operator<<(ostream& outputStream, const Fraction& f1);