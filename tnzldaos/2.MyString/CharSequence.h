class CharSequence {
    public:
    protected:
        static constexpr int CAPACITY = 15;
        char charArr[CAPACITY + 1];
        int arrLength = 0;

        void fillArray(char inputArr[]);

    public:
        ~CharSequence() = default;

        void setChar(char inputChar, int pos);

        virtual int length() const;
        virtual bool empty() const;
        virtual char at(int pos) const;

        virtual int find(const char subStr[], int pos=0) const;
        virtual int find(const CharSequence &sequence, int pos = 0) const;

    private:

    

};  