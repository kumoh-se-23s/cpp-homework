class CharSequence {
    public:
    protected:
        static constexpr int MY_STRING_SIZE = 15;
        char charArr[MY_STRING_SIZE + 1];
        int stringLength = 0;

        void fillArray(char inputArr[]);

    public:
        virtual ~CharSequence() = default;

        virtual int length() const;
        virtual bool empty() const;
        virtual char at(int pos) const;

        virtual int find(const char subStr[], int pos=0) const;
        virtual int find(const CharSequence &sequence, int pos = 0) const;

    private:

    

};  